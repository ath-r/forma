#include "FrameComponent.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace Ath::Forma 
{

    FrameComponent::FrameComponent() 
    {
        addAndMakeVisible(label);
        label.setEditable(false);
        label.setJustificationType(juce::Justification::centredBottom);
    }
    
    void FrameComponent::paint (juce::Graphics& g)
    {
        p.clear();
        p.addRoundedRectangle (getLocalBounds().reduced (10), 20);

        g.setColour (juce::Colours::ivory);
        g.strokePath (p, { 4, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded });

        g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.fillRect(label.getBounds());
    }

    void FrameComponent::resized() 
    {
        auto area = getLocalBounds().removeFromBottom(40);
        auto width = label.getFont().getStringWidth(label.getText());
        
        area.setWidth(width * 2);
        area.setCentre(getWidth() / 2, getHeight() - 20);
         
        label.setBounds(area);
    }
}