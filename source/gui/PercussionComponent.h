#pragma once

#include "Slider.h"
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
        : vts (vtsRef),
        perc16 (ParametersByID[P16].id, vts),
        perc8 (ParametersByID[P8].id, vts),
        perc5 (ParametersByID[P5].id, vts),
        perc4 (ParametersByID[P4].id, vts),
        perc2 (ParametersByID[P2].id, vts),
        perc1 (ParametersByID[P1].id, vts),
        time (ParametersByID[TIME].id, vts)
        {
            addAndMakeVisible(frame);
            addAndMakeVisible (perc16);
            addAndMakeVisible (perc8);
            addAndMakeVisible (perc5);
            addAndMakeVisible (perc4);
            addAndMakeVisible (perc2);
            addAndMakeVisible (perc1);
            addAndMakeVisible (time);
            //addAndMakeVisible(tumblerPoly);

            perc16.slider.setColor(ParameterSlider::ColorScheme::Blue);
            perc8.slider.setColor(ParameterSlider::ColorScheme::Blue);
            perc5.slider.setColor(ParameterSlider::ColorScheme::Blue);
            perc4.slider.setColor(ParameterSlider::ColorScheme::Blue);
            perc2.slider.setColor(ParameterSlider::ColorScheme::Blue);
            perc1.slider.setColor(ParameterSlider::ColorScheme::Blue);

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

            auto sliderWidth = area.getWidth() / 7;

            perc16.setBounds (area.removeFromLeft (sliderWidth));
            perc8.setBounds (area.removeFromLeft (sliderWidth));
            perc5.setBounds (area.removeFromLeft (sliderWidth));
            perc4.setBounds (area.removeFromLeft (sliderWidth));
            perc2.setBounds (area.removeFromLeft (sliderWidth));
            perc1.setBounds (area.removeFromLeft (sliderWidth));
            time.setBounds (area.removeFromLeft (sliderWidth));

            juce::Rectangle<int> tumblerArea = {0,0,128,128};
            tumblerArea.setCentre(centreX, centreY);

            tumblerPoly.setBounds({0,0,80,128});
            tumblerPoly.setTopRightPosition(centreX + 10, centreY-64);
        }

        private:
        juce::AudioProcessorValueTreeState& vts;

        FrameComponent frame;
        Gui::SliderWithLabel perc16, perc8, perc5, perc4, perc2, perc1, time;
        Gui::Tumbler tumblerPoly;
    };
}