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
    class FlutesComponent : public juce::Component
    {
        public:
        FlutesComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
          flute16 (ParametersByID[F16].id, vts),
          flute8 (ParametersByID[F8].id, vts),
          flute5 (ParametersByID[F5].id, vts),
          flute4 (ParametersByID[F4].id, vts),
          flute2 (ParametersByID[F2].id, vts),
          flute1 (ParametersByID[F1].id, vts),
          tone (ParametersByID[TONE].id, vts)
        {
            addAndMakeVisible(frame);
            addAndMakeVisible (flute16);
            addAndMakeVisible (flute8);
            addAndMakeVisible (flute5);
            addAndMakeVisible (flute4);
            addAndMakeVisible (flute2);
            addAndMakeVisible (flute1);
            addAndMakeVisible (tone);

            frame.label.setText("FLUTES", juce::NotificationType::sendNotification);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            frame.setBounds(area);

            area.reduce(20, 20);
            area.removeFromBottom(20);

            auto sliderWidth = area.getWidth() / 7;

            flute16.setBounds (area.removeFromLeft (sliderWidth));
            flute8.setBounds (area.removeFromLeft (sliderWidth));
            flute5.setBounds (area.removeFromLeft (sliderWidth));
            flute4.setBounds (area.removeFromLeft (sliderWidth));
            flute2.setBounds (area.removeFromLeft (sliderWidth));
            flute1.setBounds (area.removeFromLeft (sliderWidth));
            tone.setBounds (area.removeFromLeft (sliderWidth));
        }

        private:
        juce::AudioProcessorValueTreeState& vts;

        FrameComponent frame;
        Gui::SliderWithLabel flute16, flute8, flute5, flute4, flute2, flute1, tone;
    };
}