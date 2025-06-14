#pragma once

#include "gui/LookAndFeel.h"
#include "gui/KnobWithLabel.h"

#include "gui/SliderWithLabel.h"
#include "PluginProcessor.h"
#include "BinaryData.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    Electrophilia::Gui::SliderWithLabel flute16, flute8, flute5, flute4, flute2, flute1;

    Electrophilia::Gui::LookAndFeel lookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
