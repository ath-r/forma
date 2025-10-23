#include "MainComponent.h"

namespace Ath::Forma 
{

    MainComponent::MainComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
        flutes(vts)
    {
        addAndMakeVisible(logo);
        addAndMakeVisible(flutes);
    }

    void MainComponent::paint (juce::Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
    
    void MainComponent::resized()
    {
        auto area = getLocalBounds();
        //area.removeFromBottom (20);
        auto area2 = area;

        area.removeFromBottom (area.getHeight() * 0.66f);
        logo.setBounds(area);

        //area2.reduce (20, 20);
        area2.removeFromTop (area2.getHeight() * 0.33f);

        flutes.setBounds(area2);
    }
}