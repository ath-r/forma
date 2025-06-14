#include "KnobWithLabel.h"
#include "clap/ext/gui.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"

#include "Fonts.h"
#include "Properties.h"
#include "juce_gui_basics/juce_gui_basics.h"
namespace Electrophilia::Gui
{
    KnobWithLabel::KnobWithLabel (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl)
        : knob(parameterId, stateToControl), ticks(knob)
    {
        addChildComponent(ticks);
        setStyle(style);

        addAndMakeVisible(knob);
        knob.setName("Knob");
        knob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

        addAndMakeVisible(label);
        label.setName("KnobLabel");
        label.setText(stateToControl.getParameter(parameterId)->name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centredBottom);
    }

    void KnobWithLabel::setStyle(KnobStyle _style)
    {
        style = _style;

        if (style == KnobStyle::withTicks)
        {
            addAndMakeVisible(ticks);
        }
        else
        {
            ticks.setVisible(false);
        }
    }

    void KnobWithLabel::lookAndFeelChanged()
    {
    }

    void KnobWithLabel::resized()
    {
        auto area = getLocalBounds();

        switch (style)
        {
            default:
            case KnobStyle::regular:
            {
                label.setBounds(area.removeFromTop(area.getHeight() / 5));
                knob.setBounds(area);
                break;
            }
            case KnobStyle::withTicks:
            {
                label.setBounds(area.removeFromTop(area.getHeight() / 6));
                area.removeFromTop(area.getHeight() / 12);
                knob.setBounds(area);
                break;
            }
            case KnobStyle::withoutTicksPadded:
            {
                area.removeFromTop(area.getHeight() / 12);
                label.setBounds(area.removeFromTop(area.getHeight() / 6));
                knob.setBounds(area);
                break;
            }
        }

        knob.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::TextBoxBelow,
            true,
            knob.getWidth(),
            knob.getHeight() / 5
            );

        ticks.setBounds(getLocalBounds());
    }
}
