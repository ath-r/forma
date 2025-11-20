#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace Ath::Gui
{
    class ChoiceComponent : public juce::Component, juce::AudioProcessorValueTreeState::Listener, juce::AsyncUpdater
    {
        juce::AudioProcessorValueTreeState& vts;
        juce::AudioParameterChoice* parameter;

        juce::OwnedArray<juce::Label> labels;

        juce::StringArray choices;

        int currentChoiceIndex = 0;

    public:
        ChoiceComponent(const juce::String& parameterID, juce::AudioProcessorValueTreeState& stateToControl);

        ~ChoiceComponent();

        void parameterChanged(const juce::String& parameterID, float newValue) override;

        void handleAsyncUpdate() override;

        void paint(juce::Graphics& graphics) override;

        void resized() override;

        void mouseDown(const juce::MouseEvent& event) override;

        void mouseDrag(const juce::MouseEvent& event) override;
    };
}