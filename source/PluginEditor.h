#pragma once

#include "gui/LookAndFeel.h"
#include "gui/KnobWithLabel.h"

#include "gui/SliderWithLabel.h"
#include "PluginProcessor.h"
#include "BinaryData.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include <array>

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

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    std::array<float, 10> scales = {1.0, 1.148698355, 1.319507911, 1.515716567, 1.741101127, 2.0 };
    int scale_i = 0;

    Ath::Gui::SliderWithLabel flute16, flute8, flute5, flute4, flute2, flute1;

    Ath::Gui::LookAndFeel lookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
