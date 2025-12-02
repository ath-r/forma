#include "LedComponent.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include <algorithm>

namespace Ath::Gui 
{

    LedComponent::LedComponent (const juce::String& parameterId, juce::AudioProcessorValueTreeState& stateToControl)
    : vts(stateToControl)    
    {
        vts.addParameterListener(parameterId, this);
        parameter = vts.getParameter(parameterId);
    }

    LedComponent::~LedComponent() 
    {
        vts.removeParameterListener(parameter->getParameterID(), this);
    }

    void LedComponent::parameterChanged (const juce::String& parameterID, float newValue) 
    {
        value = newValue;

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

    void LedComponent::handleAsyncUpdate() 
    {
        repaint();
    }

    void LedComponent::resized() 
    {
        juce::Rectangle<float> localArea = getLocalBounds().toFloat();
        float size = std::min(localArea.getWidth(), localArea.getHeight());
        juce::Rectangle<float> ledArea = getLocalBounds().toFloat().withSizeKeepingCentre(size, size);

        juce::Rectangle<float> shineArea = ledArea.reduced(4);

        shine1.clear();
        shine1.addPieSegment(shineArea, juce::degreesToRadians(60.0f), juce::degreesToRadians(-60.0f), 0.5f);
        
        shine2.clear();
        shine2.addPieSegment(shineArea, juce::degreesToRadians(110.0f), juce::degreesToRadians(250.0f), 0.5f);
    }

    void LedComponent::paint (juce::Graphics& g) 
    {
        juce::Rectangle<float> localArea = getLocalBounds().toFloat();
        float size = std::min(localArea.getWidth(), localArea.getHeight());

        juce::Rectangle<float> ledArea = getLocalBounds().toFloat().withSizeKeepingCentre(size, size);

        g.setColour(juce::Colours::black);
        g.fillEllipse(ledArea);

        g.setColour(juce::Colour(0xff220a10).interpolatedWith(juce::Colour(0xfff36f5f), value));
        g.fillEllipse(ledArea.reduced(2));

        g.setColour(juce::Colour(0xff4e2223).interpolatedWith(juce::Colour(0xfff2796d), value));
        g.fillPath(shine1);
        g.fillPath(shine2);
    }
}