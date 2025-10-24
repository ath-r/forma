#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterSlider : public juce::Slider
{
public:
    ParameterSlider (const juce::String& parameterId,
                     juce::AudioProcessorValueTreeState& stateToControl);

    void paint (juce::Graphics& g) override;

    juce::Colour lineColor = juce::Colours::black;
    juce::Colour thumbMainColor = juce::Colours::ivory;
    juce::Colour thumbTopColor = juce::Colours::lightgrey;
    juce::Colour thumbLeftColor = juce::Colours::grey;
    juce::Colour thumbBottomColor = juce::Colours::darkgrey;

    enum class ColorScheme
    {
        Ivory,
        Black,
        Blue,
        Red,
        Green,
        Yellow
    };

    void setColor(ColorScheme color)
    {
        switch (color)
        {
            case ColorScheme::Ivory:
            {
                lineColor = juce::Colours::black;
                thumbMainColor = juce::Colours::ivory;
                thumbTopColor = juce::Colours::lightgrey;
                thumbLeftColor = juce::Colours::grey;
                thumbBottomColor = juce::Colours::darkgrey;
                break;
            }
            case ColorScheme::Blue:
            {
                lineColor = juce::Colours::lightgrey;
                thumbMainColor = juce::Colour(0xff006ac8);
                thumbTopColor = juce::Colour(0xff005aba);
                thumbLeftColor = juce::Colour(0xff1c488a);
                thumbBottomColor = juce::Colour(0xff173460);
                break;
            }
        }
        repaint();
    }

private:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;
    //juce::Label label;

    juce::Path left;
    juce::Path right = left;
    juce::Path top;
    juce::Path bottom;
    juce::Path line;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};
