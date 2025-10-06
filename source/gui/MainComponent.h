#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "LookAndFeel.h"
#include "SliderWithLabel.h"
#include "../PluginParameters.h"
#include "BinaryData.h"

namespace Ath::Forma 
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent (juce::AudioProcessorValueTreeState& vtsRef);

        void paint (juce::Graphics& g) override;

        void resized() override;

    private:
    juce::AudioProcessorValueTreeState& vts;

    Ath::Gui::SliderWithLabel flute16, flute8, flute5, flute4, flute2, flute1;

    Ath::Gui::LookAndFeel lookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
}