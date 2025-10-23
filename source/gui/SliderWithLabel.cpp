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
        slider.setName("Slider");
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

        addAndMakeVisible(label);
        label.setName("SliderLabel");
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

        label.setBounds(area.removeFromBottom(40));
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
    void SliderWithLabel::paint (juce::Graphics& g) 
    {
        auto bounds = slider.getBoundsInParent().toFloat();
        auto centreX = bounds.getCentreX();

        g.setColour(juce::Colours::black);
        g.drawLine(centreX, bounds.getY(), centreX, bounds.getBottom(), 8);

        const float ticksFrom = slider.getPositionOfValue(slider.getMaximum());// + 16.0f * juce::dsp::FastMathApproximations::sin(maxAngle);
        const float ticksTo = slider.getPositionOfValue(slider.getMinimum());// - 16.0f * juce::dsp::FastMathApproximations::sin(maxAngle);
        const float ticksSpan = std::abs(ticksTo - ticksFrom);

        g.setColour(juce::Colours::darkgrey);
        for(int i = 0; i < 10; i++)
        {
            const auto y = ticksFrom + ticksSpan / 9.0f * i;
            //if (y > thumbY) g.setColour(juce::Colours::lightgrey);
            g.drawLine(centreX + 8, y, centreX + 24, y, 2);
            g.drawLine(centreX - 8, y, centreX - 24, y, 2);
        }
    }
}
