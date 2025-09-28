#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace Ath::Gui
{
    class LookAndFeel : public juce::LookAndFeel_V4
    {
    public:

        LookAndFeel();

        void drawRotarySlider (juce::Graphics & graphics,
                            int x,
                            int y,
                            int width,
                            int height,
                            float sliderPosProportional,
                            float rotaryStartAngle,
                            float rotaryEndAngle,
                            juce::Slider & slider
                            ) override;

        juce::Font getLabelFont(juce::Label & label) override;

    private:

        void drawRotarySliderMeter (juce::Graphics& g,
                                    juce::Rectangle<float> bounds,
                                    float lineWidth,
                                    float arcRadius,
                                    float rotaryStartAngle,
                                    float rotaryEndAngle,
                                    float toAngle,
                                    juce::Slider& slider
                                    );

        void drawRotarySliderBase (juce::Graphics& g,
                                   float radius,
                                   float toAngle,
                                   juce::Rectangle<float> bounds,
                                   juce::Slider&
                                   );
    };
}
