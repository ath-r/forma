#pragma once

#include "juce_core/juce_core.h"
#include <string>

#include <juce_gui_basics/juce_gui_basics.h>

namespace Electrophilia::Gui
{

    using VarArray = juce::Array<juce::var>;

    enum propertyIds
    {
        knobColor,
        knobTickPosition,
        knobTickText,
        PROPERTY_COUNT
    };

    static juce::Identifier propertyNames[PROPERTY_COUNT]
    {
        [knobColor] = "knobColor",
        [knobTickPosition] = "knobTickPosition",
        [knobTickText] = "knobTickText"
    };

    class Properties
    {
    public:

        static juce::var get(const juce::Component & component, propertyIds id);

        static bool set(juce::Component & component, propertyIds id, juce::var value);

        static bool contains(const juce::Component & component, propertyIds id);
    };

}
