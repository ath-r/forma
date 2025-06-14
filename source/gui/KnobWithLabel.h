#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Slider.h"
#include "KnobNamedTicks.h"

namespace Electrophilia::Gui
{
    class KnobWithLabel : public juce::Component
    {
    public:

        enum class KnobStyle
        {
            regular,
            withTicks,
            withoutTicksPadded
        };

        KnobWithLabel (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl);

        void setStyle(KnobStyle _style);

        void resized() override;

        void lookAndFeelChanged() override;

        ParameterSlider knob;
        KnobNamedTicks ticks;
        juce::Label label;

        private:

        KnobStyle style = KnobStyle::regular;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobWithLabel)
    };
}
