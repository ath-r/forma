
#include "Slider.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_dsp/maths/juce_FastMathApproximations.h"

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

    left.preallocateSpace(16);
    right.preallocateSpace(16);
    top.preallocateSpace(16);
    bottom.preallocateSpace(16);
    line.preallocateSpace(16);
}

void ParameterSlider::paint (juce::Graphics& g) 
{
    getValue();
    
    auto bounds = getLocalBounds().toFloat();
    auto cur = getPositionOfValue(getValue());

    const auto centreX = bounds.getCentreX();
    const auto centreY = bounds.getCentreY();

    const auto thumbY = getPositionOfValue(getValue());

    g.setColour(juce::Colours::darkgrey);
    g.drawLine(centreX, thumbY, centreX, getPositionOfValue(getMinimum()), 2);

    const float ticksFrom = getPositionOfValue(getMaximum());
    const float ticksTo = getPositionOfValue(getMinimum());
    const float ticksSpan = std::abs(ticksTo - ticksFrom);

    g.setColour(juce::Colours::lightgrey);
    for(int i = 0; i < 10; i++)
        {
            const auto y = ticksFrom + ticksSpan / 9.0f * i;
            if (y > thumbY)
            {
                g.drawLine(centreX + 8, y, centreX + 24, y, 2);
                g.drawLine(centreX - 8, y, centreX - 24, y, 2);
            }            
        }

    const float positionRatio = 1.0f - (getValue() - getMinimum()) / (getMaximum() - getMinimum());
    const float maxAngle = juce::degreesToRadians(20.0);

    const float positionAngle = (positionRatio - 0.5f) * 2.0f * maxAngle;
    const float positionSin = -juce::dsp::FastMathApproximations::sin(positionAngle);
    const float positionCos = juce::dsp::FastMathApproximations::cos(positionAngle);

    const float sinY = positionSin * 16.0f;

    juce::Rectangle<float> thumbBase(-32, -16 * positionCos + sinY, 64, 32 * positionCos);
    juce::Rectangle<float> thumbTop(-28, -8 * positionCos, 56, 16 * positionCos);

    const float reduction = (1 - positionCos) * 0.2f;

    thumbBase.reduce(thumbBase.getWidth() * reduction, thumbBase.getHeight() * reduction);
    thumbTop.reduce(thumbTop.getWidth() * reduction, thumbTop.getHeight() * reduction);

    g.setOrigin(centreX, thumbY);
    left.clear();
    left.startNewSubPath(thumbBase.getTopLeft());
    left.lineTo(thumbBase.getBottomLeft());
    left.lineTo(thumbTop.getBottomLeft());
    left.lineTo(thumbTop.getTopLeft());
    left.lineTo(thumbBase.getTopLeft());

    right = left;    
    right.applyTransform(juce::AffineTransform::scale(-1.0, 1.0));

    top.clear();
    top.startNewSubPath(thumbBase.getTopLeft());
    top.lineTo(thumbBase.getTopRight());
    top.lineTo(thumbTop.getTopRight());
    top.lineTo(thumbTop.getTopLeft());
    top.lineTo(thumbBase.getTopLeft());

    bottom.clear();
    bottom.startNewSubPath(thumbBase.getBottomLeft());
    bottom.lineTo(thumbBase.getBottomRight());
    bottom.lineTo(thumbTop.getBottomRight());
    bottom.lineTo(thumbTop.getBottomLeft());
    bottom.lineTo(thumbBase.getBottomLeft());

    line.clear();
    line.startNewSubPath(-32, sinY);
    line.lineTo(-28, 0);
    line.lineTo(28, 0);
    line.lineTo(32, sinY);

    thumbBase.expand(3, 3);
    g.setColour(juce::Colours::black);
    g.fillRect(thumbBase);

    g.setColour(juce::Colours::grey);
    g.fillPath(left);

    g.setColour(juce::Colours::lightgrey);
    g.fillPath(top);

    g.setColour(juce::Colours::darkgrey);
    g.fillPath(right);
    g.fillPath(bottom);

    g.setColour(juce::Colours::ivory);
    g.fillRect(thumbTop);

    g.setColour(juce::Colours::black);
    g.strokePath(line, {4, juce::PathStrokeType::JointStyle::beveled, juce::PathStrokeType::EndCapStyle::rounded});
}
