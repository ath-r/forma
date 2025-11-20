#pragma once

#include "ChoiceComponent.h"
#include "juce_graphics/juce_graphics.h"

namespace Ath::Gui
{
    ChoiceComponent::ChoiceComponent(const juce::String& parameterId, juce::AudioProcessorValueTreeState& vts)
        : vts(vts)
    {
        vts.addParameterListener(parameterId, this);
        parameter = dynamic_cast<juce::AudioParameterChoice*>(vts.getParameter(parameterId));
        currentChoiceIndex = parameter->getIndex();

        choices = parameter->choices;
    }

    ChoiceComponent::~ChoiceComponent()
    {
        vts.removeParameterListener(parameter->paramID, this);
    }

    void ChoiceComponent::parameterChanged(const juce::String& parameterID, float newValue)
    {
        currentChoiceIndex = int(newValue);

        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void ChoiceComponent::handleAsyncUpdate()
    {
        repaint();
    }

    void ChoiceComponent::paint(juce::Graphics & g)
    {
        //graphics.setColour(juce::Colours::black);
        //graphics.fillAll();

        auto area = getLocalBounds().withTrimmedBottom(40).withTrimmedTop(40);
        auto entrySize = area.getHeight() / parameter->choices.size();

        g.setColour(juce::Colours::black);
        g.fillRect(area);

        auto highlightArea = area;
        highlightArea.removeFromTop(entrySize * currentChoiceIndex);
        highlightArea.removeFromBottom(entrySize * (parameter->choices.size() - currentChoiceIndex - 1));

        g.setColour(juce::Colours::lightgrey);
        g.fillRect(highlightArea);

        for (int i = 0; i < choices.size(); i++)
        {   
            if (i == currentChoiceIndex) g.setColour(juce::Colours::black);
            else g.setColour(juce::Colours::ivory);
            juce::Rectangle<int> textArea = {0, 40 + entrySize * i, area.getWidth(), entrySize};
            g.drawText(choices[i], textArea, juce::Justification::centred);
        }
    }

    void ChoiceComponent::resized()
    {
        auto area = getLocalBounds().withTrimmedBottom(40).withTrimmedTop(40);
        auto entrySize = area.getHeight() / parameter->choices.size();

        for (auto& label : labels)
        {
            label->setBounds(area.removeFromTop(entrySize));
        }
    }

    void ChoiceComponent::mouseDown(const juce::MouseEvent& event)
    {
        auto pos = getMouseXYRelative();

        int newIndex = pos.y / (getHeight() / parameter->choices.size());

        if (newIndex != currentChoiceIndex)
        {
            parameter->setValueNotifyingHost(parameter->convertTo0to1(newIndex));
        }
    }

    void ChoiceComponent::mouseDrag(const juce::MouseEvent& event)
    {
        mouseDown(event);
    }
}