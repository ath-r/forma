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
    setSize (600, 450);
    setResizable(true, true);

    addAndMakeVisible(mainComponent);
    mainComponent.setBounds(0,0,600,450);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    

}

void PluginEditor::resized()
{
    float ratio = 600.0f / 450.0f;
    setSize(getHeight() * ratio, getHeight());

    float scale = getHeight() / 450.0f;
    
    mainComponent.setTransform(juce::AffineTransform::scale(scale));
}
bool PluginEditor::keyPressed (const juce::KeyPress& key) 
{
}
