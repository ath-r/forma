#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "../PluginParameters.h"
#include "FrameComponent.h"
#include "SliderWithLabel.h"
#include "Tumbler.h"

namespace Ath::Forma 
{
    class PercussionComponent : public juce::Component
    {
        public:
        PercussionComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef)
        {
            addAndMakeVisible(frame);
            
            addAndMakeVisible(tumbler2);
            addAndMakeVisible(tumbler1);

            frame.label.setText("PERCUSSION", juce::NotificationType::sendNotification);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            frame.setBounds(area);

            auto centreX = getWidth()/2;
            auto centreY = getHeight()/2;

            area.reduce(20, 20);
            area.removeFromBottom(20);

            juce::Rectangle<int> tumblerArea = {0,0,128,128};
            tumblerArea.setCentre(centreX, centreY);

            tumbler1.setBounds({0,0,80,128});
            tumbler1.setTopRightPosition(centreX + 10, centreY-64);
            tumbler2.setBounds({0,0,80,128});
            tumbler2.setTopLeftPosition({tumbler1.getRight()-19, centreY-64});
        }

        private:
        juce::AudioProcessorValueTreeState& vts;

        FrameComponent frame;
        Gui::Tumbler tumbler1, tumbler2;
    };
}