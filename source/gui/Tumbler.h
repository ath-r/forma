#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <juce_audio_processors/juce_audio_processors.h>

namespace Ath::Gui
{
    class Tumbler : public juce::Component, juce::AudioProcessorValueTreeState::Listener, juce::AsyncUpdater
    {
        juce::AudioProcessorValueTreeState& vts;
        bool switchedOn = true;

        juce::String paramId;
        juce::RangedAudioParameter* parameter;

        juce::Path line, top, bottom;
        juce::Rectangle<float> switchArea;

    public:

        juce::Label topLabel, bottomLabel;

        Tumbler (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl);

        ~Tumbler();

        void parameterChanged (const juce::String& parameterID, float newValue) override;

        void mouseDown (const juce::MouseEvent& event) override;

        void handleAsyncUpdate() override;

        void resized() override;

        void paint (juce::Graphics& g) override;
    };
}