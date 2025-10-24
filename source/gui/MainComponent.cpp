#include "MainComponent.h"
#include "PercussionComponent.h"

namespace Ath::Forma 
{

    MainComponent::MainComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
        flutes(vts),
        percussion(vts)
    {
        addAndMakeVisible(logo);
        addAndMakeVisible(flutes);
        addAndMakeVisible(percussion);
    }

    void MainComponent::paint (juce::Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void MainComponent::resized()
    {
        auto area = getLocalBounds();

        logo.setBounds(area.removeFromTop (area.getHeight() * 0.333f));

        flutes.setBounds(area.removeFromLeft(area.getWidth() * (7.0f / 14.0f)));
        percussion.setBounds(area);
    }
}