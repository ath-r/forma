#pragma once

#include "VcaComponent.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "LookAndFeel.h"
#include "LogoComponent.h"
#include "FlutesComponent.h"
#include "PercussionComponent.h"

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

    LogoComponent logo;
    FlutesComponent flutes;
    VcaComponent vca;
    PercussionComponent percussion;

    Ath::Gui::LookAndFeel lookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
}