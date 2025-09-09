#include "SliderWithLabel.h"
#include "KnobWithLabel.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"

#include "Fonts.h"
#include "Properties.h"
#include "juce_gui_basics/juce_gui_basics.h"
namespace Ath::Gui
{
    SliderWithLabel::SliderWithLabel (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl)
        : slider(parameterId, stateToControl)
    {
        setStyle(style);

        addAndMakeVisible(slider);
        slider.setName("Knob");
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

        addAndMakeVisible(label);
        label.setName("KnobLabel");
        label.setText(stateToControl.getParameter(parameterId)->name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centredBottom);
    }

    void SliderWithLabel::setStyle(SliderStyle _style)
    {
        style = _style;

        if (style == SliderStyle::withTicks)
        {
            //
        }
        else
        {
            //
        }
    }

    void SliderWithLabel::lookAndFeelChanged()
    {
    }

    void SliderWithLabel::resized()
    {
        auto area = getLocalBounds();

        label.setBounds(area.removeFromBottom(area.getHeight() / 6));
        slider.setBounds(area);

        slider.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox,
            true,
            slider.getWidth(),
            0
            );

/*
        slider.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::TextBoxBelow,
            true,
            slider.getWidth(),
            slider.getHeight() / 5
            );
*/
    }
}
