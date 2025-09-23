#pragma once

#include "FormaKeySwitch.h"
#include "math/Clamp.h"
#include <cmath>

namespace Ath::Forma
{
    void FormaKeySwitch::setContext (Context context)
    {
        c = context;
    }

    void FormaKeySwitch::setThreshold (float threshold) 
    {
        actionThreshold = threshold;
    };

    bool FormaKeySwitch::isActive() { return gate > 0.0f || value > 0.0f; }

    void FormaKeySwitch::handleNoteOn (Midi::MessageNoteOn message)
    {
        gate = 1.0f;

        const float x = float(message.velocity) / 127.0f;
        const float x1 = 1.0f - x;
        const float ease = 1.0f - x1 * x1 * x1;
        const float time = std::lerp(maxVelocityGateAttack, minVelocityGateAttack, ease);
        
        delta = c.T / time;
    }

    void FormaKeySwitch::handleNoteOff (Midi::MessageNoteOff message) 
    { 
        gate = 0.0f;
        delta = -c.T / time;
    }

    float FormaKeySwitch::processSample()
    {
        value += delta;
        value = Math::clamp(value, 0.0f, 1.0f);

        return value > actionThreshold ? 1.0f : 0.0f;
    }
}
