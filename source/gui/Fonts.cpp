#include "Fonts.h"

#include <BinaryData.h>

namespace Ath::Gui
{
    juce::Typeface::Ptr getTypeface(fontsIds font)
    {
        static juce::Typeface::Ptr fontArray[FONT_COUNT]
        {
            [standardLabelFontId] = juce::Typeface::createSystemTypefaceFor(BinaryData::RussoOneCustom_otf,
                                                                          BinaryData::RussoOneCustom_otfSize),
            [standardReadoutFontId] = juce::Typeface::createSystemTypefaceFor(BinaryData::OxaniumBold_ttf,
                                                                                BinaryData::OxaniumBold_ttfSize)
        };

        return fontArray[font];
    }
}
