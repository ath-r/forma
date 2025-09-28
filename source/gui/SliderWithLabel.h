#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Slider.h"
#include "juce_graphics/juce_graphics.h"

namespace Ath::Gui
{
    class SliderWithLabel : public juce::Component
    {
    public:

        enum class SliderStyle
        {
            regular,
            withTicks
        };

        SliderWithLabel (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl);

        void setStyle(SliderStyle _style);

        void resized() override;

        void lookAndFeelChanged() override;

        void paint (juce::Graphics& g) override;

        ParameterSlider slider;
        juce::Label label;

        private:

        SliderStyle style = SliderStyle::regular;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderWithLabel)
    };
}
