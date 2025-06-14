#include "KnobNamedTicks.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_dsp/maths/juce_FastMathApproximations.h"

#include "Fonts.h"
#include <cmath>

namespace Electrophilia::Gui
{
    KnobNamedTicks::KnobNamedTicks(juce::Slider & slider) : slider(slider)
    {

    }

    void KnobNamedTicks::paint(juce::Graphics & g)
    {
        const auto bounds = getLookAndFeel().getSliderLayout(slider).sliderBounds.translated(slider.getX(), slider.getY());

        const auto radius = juce::jmin(bounds.getHeight(), bounds.getWidth()) / 2.0f;
        const auto lineWidth = radius * 0.05;
        const auto arcRadius = radius - lineWidth;

        g.setOrigin(bounds.getCentre());

        juce::Path p;

        auto startAngle = fmod(slider.getRotaryParameters().startAngleRadians, juce::MathConstants<float>::twoPi);
        if (startAngle > juce::MathConstants<float>::pi) startAngle -= juce::MathConstants<float>::twoPi;

        auto endAngle = fmod(slider.getRotaryParameters().endAngleRadians, juce::MathConstants<float>::twoPi);
        if (endAngle > juce::MathConstants<float>::pi) endAngle -= juce::MathConstants<float>::twoPi;

        const auto textHeight = slider.getTextBoxHeight() * 0.5f;
            g.setFont(juce::Font(getTypeface(fontsIds::standardReadoutFontId))
                .withHeight(textHeight));

        for (auto tick : ticks)
        {
            const auto pos = tick.position;
            const auto text = tick.text;

            const auto toAngle = startAngle + (endAngle - startAngle) * pos;

            const auto tickStart = juce::Point<float>(0.0,
                                                                        -arcRadius).rotatedAboutOrigin(toAngle);

            const auto tickEnd = juce::Point<float>(0.0,
                                                                        -radius).rotatedAboutOrigin(toAngle);

            const auto textWidth = juce::GlyphArrangement().getStringWidth(g.getCurrentFont(), text);

            auto textArea = juce::Rectangle<float>(0.0f,
                                                                        0.0f,
                                                                        textWidth,
                                                                        textHeight
                                                                        );

            const auto textPosition = juce::Point<float>(0.0,
                                                                            -radius - lineWidth).
                                                                                rotatedAboutOrigin(toAngle);

            textArea.setCentre(textPosition);

            auto area1 = juce::Rectangle<float>(textPosition, juce::Point<float>());

            const auto textDelta = juce::Point<float>(0.0f, -1.0f).rotatedAboutOrigin(toAngle);

            while( textArea.intersects(area1))
            {
                textArea.translate(textDelta.x, textDelta.y);
            }

            g.drawText(text, textArea, juce::Justification::centred);

            p.addLineSegment(juce::Line<float>(tickStart, tickEnd),
                             0.1f);
        }



        g.strokePath(p, juce::PathStrokeType(lineWidth,
                                                              juce::PathStrokeType::JointStyle::mitered,
                                                              juce::PathStrokeType::EndCapStyle::butt));

    }
}
