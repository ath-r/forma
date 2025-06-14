#pragma once

#include "Properties.h"

namespace Electrophilia::Gui
{

    juce::var Properties::get(const juce::Component & component, propertyIds id)
    {
        //DBG("get");
        //DBG(propertyNames[id]);
        return component.getProperties().getWithDefault(propertyNames[id], -1);
    }

    bool Properties::set(juce::Component & component, propertyIds id, juce::var value)
    {
        //DBG("set");
        //DBG(propertyNames[id]);
        return component.getProperties().set(propertyNames[id], value);
    }

    bool Properties::contains(const juce::Component & component, propertyIds id)
    {
        //DBG("contains");
        //DBG(propertyNames[id]);
        //DBG((component.getProperties().contains(propertyNames[id]) ? "yes" : "no"));
        return component.getProperties().contains(propertyNames[id]);
    }

}
