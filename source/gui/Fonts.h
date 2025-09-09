#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Ath::Gui
{

    enum fontsIds
    {
        standardLabelFontId,
        standardReadoutFontId,
        FONT_COUNT
    };

    juce::Typeface::Ptr getTypeface(fontsIds font);
}
