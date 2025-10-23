#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace Ath::Forma 
{
    class FrameComponent : public juce::Component
    {
    public:

        juce::Label label;

        FrameComponent();

        void paint (juce::Graphics& g) override;

        void resized() override;

    private:
        juce::Path p;
    };
}