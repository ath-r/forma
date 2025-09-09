#pragma once

#include "PluginParameters.h"
#include "ParameterObserver.h"

#include "juce_audio_basics/juce_audio_basics.h"
#include <array>
#include <juce_audio_processors/juce_audio_processors.h>

#include "FormaSynth.h"

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

private:

    std::array<Ath::Control::Midi::MessageMeta, 10000> midiEvents;

    Ath::Forma::FormaSynth formaSynth;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::array<ParameterObserver, Ath::Forma::PARAM_COUNT> parameterObservers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
