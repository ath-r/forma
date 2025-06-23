#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PluginParameters.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numbers>


#include "VeronikaVoice.h"
#include "control/Midi.h"
#include "dsp/Context.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_data_structures/juce_data_structures.h"

#include "dsp/BlitSquare.h"
#include "math/Math.h"
#include "control/Events.h"

namespace Electrophilia::Plugin::Tremolo
{

}

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
    treeState(*this, nullptr, "PARAMETER", createParameterLayout())
{
    for (int i = 0; i < 16; i++)
    {
        voiceManager.noteOn_out(i).addMemberCallback(voices[i], &Electrophilia::Veronika::VeronikaVoice::handleNoteOn);
        voiceManager.noteOff_out(i).addMemberCallback(voices[i], &Electrophilia::Veronika::VeronikaVoice::handleNoteOff);
        contextOut.addMemberCallback(voices[i], &Electrophilia::Veronika::VeronikaVoice::setContext);
    }
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    contextOut.fire(Electrophilia::Dsp::Context(sampleRate));
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

    // This loop will iterate over all the midi events AND the audio frames after the last midi event
    // If there are no midi events, then it will just go through the audio frames in one go

    int midiEventCount = midiMessages.getNumEvents();
    auto iterator = midiMessages.begin();

    int currentSample = 0;
    for (int i_midi = 0; i_midi < midiEventCount + 1; i_midi++)
    {
        bool processMidi = i_midi < midiEventCount;

        int stopAtSample = numSamples;
        if (processMidi)
        {
            juce::MidiMessageMetadata metadata = *iterator;
            stopAtSample = metadata.samplePosition;
        }

        int samplesToProcess = stopAtSample - currentSample;
        //PROCESS AUDIO HERE

        for (int i = currentSample; i < stopAtSample; i++)
        {
            for (auto& voice : voices)
            {
                if (voice.isActive())
                {
                auto sample = voice.processSample().sum() / 128.0f;

                ch0[i] += sample;
                ch1[i] += sample;
                }
            }
        }

        currentSample = stopAtSample;

        if (processMidi)
        {
            juce::MidiMessageMetadata metadata = *iterator;
            auto currentMidiEvent = metadata.getMessage();

            Electrophilia::Control::Midi::Message message = Electrophilia::Control::Midi::Message::fromRawData(currentMidiEvent.getRawData(), currentMidiEvent.getRawDataSize());

            if (message.isNoteOnOrOff())
            {
                int noteNumber = message.data1;

                constexpr int lowestNote = Electrophilia::Math::C1_MIDI_NOTE_NUMBER;
                constexpr int highestNote = lowestNote + 61;

                if ((noteNumber >= lowestNote) && (noteNumber <= highestNote))
                {
                     // fire midi events here

                     if (message.isNoteOn())
                     {
                        voiceManager.handleNoteOn(Electrophilia::Control::Midi::MessageNoteOn(message));
                     }
                     if (message.isNoteOff())
                     {
                        voiceManager.handleNoteOff(Electrophilia::Control::Midi::MessageNoteOff(message));
                     }
                }
            }

            if (i_midi < (midiEventCount - 1)) iterator++;
        }

    }

}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);

}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

using namespace Electrophilia::Plugin::Tremolo;

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    for (int i = 0; i < PARAM_COUNT; ++i)
    {
        auto paramData = ParametersByID[i];

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
                juce::ParameterID {paramData.id, 0},
                paramData.name,
                juce::NormalisableRange<float>(paramData.min, paramData.max),
                paramData.def,
                paramData.name,
                juce::AudioProcessorParameter::genericParameter,
                [paramData](float value, int){ return juce::String(paramData.getStringFromValue(value)); }
                )
            );
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
