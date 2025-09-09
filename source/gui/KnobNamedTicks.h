#pragma once

#include "juce_core/juce_core.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace Ath::Gui
{
    class KnobNamedTicks : public juce::Component
    {
        juce::Slider & slider;

    public:

        KnobNamedTicks(juce::Slider & slider);
        void paint(juce::Graphics & g) override;

        struct Tick
        {
            float position = 0;
            juce::String text;
        };

        juce::Array<Tick> ticks;
    };
}
