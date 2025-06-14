
#include "Slider.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

ParameterSlider::ParameterSlider (const juce::String& parameterId,
                                  juce::AudioProcessorValueTreeState& stateToControl)
    : sliderValue (std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (stateToControl,
                                                                                           parameterId,
                                                                                           *this))
{
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    //setTextValueSuffix (stateToControl.getParameter (parameterId)->label);
    setTextBoxStyle (Slider::TextEntryBoxPosition::TextBoxBelow,
        true,
        getTextBoxWidth(),
        getTextBoxHeight()
        );
}
