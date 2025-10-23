#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "BinaryData.h"

namespace Ath::Forma 
{
    class LogoComponent : public juce::Component
    {
    public:
        void paint (juce::Graphics& g) override
        {
            auto logo =
            juce::Drawable::createFromImageData (BinaryData::logo_svg, BinaryData::logo_svgSize)->getOutlineAsPath();

            auto area = getLocalBounds().reduced(20, 20);
            logo.scaleToFit(area.getX(), 
                            area.getY(), 
                            area.getWidth(), 
                            area.getHeight(), 
                            true);
            
            g.setColour(juce::Colours::ivory);
            g.fillPath (logo);
        }
    };
}