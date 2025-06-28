#pragma once

#include "control/Events.h"
#include "dsp/Context.h"
#include "dsp/PhaseCounter.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include <array>
#include <juce_audio_processors/juce_audio_processors.h>

#if (MSVC)
#include "ipps.h"
#endif

#include "control/VoiceManager.h"
#include "VeronikaVoice.h"
#include "VeronikaTimbreProcessor.h"

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

    Electrophilia::Dsp::PhaseCounter phaseCounter;

    Electrophilia::Control::VoiceManager voiceManager;
    std::array<Electrophilia::Veronika::VeronikaVoice, 16> voices;

    Electrophilia::Veronika::TimbreProcessor timbreProcessor;

    Electrophilia::Control::EventOutput<Electrophilia::Dsp::Context> contextOut;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
