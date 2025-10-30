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
        juce::String str;
        bool visible = true;

        PerformanceMeterComponent(PerformanceData& p)
        : pdata(p)
        {
            setFramesPerSecond(10);
        }

        void update() override 
        {
            str.clear();
            str += juce::String(pdata.SamplesPerBlock.read());
            str += " samples / ";
            str += juce::String(pdata.SampleRate.read());
            str += " hz;\n";
            str += juce::String(pdata.ExecutionTimeNs.read(), 2, true);
            str += " nS over 100 measurements\n";
            str += "max ";
            str += juce::String(pdata.MaxExecutionTimeNs.read(), 2, true);
            str += " nS over 1000 measurements\n";
            str += "min ";
            str += juce::String(pdata.MinExecutionTimeNs.read(), 2, true);
            str += " nS over 1000 measurements";
            repaint();
        }

        void mouseDown(const juce::MouseEvent& event) override
        {
            if (event.mods.isLeftButtonDown())
            {
                auto time = pdata.ExecutionTimeNs.read();
                pdata.MaxExecutionTimeNs.write(time);
                pdata.MinExecutionTimeNs.write(time);
            }

            if (event.mods.isRightButtonDown())
            {
                visible = !visible;
            }
        }

        void resized() override
        {
        }

        void paint(juce::Graphics& g) override
        {
            if (visible)
            {
                g.setColour(juce::Colours::lightgrey);
                g.setFont(g.getCurrentFont().withHeight(getHeight() / 4));
                g.drawFittedText(str, getLocalBounds(), juce::Justification::topLeft, 2);
            }
        }
    };
}