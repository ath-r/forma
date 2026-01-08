#pragma once

#include "FormaKeySwitch.h"
#include "ath_dsp/math/Math.h"
#include "ath_dsp/math/Simd.h"
#include "ath_dsp/math/Random.h"
#include <cmath>

namespace Ath::Forma
{
    void FormaNeedleContacts::setContext (Context context)
    {
        c = context;
    }

    void FormaNeedleContacts::init (int keyNumber) 
    {
        Math::Random::MersenneTwister rng;
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

    bool FormaNeedleContacts::isGateOn() { return gate; }

    bool FormaNeedleContacts::isActive() { return value.sum() > 0.0f; }

    void FormaNeedleContacts::handleNoteOn (Midi::MessageNoteOn message)
    {
        const Simd::float8 x = Math::cubeInv(float(message.velocity) / 127.0f);
        const Simd::float8 time = Simd::lerp(maxVelocityGateAttack, minVelocityGateAttack, x);

        delta = Simd::float8(c.T) / time;

        gate = true;
    }

    void FormaNeedleContacts::handleNoteOff (Midi::MessageNoteOff message) 
    {
        delta = Simd::float8(-c.T) / 0.001f;

        gate = false;
    }

    Simd::float8 FormaNeedleContacts::processSample()
    {
        value += delta;
        value = Simd::clamp(value, 0.0f, 1.0f);
        auto logic = value > actionThreshold;

        y = Simd::float8(1.0f) & logic;
        return y;
    }
}
