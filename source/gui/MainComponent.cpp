#include "MainComponent.h"

namespace Ath::Forma 
{

    MainComponent::MainComponent (juce::AudioProcessorValueTreeState& vtsRef)
        : vts (vtsRef),
          flute16 (ParametersByID[F16].id, vts),
          flute8 (ParametersByID[F8].id, vts),
          flute5 (ParametersByID[F5].id, vts),
          flute4 (ParametersByID[F4].id, vts),
          flute2 (ParametersByID[F2].id, vts),
          flute1 (ParametersByID[F1].id, vts),
          tone (ParametersByID[TONE].id, vts)
    {
        addAndMakeVisible (flute16);
        addAndMakeVisible (flute8);
        addAndMakeVisible (flute5);
        addAndMakeVisible (flute4);
        addAndMakeVisible (flute2);
        addAndMakeVisible (flute1);
        addAndMakeVisible (tone);
    }

    void MainComponent::paint (juce::Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

        juce::Path p;
        p.addRoundedRectangle (getLocalBounds().reduced (10), 20);

        g.setColour (juce::Colours::ivory);
        g.strokePath (p, { 4, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded });
        //g.fillAll(juce::Colours::black);

        auto logo =
            juce::Drawable::createFromImageData (BinaryData::logo_svg, BinaryData::logo_svgSize)->getOutlineAsPath();

        const float centreX = getWidth() / 2;
        logo.applyTransform (juce::AffineTransform::scale (3.0f));
        logo.applyTransform (juce::AffineTransform::translation (-logo.getBounds().getWidth() / 2.0f, 0.0f));
        logo.applyTransform (juce::AffineTransform::translation (getWidth() / 2.0f, 40.0f));
        g.fillPath (logo);
    }
    
    void MainComponent::resized()
    {
        auto area = getLocalBounds();
        area.removeFromBottom (20);
        auto area2 = area;

        area.removeFromBottom (area.getHeight() * 0.66f);

        area2.reduce (20, 20);
        area2.removeFromTop (area2.getHeight() * 0.33f);

        auto sliderWidth = area2.getWidth() / 7;

        flute16.setBounds (area2.removeFromLeft (sliderWidth));
        flute8.setBounds (area2.removeFromLeft (sliderWidth));
        flute5.setBounds (area2.removeFromLeft (sliderWidth));
        flute4.setBounds (area2.removeFromLeft (sliderWidth));
        flute2.setBounds (area2.removeFromLeft (sliderWidth));
        flute1.setBounds (area2.removeFromLeft (sliderWidth));
        tone.setBounds (area2);
    }
}