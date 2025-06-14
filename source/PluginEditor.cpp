#include "gui/KnobWithLabel.h"
#include "gui/LookAndFeel.h"
#include "gui/Properties.h"

#include "PluginEditor.h"
#include "PluginParameters.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <BinaryData.h>


using namespace Electrophilia::Plugin::Tremolo;

using namespace Electrophilia::Gui;

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
    flute16(
        ParametersByID[F16].id,
        processorRef.treeState),
    flute8(
        ParametersByID[F8].id,
        processorRef.treeState),
    flute5(
        ParametersByID[F5].id,
        processorRef.treeState),
    flute4(
        ParametersByID[F4].id,
        processorRef.treeState),
    flute2(
        ParametersByID[F2].id,
        processorRef.treeState),
    flute1(
        ParametersByID[F1].id,
        processorRef.treeState)
{
    Electrophilia::Gui::LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);

    juce::ignoreUnused (processorRef);

    addAndMakeVisible(flute16);
    addAndMakeVisible(flute8);
    addAndMakeVisible(flute5);
    addAndMakeVisible(flute4);
    addAndMakeVisible(flute2);
    addAndMakeVisible(flute1);

    //addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 450);


}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Path p;
    p.addRoundedRectangle(getLocalBounds().reduced(10), 20);

    g.setColour(juce::Colours::ivory);
    g.strokePath(p, {4, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded});
    //g.fillAll(juce::Colours::black);

    auto logo = juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize)->getOutlineAsPath();

    const float centreX = getWidth() / 2;
    logo.applyTransform(juce::AffineTransform::scale(3.0f));
    logo.applyTransform(juce::AffineTransform::translation(-logo.getBounds().getWidth() / 2.0f, 0.0f));
    logo.applyTransform(juce::AffineTransform::translation(getWidth() / 2.0f, 40.0f));
    g.fillPath(logo);

}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(20);
    auto area2 = area;

    area.removeFromBottom(area.getHeight() * 0.66f);
    inspectButton.setBounds (area.withSizeKeepingCentre(100, 50));

    area2.reduce(20, 20);
    area2.removeFromTop(area2.getHeight() * 0.33f);

    auto sliderWidth = area2.getWidth() / 6;

    flute16.setBounds(area2.removeFromLeft(sliderWidth));
    flute8.setBounds(area2.removeFromLeft(sliderWidth));
    flute5.setBounds(area2.removeFromLeft(sliderWidth));
    flute4.setBounds(area2.removeFromLeft(sliderWidth));
    flute2.setBounds(area2.removeFromLeft(sliderWidth));
    flute1.setBounds(area2);
}
