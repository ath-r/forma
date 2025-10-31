#include "Tumbler.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"

namespace Ath::Gui
{
    Tumbler::Tumbler (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl)
        : vts (stateToControl),
        paramId(parameterId)
    {
        vts.addParameterListener(parameterId, this);
        parameter = vts.getParameter(parameterId);

        addAndMakeVisible(topLabel);
        topLabel.setEditable(false);
        topLabel.setJustificationType(juce::Justification::centredBottom);
        topLabel.setText(parameter->getText(1, 10), juce::dontSendNotification);

        addAndMakeVisible(bottomLabel);
        bottomLabel.setEditable(false);
        bottomLabel.setJustificationType(juce::Justification::centredBottom);
        bottomLabel.setText(parameter->getText(0, 10), juce::dontSendNotification);

        switchedOn = vts.getParameter(paramId)->getValue();
    }

    Tumbler::~Tumbler() 
    {
        vts.removeParameterListener(paramId, this);
    }

    void Tumbler::mouseDown (const juce::MouseEvent& event)
    {
        auto buttonArea = getLocalBounds();
        buttonArea.reduce (3 + 4 * 2, 3);

        if (switchArea.contains (event.getMouseDownPosition().toFloat()))
        {
            switchedOn = !switchedOn;
            repaint();
        }

        if (parameter != nullptr)
        {
            parameter->setValueNotifyingHost(float(switchedOn));
        }
    }

    void Tumbler::parameterChanged (const juce::String& parameterID, float newValue) 
    {
        switchedOn = newValue >= 0.5f;

        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    };

    void Tumbler::handleAsyncUpdate() 
    {
        repaint();
    }

    void Tumbler::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();

        topLabel.setBounds(area.removeFromTop(40));
        bottomLabel.setBounds(area.removeFromBottom(40));

        switchArea = area.toFloat();
    }

    void Tumbler::paint (juce::Graphics& g)
    {
        auto centreX = float (getWidth()) / 2.0f;
        auto centreY = float (getHeight()) / 2.0f;

        const float globalX = getTopLevelComponent()->getLocalPoint (this, juce::Point<int> { int (centreX), int (centreY) }).x;
        const float globalWidth = getTopLevelComponent()->getWidth();
        const float globalRatio = globalX / globalWidth;
        const float maxHorizontalOffset = 4.0f;
        const float horizontalOffset = (globalRatio * 2.0f - 1.0f) * maxHorizontalOffset;

        juce::Rectangle<float> RockerArea = switchArea.withWidth(80).withCentre({0,0});
        RockerArea.reduce (3 + maxHorizontalOffset * 2, 3);

        auto lineArea = RockerArea.expanded (3, 3);

        juce::Point<float> corner1, corner2;
        if (switchedOn)
        {
            corner1 = RockerArea.getBottomLeft().translated (-1 + horizontalOffset, -8);
            corner2 = RockerArea.getBottomRight().translated (1 + horizontalOffset, -8);
        }
        else
        {
            corner1 = RockerArea.getTopLeft().translated (-1 + horizontalOffset, 8);
            corner2 = RockerArea.getTopRight().translated (1 + horizontalOffset, 8);
        }

        line.clear();
        line.startNewSubPath (lineArea.getTopLeft());
        line.lineTo (lineArea.getTopRight());
        line.lineTo (corner2.translated (4, 0));
        line.lineTo (lineArea.getBottomRight());
        line.lineTo (lineArea.getBottomLeft());
        line.lineTo (corner1.translated (-4, 0));
        line.closeSubPath();

        top.clear();
        top.startNewSubPath (RockerArea.getTopLeft());
        top.lineTo (RockerArea.getTopRight());
        top.lineTo (corner2);
        top.lineTo (corner1);
        top.closeSubPath();

        bottom.clear();
        bottom.startNewSubPath (RockerArea.getBottomLeft());
        bottom.lineTo (RockerArea.getBottomRight());
        bottom.lineTo (corner2);
        bottom.lineTo (corner1);
        bottom.closeSubPath();

        g.setOrigin (centreX, centreY);

        g.setColour (juce::Colours::black);
        g.fillRect (lineArea);
        g.fillPath (line);

        g.setColour (juce::Colours::darkgrey);
        g.fillRect (RockerArea);

        g.setColour (switchedOn ? juce::Colours::lightgrey : juce::Colours::grey);
        g.fillPath (top);

        g.setColour (switchedOn ? juce::Colours::grey : juce::Colours::darkgrey);
        g.fillPath (bottom);
    }
}