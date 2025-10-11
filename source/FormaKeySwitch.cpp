#pragma once

#include "FormaKeySwitch.h"
#include "math/Clamp.h"
#include "math/Easings.h"
#include "math/Simd.h"
#include <cmath>

namespace Ath::Forma
{
    void FormaKeySwitch::setContext (Context context)
    {
        c = context;
        filter.setContext(context);
        filter.setCutoffFrequency(50.0f);
    }

    void FormaKeySwitch::init (int keyNumber) 
    {
        Math::Random::Mersenne rng;
        rng.setSeed(keyNumber);
        actionThreshold = {
            rng.getFloat(), 
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            rng.getFloat(),
            2.0f,
            2.0f
        };
    };

    bool FormaKeySwitch::isActive() { return true; }

    void FormaKeySwitch::handleNoteOn (Midi::MessageNoteOn message)
    {
        const Simd::float8 x = Math::easeOutCubic(float(message.velocity) / 127.0f);
        const Simd::float8 time = Simd::lerp(maxVelocityGateAttack, minVelocityGateAttack, x);

        filter.setCutoffFrequency(10000.0f);
        delta = Simd::float8(c.T) / time;
    }

    void FormaKeySwitch::handleNoteOff (Midi::MessageNoteOff message) 
    {
        filter.setCutoffFrequency(100.0f);
        delta = Simd::float8(-c.T) / time;
    }

    Simd::float8 FormaKeySwitch::processSample(Simd::float8 x)
    {
        value += delta;
        value = Simd::clamp(value, 0.0f, 1.0f);

        auto inputSign = Simd::sign(x);
        auto zeroCrossing = inputSign != inputSign1;

        auto logic = value > actionThreshold;
        auto shouldChange = logic != gate;

        gate = gate ^ (shouldChange & zeroCrossing);

        inputSign1 = inputSign;

        return x * filter.process(Simd::float8(1.0f) & logic);
    }
}
