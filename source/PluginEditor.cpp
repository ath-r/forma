#include "gui/LookAndFeel.h"

#include "PluginEditor.h"
#include "PluginParameters.h"
#include "gui/MainComponent.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <BinaryData.h>
using namespace Ath::Forma;

using namespace Ath::Gui;

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), 
    processorRef (p), 
    mainComponent(p.treeState)
{
    Ath::Gui::LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);

    juce::ignoreUnused (processorRef);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);

    addAndMakeVisible(mainComponent);
    constrainer.setFixedAspectRatio(4.0f / 3.0f);
    constrainer.setMinimumHeight(200);
    setConstrainer(&constrainer);
    mainComponent.setBounds(0,0,600,450);

    float scale = processorRef.pluginInstanceSettings.getDoubleAttribute("scale", -1.0);
    if (scale > 0.0) setSize(600.0f * scale, 450.0f * scale);
    else setSize (600, 450);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
}

void PluginEditor::resized()
{
    float scale = getHeight() / 450.0f;
    processorRef.pluginInstanceSettings.setAttribute("scale", scale);
    
    mainComponent.setTransform(juce::AffineTransform::scale(scale));
}

bool PluginEditor::keyPressed (const juce::KeyPress& key) 
{

}

void PluginEditor::readSettings() 
{
    
}
