#pragma once

#include "FormaKeySwitch.h"
#include "math/Clamp.h"
#include <cmath>

namespace Ath::Forma
{
    void FormaKeySwitch::setContext (Context context)
    {
        c = context;
        filter.setContext(context);
        filter.setCutoffFrequency(5000.0f);
    }

    void FormaKeySwitch::init (int keyNumber) 
    {
        Math::Random::LCG rng;
        rng.setSeed(keyNumber);
        actionThreshold = {
            rng.getFloat(), 
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            0.0f,
            0.0f
        };
    };

    bool FormaKeySwitch::isActive() { return true; }

    void FormaKeySwitch::handleNoteOn (Midi::MessageNoteOn message)
    {
        gate = 1.0f;

        const Simd::float8 x = float(message.velocity) / 127.0f;
        const Simd::float8 time = Simd::lerp(maxVelocityGateAttack, minVelocityGateAttack, x);
        
        delta = Simd::float8(c.T) / time;
    }

    void FormaKeySwitch::handleNoteOff (Midi::MessageNoteOff message) 
    { 
        gate = 0.0f;
        delta = Simd::float8(-c.T) / time;
    }

    Simd::float8 FormaKeySwitch::processSample()
    {
        value += delta;
        value = Simd::clamp(value, 0.0f, 1.0f);

        auto logic = Simd::blend(Simd::float8(1.0f), Simd::float8(0.0f), value > actionThreshold);

        return filter.process(logic);
    }
}
