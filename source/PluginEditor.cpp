#include "gui/LookAndFeel.h"

#include "PluginEditor.h"
#include "PluginParameters.h"
#include "gui/MainComponent.h"
#include "gui/PerformanceMeterComponent.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <BinaryData.h>
using namespace Ath::Forma;

using namespace Ath::Gui;

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), 
    processorRef (p), 
    mainComponent(p.treeState),
    performanceMeter(p.performance)
{
    Ath::Gui::LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);

    juce::ignoreUnused (processorRef);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);

    addAndMakeVisible(mainComponent);
    addAndMakeVisible(performanceMeter);

    constrainer.setFixedAspectRatio(initialWidth / initialHeight);
    constrainer.setMinimumHeight(200);
    setConstrainer(&constrainer);

    float scale = processorRef.pluginInstanceSettings.getDoubleAttribute("scale", -1.0);
    if (scale > 0.0) setSize(initialWidth * scale, initialHeight * scale);
    else setSize (initialWidth, initialHeight);

    mainComponent.setBounds(0, 0, initialWidth, initialHeight);
    performanceMeter.setBounds(20, 20, 1000, 100);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
}

void PluginEditor::resized()
{
    float scale = getHeight() / initialHeight;
    processorRef.pluginInstanceSettings.setAttribute("scale", scale);

    mainComponent.setTransform(juce::AffineTransform::scale(scale));
    performanceMeter.setTransform(juce::AffineTransform::scale(scale * 2 / 3));
}

bool PluginEditor::keyPressed (const juce::KeyPress& key) 
{

}

void PluginEditor::readSettings() 
{
    
}
