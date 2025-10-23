#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

#include "control/Events.h"

class ParameterObserver : public juce::AudioProcessorValueTreeState::Listener
{
public:

    juce::RangedAudioParameter* parameter;
    Ath::Control::EventOutput<float> eventOut;

    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        eventOut.fire(newValue);
    }

    void forceCheck()
    {
        if (parameter != nullptr)
        {
            parameter->sendValueChangedMessageToListeners(parameter->getValue());
        }
        //eventOut.fire(parameter->convertTo0to1(parameter->getValue()));
    }
};
