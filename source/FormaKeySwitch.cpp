#pragma once

#include "FormaKeySwitch.h"
#include "math/Clamp.h"
#include "math/Easings.h"
#include "math/Simd.h"
#include <cmath>

namespace Ath::Forma
{
    void FormaNeedleContacts::setContext (Context context)
    {
        c = context;
        filter.setContext(context);
        filter.setCutoffFrequency(50.0f);
    }

    void FormaNeedleContacts::init (int keyNumber) 
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

    bool FormaNeedleContacts::isActive() { return active; }

    void FormaNeedleContacts::handleNoteOn (Midi::MessageNoteOn message)
    {
        const Simd::float8 x = Math::easeOutCubic(float(message.velocity) / 127.0f);
        const Simd::float8 time = Simd::lerp(maxVelocityGateAttack, minVelocityGateAttack, x);

        filter.setCutoffFrequency(10000.0f);
        delta = Simd::float8(c.T) / time;

        active = true;
    }

    void FormaNeedleContacts::handleNoteOff (Midi::MessageNoteOff message) 
    {
        filter.setCutoffFrequency(100.0f);
        delta = Simd::float8(-c.T) / time;

        active = false;
    }

    Simd::float8 FormaNeedleContacts::processSample(Simd::float8 x)
    {
        value += delta;
        value = Simd::clamp(value, 0.0f, 1.0f);
        auto logic = value > actionThreshold;

        return x * filter.process(Simd::float8(1.0f) & logic);
    }
}
