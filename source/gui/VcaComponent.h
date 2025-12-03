#pragma once

#include "Slider.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "../PluginParameters.h"
#include "ParameterListeningComponent.h"
#include "FrameComponent.h"
#include "LedComponent.h"
#include "SliderWithLabel.h"
#include "Tumbler.h"

namespace Ath::Forma 
{
    class VcaComponent : public Gui::ParameterListeningComponent
    {
        public:
        VcaComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
        time (ParametersByID[VCA_TIME].id, vts),
        vcaOn(ParametersByID[VCA_ON].id, vts),
        vcaSource(ParametersByID[VCA_SOURCE].id, vts),
        cresc(ParametersByID[VCA_CRESC].id, vts),
        led(ParametersByID[PERC_CV].id, vts)
        {
            addAndMakeVisible(frame);
            addAndMakeVisible(vcaOn);
            addAndMakeVisible(vcaSource);
            addAndMakeVisible (time);
            addAndMakeVisible (cresc);
            addAndMakeVisible(led);

            vts.addParameterListener(ParametersByID[VCA_SOURCE].id, this);
            value = vts.getParameter(ParametersByID[VCA_SOURCE].id)->getValue();
            triggerAsyncUpdate();
        }

        ~VcaComponent()
        {
            vts.removeParameterListener(ParametersByID[VCA_SOURCE].id, this);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            frame.setBounds(area);

            auto centreX = getWidth()/2;
            auto centreY = getHeight()/2;

            area.reduce(20, 20);
            area.removeFromBottom(20);

            auto sliderWidth = area.getWidth() / 4;

            vcaOn.setBounds(area.removeFromLeft (sliderWidth));
            vcaSource.setBounds(area.removeFromLeft (sliderWidth));
            time.setBounds (area.removeFromLeft (sliderWidth));
            cresc.setBounds(area.removeFromLeft (sliderWidth));

            auto ledArea = juce::Rectangle<int>(24, 24).withCentre(getLocalBounds().getBottomLeft().translated(30, -30));
            led.setBounds(ledArea);
        }

         void handleAsyncUpdate() override
        {
            frame.label.setText(value > 0.5f ? "PERCUSSION VCA" : "MASTER VCA", juce::dontSendNotification);
            frame.resized();
        }

        private:
        juce::AudioProcessorValueTreeState& vts;

        FrameComponent frame;
        Gui::LedComponent led;
        Gui::SliderWithLabel time;
        Gui::Tumbler vcaOn, vcaSource, cresc;
    };
}