#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <juce_audio_processors/juce_audio_processors.h>

namespace Ath::Gui
{
    class ParameterListeningComponent : public juce::Component, public juce::AudioProcessorValueTreeState::Listener, public juce::AsyncUpdater
    {
    protected:
        float value = 0;

    public:
        void parameterChanged (const juce::String& parameterID, float newValue) override
        {
            value = newValue;

            if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            {
                cancelPendingUpdate();
                handleAsyncUpdate();
            }
            else
            {
                triggerAsyncUpdate();
            }
        }
    };
}