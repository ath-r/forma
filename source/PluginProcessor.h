#pragma once

#include "PluginParameters.h"
#include "ParameterObserver.h"

#include "PluginPerformance.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include <array>
#include <juce_audio_processors/juce_audio_processors.h>

#include "FormaSynth.h"
#include "juce_core/juce_core.h"

#if (MSVC)
#include "ipps.h"
#endif

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;
    juce::XmlElement pluginInstanceSettings;

    Ath::Forma::PerformanceData performance;

private:

    std::array<Ath::Control::Midi::MessageMeta, 10000> midiEvents;

    Ath::Forma::FormaSynth formaSynth;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::array<ParameterObserver, Ath::Forma::PARAM_COUNT> parameterObservers;

    void setParameter(Ath::Control::Parameter p, Ath::Forma::FormaSynth::ParameterValueData parameterValueData)
    {
        if (!parameterValueData.touched) return;
        auto value = parameterValueData.value;

        auto param = treeState.getParameter(p.id);
        auto valueToCompare = param->getValue();

        auto diff = std::abs(value - valueToCompare);
        auto diffth = 1.0 / 256.0f;

        if (diff > diffth && parameterValueData.touched)
        {
            param->setValueNotifyingHost(value);
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
