#pragma once

#include "../PluginPerformance.h"

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

namespace Ath::Forma 
{
    class PerformanceMeterComponent : public juce::AnimatedAppComponent
    {
    public:
        PerformanceData& pdata;
        juce::Label label;

        PerformanceMeterComponent(PerformanceData& p)
        : pdata(p)
        {
            setSynchroniseToVBlank(true);
            addAndMakeVisible(label);
        }

        void update() override 
        {
            juce::String str;

            str += juce::String(pdata.SamplesPerBlock.read());
            str += " samples / ";
            str += juce::String(pdata.SampleRate.read());
            str += " hz; ";
            str += juce::String(pdata.ExecutionTimeNs.read(), 2, true);
            str += " ns";

            label.setText(str, juce::dontSendNotification);
            repaint();
        }

        void resized() override
        {
            label.setBounds(getLocalBounds());
        }

        void paint(juce::Graphics& g) override
        {

        }
    };
}