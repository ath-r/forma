#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <juce_audio_processors/juce_audio_processors.h>

namespace Ath::Gui
{
    class LedComponent : public juce::Component, juce::AudioProcessorValueTreeState::Listener, juce::AsyncUpdater
    {
        juce::AudioProcessorValueTreeState& vts;
        juce::RangedAudioParameter* parameter;
        float value = 0.0f;

        juce::Path shine1, shine2;

    public:
        LedComponent (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl);

        ~LedComponent();

        void parameterChanged (const juce::String& parameterID, float newValue) override;

        void handleAsyncUpdate() override;

        void resized() override;

        void paint (juce::Graphics& g) override;
    };
}