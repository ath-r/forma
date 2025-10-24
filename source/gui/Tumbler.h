#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <juce_audio_processors/juce_audio_processors.h>

namespace Ath::Gui
{
    class Tumbler : public juce::Component
    {
        bool switchedOn = true;

    public:

        Tumbler()
        {
        }

        void mouseDown(const juce::MouseEvent& event) override
        {
            auto buttonArea = getLocalBounds();
            buttonArea.reduce(3 + 4 * 2, 3);
            
            if (buttonArea.contains(event.getMouseDownPosition()))
            {
                switchedOn = !switchedOn;
                repaint();
            }
            
        }

        void paint (juce::Graphics& g) override
        {
            auto centreX = float(getWidth()) / 2.0f;
            auto centreY = float(getHeight()) / 2.0f;

            const float globalX = getTopLevelComponent()->getLocalPoint(this, juce::Point<int>{int(centreX), int(centreY)}).x;
            const float globalWidth = getTopLevelComponent()->getWidth();
            const float globalRatio = globalX / globalWidth;
            const float maxHorizontalOffset = 4.0f;
            const float horizontalOffset = (globalRatio * 2.0f - 1.0f) * maxHorizontalOffset;
            
            juce::Rectangle<float> buttonArea = {-centreX, -centreY, float(getWidth()), float(getHeight()) };
            buttonArea.reduce(3 + maxHorizontalOffset * 2, 3);

            auto lineArea = buttonArea.expanded(3, 3);

            juce::Point<float> corner1, corner2;
            if (switchedOn)
            {
                corner1 = buttonArea.getBottomLeft().translated(-1 + horizontalOffset, -8);
                corner2 = buttonArea.getBottomRight().translated(1 + horizontalOffset, -8);
            }
            else 
            {
                corner1 = buttonArea.getTopLeft().translated(-1 + horizontalOffset, 8);
                corner2 = buttonArea.getTopRight().translated(1 + horizontalOffset, 8);
            }

            juce::Path line;
            line.startNewSubPath(lineArea.getTopLeft());
            line.lineTo(lineArea.getTopRight());
            line.lineTo(corner2.translated(4, 0));
            line.lineTo(lineArea.getBottomRight());
            line.lineTo(lineArea.getBottomLeft());
            line.lineTo(corner1.translated(-4, 0));
            line.closeSubPath();  

            juce::Path top;
            top.startNewSubPath(buttonArea.getTopLeft());
            top.lineTo(buttonArea.getTopRight());
            top.lineTo(corner2);
            top.lineTo(corner1);
            top.closeSubPath();

            juce::Path bottom;
            bottom.startNewSubPath(buttonArea.getBottomLeft());
            bottom.lineTo(buttonArea.getBottomRight());
            bottom.lineTo(corner2);
            bottom.lineTo(corner1);
            bottom.closeSubPath();  

            g.setOrigin(centreX, centreY);

            g.setColour(juce::Colours::black);
            g.fillPath(line);

            g.setColour(juce::Colours::darkgrey);
            g.fillRect(buttonArea);
            
            g.setColour(switchedOn ? juce::Colours::lightgrey : juce::Colours::grey);
            g.fillPath(top);

            g.setColour(switchedOn ? juce::Colours::grey : juce::Colours::darkgrey);
            g.fillPath(bottom);
        }
    };
}