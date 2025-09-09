#include "LookAndFeel.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_dsp/maths/juce_FastMathApproximations.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "Fonts.h"
#include "Properties.h"
#include <cstdlib>

namespace Ath::Gui
{
    LookAndFeel::LookAndFeel()
    {
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1e232b));

        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff282d41));
        setColour(juce::TextButton::textColourOffId, juce::Colours::ivory);

        setColour(juce::Label::textColourId, juce::Colours::ivory);

        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::ivory);
        setColour(juce::Slider::backgroundColourId, juce::Colours::ivory);
        setColour(juce::Slider::thumbColourId, juce::Colours::ivory);

        setColour(juce::Slider::textBoxTextColourId, juce::Colours::ivory);
        //setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());

        setDefaultSansSerifTypeface(getTypeface(fontsIds::standardReadoutFontId));
        setDefaultLookAndFeel(this);
    }

    void LookAndFeel::drawRotarySlider (juce::Graphics & g,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPosProportional,
                                        float rotaryStartAngle,
                                        float rotaryEndAngle,
                                        juce::Slider & slider
                                        )
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto lineWidth = radius * 0.10f;
        auto arcRadius = radius - lineWidth;
        const auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        drawRotarySliderMeter(g, bounds, lineWidth, arcRadius, rotaryStartAngle, rotaryEndAngle, toAngle, slider);
        drawRotarySliderBase(g, radius * 0.6f, toAngle, bounds, slider);

    }

    void LookAndFeel::drawRotarySliderMeter (juce::Graphics& g,
                                            const juce::Rectangle<float> bounds,
                                            const float lineWidth,
                                            const float radius,
                                            const float startAngle,
                                            float endAngle,
                                            const float toAngle,
                                            juce::Slider& slider
                                            )
    {
        auto outline = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);

        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    radius,
                                    radius,
                                    0.0f,
                                    startAngle,
                                    endAngle,
                                    true);

        g.setColour (outline);
        g.strokePath (backgroundArc,
                    juce::PathStrokeType (lineWidth,
                                            juce::PathStrokeType::curved,
                                            juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    radius,
                                    radius,
                                    0.0f,
                                    startAngle,
                                    toAngle,
                                    true);

            g.setColour (fill);
            g.strokePath (valueArc,
                        juce::PathStrokeType (lineWidth,
                                                juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));
        }
    }

    void LookAndFeel::drawRotarySliderBase (juce::Graphics& g,
                                            const float radius,
                                            const float toAngle,
                                            const juce::Rectangle<float> bounds,
                                            juce::Slider& slider
                                            )
    {
        const auto viewAngle = juce::degreesToRadians(1.0f);
        const auto viewAngleCoefficientY = juce::dsp::FastMathApproximations::cos(viewAngle);
        const auto viewAngleCoefficientZ = juce::dsp::FastMathApproximations::sin(viewAngle);
        const juce::AffineTransform viewAngleTransform = juce::AffineTransform::scale(1.0f, viewAngleCoefficientY);

        const auto centreX = bounds.getCentreX();
        const auto centreY = bounds.getCentreY();

        constexpr auto ellipseArea = [](float x, float y, float radius)
        {
            const auto rx = x - radius;
            const auto ry = y - radius;
            const auto rw = radius * 2.0f;
            const auto rh = radius * 2.0f;

            return juce::Rectangle<float>(rx, ry, rw, rh);
        };

        g.setOrigin(centreX, centreY);

        const auto area = ellipseArea(0.0f, 0.0f, radius);
        g.setColour(juce::Colours::black);
        g.addTransform(viewAngleTransform);
        g.fillEllipse(area);

        const auto knobHeight = radius * 4.0f;
        const auto knobHeightTransformed = knobHeight * viewAngleCoefficientZ;

        auto thumbColor = findColour(juce::Slider::thumbColourId);
        if (Properties::contains(slider, propertyIds::knobColor))
        {
            //DBG("property caught");
            thumbColor = juce::Colour((int)Properties::get(slider, propertyIds::knobColor));
        }

        g.setColour(thumbColor);
        g.fillEllipse(ellipseArea(0.00, -knobHeightTransformed, radius * 0.9f));

        juce::Path pShadow;
        pShadow.addPieSegment(ellipseArea(0.0f, -knobHeightTransformed, radius * 0.8f),
            toAngle - 0.05f,
            toAngle + 0.05f,
            0.35f);
        pShadow.addPieSegment(ellipseArea(0.0f, -knobHeightTransformed, radius * 0.9f),
            juce::MathConstants<float>::pi * -0.75f,
            juce::MathConstants<float>::pi * 0.75f,
            0.99f);

        pShadow.applyTransform(juce::AffineTransform::translation(0.0f, -radius * -0.04f));
        g.setColour(juce::Colours::black);
        g.strokePath(pShadow,
                    juce::PathStrokeType(radius * 0.15f,
                                                    juce::PathStrokeType::JointStyle::curved,
                                                    juce::PathStrokeType::EndCapStyle::rounded));

        juce::Path p;
        p.addEllipse(ellipseArea(0.0f, -knobHeightTransformed, radius * 0.9f));
        p.addPieSegment(ellipseArea(0.0f, -knobHeightTransformed, radius * 0.9f),
                        toAngle - 0.05f,
                        toAngle + 0.05f,
                        0.3f);

        p.applyTransform(juce::AffineTransform::translation(0.0f, 0.0f));
        g.setColour(juce::Colour(0xff3c4361));
        g.strokePath(p,
                    juce::PathStrokeType(radius * 0.15f,
                                                    juce::PathStrokeType::JointStyle::curved,
                                                    juce::PathStrokeType::EndCapStyle::rounded));

    }

    juce::Font LookAndFeel::getLabelFont(juce::Label & label)
    {
        auto height = label.getHeight() * 0.7f;

        if (label.getParentComponent()->getName() == "Knob")
        {
            return juce::Font(getTypeface(fontsIds::standardReadoutFontId)).withHeight(height);
        }

        if (label.getName() == "KnobLabel")
        {
            return juce::Font(getTypeface(fontsIds::standardReadoutFontId)).withHeight(height);
        }

        return juce::Font(getTypeface(fontsIds::standardReadoutFontId)).withHeight(height);
    }

    void LookAndFeel::drawLinearSlider (juce::Graphics & g,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPosProportional,
                                        float minSliderPos,
                                        float maxSliderPos,
                                        juce::Slider::SliderStyle style,
                                        juce::Slider & slider
                                        )
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();

        const auto centreX = bounds.getCentreX();
        const auto centreY = bounds.getCentreY();

        const auto thumbY = sliderPosProportional;

        g.setColour(juce::Colours::black);
        g.drawLine(centreX, bounds.getY(), centreX, bounds.getBottom(),8);

        //g.fillRect(-32, -4, 64, 8);

        const float positionRatio = (float)sliderPosProportional / (float)maxSliderPos;
        const float maxAngle = juce::degreesToRadians(20.0);

        const float positionAngle = (positionRatio - 0.5f) * 2.0f * maxAngle;
        const float positionSin = -juce::dsp::FastMathApproximations::sin(positionAngle);
        const float positionCos = juce::dsp::FastMathApproximations::cos(positionAngle);

        const float sinY = positionSin * 16.0f;

        juce::Rectangle<float> thumbBase(-32, -16 * positionCos + sinY, 64, 32 * positionCos);
        juce::Rectangle<float> thumbTop(-28, -8 * positionCos, 56, 16 * positionCos);

        const float reduction = (1 - positionCos) * 0.2f;

        thumbBase.reduce(thumbBase.getWidth() * reduction, thumbBase.getHeight() * reduction);
        thumbTop.reduce(thumbTop.getWidth() * reduction, thumbTop.getHeight() * reduction);

        const float ticksFrom = bounds.getY() + 16.0f * juce::dsp::FastMathApproximations::sin(maxAngle);
        const float ticksTo = bounds.getBottom() - 16.0f * juce::dsp::FastMathApproximations::sin(maxAngle);
        const float ticksSpan = std::abs(ticksTo - ticksFrom);

        g.setColour(juce::Colours::ivory);
        for(int i = 0; i < 10; i++)
        {
            const float y = ticksFrom + ticksSpan / 9.0f * i;

            //g.drawLine(centreX + 12, y, centreX + 48, y, 4);
        }

        g.setOrigin(centreX, thumbY);
        juce::Path left;
        left.startNewSubPath(thumbBase.getTopLeft());
        left.lineTo(thumbBase.getBottomLeft());
        left.lineTo(thumbTop.getBottomLeft());
        left.lineTo(thumbTop.getTopLeft());
        left.lineTo(thumbBase.getTopLeft());

        juce::Path right = left;
        right.applyTransform(juce::AffineTransform::scale(-1.0, 1.0));

        juce::Path top;
        top.startNewSubPath(thumbBase.getTopLeft());
        top.lineTo(thumbBase.getTopRight());
        top.lineTo(thumbTop.getTopRight());
        top.lineTo(thumbTop.getTopLeft());
        top.lineTo(thumbBase.getTopLeft());

        juce::Path bottom;
        bottom.startNewSubPath(thumbBase.getBottomLeft());
        bottom.lineTo(thumbBase.getBottomRight());
        bottom.lineTo(thumbTop.getBottomRight());
        bottom.lineTo(thumbTop.getBottomLeft());
        bottom.lineTo(thumbBase.getBottomLeft());

        juce::Path line;
        line.startNewSubPath(-32, sinY);
        line.lineTo(-28, 0);
        line.lineTo(28, 0);
        line.lineTo(32, sinY);

        g.setColour(findColour(juce::ResizableWindow::backgroundColourId));
        g.fillRect(thumbBase);

        g.setColour(juce::Colours::grey);
        g.fillPath(left);

        g.setColour(juce::Colours::lightgrey);
        g.fillPath(top);

        g.setColour(juce::Colours::darkgrey);
        g.fillPath(right);
        g.fillPath(bottom);

        g.setColour(juce::Colours::ivory);
        g.fillRect(thumbTop);

        g.setColour(findColour(juce::ResizableWindow::backgroundColourId));
        g.strokePath(line, {4, juce::PathStrokeType::JointStyle::mitered, juce::PathStrokeType::EndCapStyle::rounded});
    }
}
