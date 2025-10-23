#pragma once

#include "../PluginParameters.h"
#include "FrameComponent.h"
#include "SliderWithLabel.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace Ath::Forma 
{
    class PercussionComponent : public juce::Component
    {
        public:
        PercussionComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef)
        {
            addAndMakeVisible(frame);

            frame.label.setText("PERCUSSION", juce::NotificationType::sendNotification);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            frame.setBounds(area);

            area.reduce(20, 20);
            area.removeFromBottom(20);
        }

        private:
        juce::AudioProcessorValueTreeState& vts;

        FrameComponent frame;
    };
}