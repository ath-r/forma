#include "MainComponent.h"
#include "PercussionComponent.h"

namespace Ath::Forma 
{

    MainComponent::MainComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
        flutes(vts),
        vca(vts),
        percussion(vts)        
    {
        addAndMakeVisible(logo);
        addAndMakeVisible(flutes);
        addAndMakeVisible(vca);
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

        flutes.setBounds(area.removeFromLeft(getWidth() * (7.0f / 17.0f)));
        vca.setBounds(area.removeFromLeft(getWidth() * (4.0f / 17.0f)));
        percussion.setBounds(area);
    }
}