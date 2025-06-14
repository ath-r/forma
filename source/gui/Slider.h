#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterSlider : public juce::Slider
{
public:
    ParameterSlider (const juce::String& parameterId,
                     juce::AudioProcessorValueTreeState& stateToControl);

private:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;
    //juce::Label label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};
