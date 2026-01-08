#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PluginParameters.h"
#include <array>
#include <cmath>
#include <memory>

#include "FormaSynth.h"
#include "ath_dsp/control/Parameter.h"
#include "ath_dsp/control/Midi.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_data_structures/juce_data_structures.h"

#include <chrono>
#include <ratio>

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER", createParameterLayout()),
    pluginInstanceSettings("pluginInstanceSettings")
{
    using namespace Ath::Forma;
    for (int i = 0; i < PARAM_COUNT; ++i) treeState.addParameterListener(ParametersByID[i].id, this);
}

PluginProcessor::~PluginProcessor()
{
    using namespace Ath::Forma;
    for (int i = 0; i < PARAM_COUNT; ++i) treeState.removeParameterListener(ParametersByID[i].id, this);
}

void PluginProcessor::parameterChanged (const juce::String& parameterId, float newValue) 
{
    formaSynth.setParameter(treeState.getParameter(parameterId)->getParameterIndex(), newValue);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    formaSynth.setContext(sampleRate);

    performance.SampleRate.write(sampleRate);
    performance.SamplesPerBlock.write(samplesPerBlock);

    using namespace Ath::Forma;
    for (int i = 0; i < PARAM_COUNT; i++)
    {
        auto parameter = treeState.getParameter(ParametersByID[i].id);
        parameter->sendValueChangedMessageToListeners(parameter->getValue());
    }
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, numSamples);
    }

    auto ch0 = buffer.getWritePointer(0);
    auto ch1 = buffer.getWritePointer(1);

    int midiEventCount = midiMessages.getNumEvents();
    juce::MidiBufferIterator iterator = midiMessages.begin();

    for (int i = 0; i < midiEventCount; i++)
    {
        juce::MidiMessageMetadata juceMetadata = *iterator;
        Ath::Control::Midi::MessageMeta metadata =
        {
        Ath::Control::Midi::Message::fromRawData(juceMetadata.data, juceMetadata.numBytes),
        juceMetadata.samplePosition
        };

        midiEvents[i] = metadata;
        iterator++;
    }
    midiMessages.clear();

    auto t1 = std::chrono::high_resolution_clock::now();
    formaSynth.process(ch0, numSamples, midiEvents.data(), midiEventCount);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration<float, std::nano>(t2 - t1).count();

    performance.ExecutionTimeImmediate.write(ns);

    float integratedTime100 = performance.ExecutionTime100.value * 0.99f + ns * 0.01f;
    performance.ExecutionTime100.write(integratedTime100);

    float integratedTime1000 = performance.ExecutionTime1000.value * 0.999f + ns * 0.001f;
    performance.ExecutionTime1000.write(integratedTime1000);

    if (ns > performance.MaxExecutionTimeNs.value) performance.MaxExecutionTimeNs.write(ns);
    if (ns < performance.MinExecutionTimeNs.value) performance.MinExecutionTimeNs.write(ns);

    buffer.copyFrom(1, 0, buffer.getReadPointer(0), numSamples);

    using namespace Ath::Forma;

    for (int i = 0; i < PARAM_COUNT; i++)
    {
        setParameter(ParametersByID[i], formaSynth.getParameter(i));
    }
    
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    auto editor = new PluginEditor (*this);
    return editor;
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();

    auto xml = std::make_unique<juce::XmlElement>("pluginInstanceTree");
    auto xmlSettings = std::make_unique<juce::XmlElement>(pluginInstanceSettings);

    std::unique_ptr<juce::XmlElement> pluginValueTree (state.createXml());

    xml->addChildElement(xmlSettings.release());
    xml->addChildElement(pluginValueTree.release());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("pluginInstanceTree"))
        {
            auto state = xmlState->getChildByName(treeState.state.getType());
            if (state != nullptr) treeState.replaceState (juce::ValueTree::fromXml (*state));

            auto settings = xmlState->getChildByName("pluginInstanceSettings");
            if (settings != nullptr) pluginInstanceSettings = *settings;
        }        
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    for (int i = 0; i < Ath::Forma::PARAM_COUNT; ++i)
    {
        auto paramData = Ath::Forma::ParametersByID[i];        

        switch (paramData.type)
        {
            case Ath::Control::Parameter::Type::Float:
            {
                params.push_back (std::make_unique<juce::AudioParameterFloat> (
                    juce::ParameterID {paramData.id, 0},
                    paramData.name,
                    juce::NormalisableRange<float>(paramData.min, paramData.max),
                    paramData.def,
                    paramData.name,
                    static_cast<juce::AudioProcessorParameter::Category>(paramData.category),
                    [paramData](float value, int){ return juce::String(paramData.getStringFromValue(value)); }
                    )
                );
                break;
            }
            case Ath::Control::Parameter::Type::Bool:
            {
                params.push_back(std::make_unique<juce::AudioParameterBool>(
                        juce::ParameterID {paramData.id, 0},
                        paramData.name,
                        paramData.def >= 0.5,
                        paramData.name,
                        [paramData](bool value, int){ return juce::String(value ? paramData.onText : paramData.offText); }
                    )
                );

                break;
            }
            case Ath::Control::Parameter::Type::Choice:
            {
                juce::StringArray strings;
                for (auto& string : paramData.choices) strings.add(string);

                params.push_back(std::make_unique<juce::AudioParameterChoice>(
                        juce::ParameterID {paramData.id, 0},
                        paramData.name,
                        strings,
                        paramData.def,
                        paramData.name,
                        [paramData](int index, int){ return juce::String(paramData.choices[index]); }
                    )
                );

                break;
            }

        }
    }

    return {params.begin(), params.end()};
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

//===============================================================================

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

