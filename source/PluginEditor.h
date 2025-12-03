#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

#include "gui/LookAndFeel.h"
#include "gui/MainComponent.h"
#include "gui/PerformanceMeterComponent.h"

#include "PluginProcessor.h"


//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    bool keyPressed (const juce::KeyPress &key) override;

    void readSettings();

private:

    float initialWidth = 1500.0f;
    float initialHeight = 400.0f; 

    PluginProcessor& processorRef;

    Ath::Forma::MainComponent mainComponent;
    Ath::Forma::PerformanceMeterComponent performanceMeter;

    Ath::Gui::LookAndFeel lookAndFeel;
    juce::ComponentBoundsConstrainer constrainer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
