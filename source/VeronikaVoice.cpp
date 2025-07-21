#pragma once

#include "VeronikaVoice.h"
#include "dsp/SIMD.h"
#include "math/Conversion.h"
#include <cmath>

namespace Electrophilia::Veronika
{

    void VeronikaVoice::setContext (Context context)
    {
        c = context;

        squareOctaves.setContext(c);
        squareMutations.setContext(c);
        setFrequency (frequency);

        gateTimer.setContext(c);
        gateTimer.setTime(0.005f);

        actionOctaves.setContext(c);
        actionOctaves.setTime(0.0001f);

        actionMutations.setContext(c);
        actionMutations.setTime(0.0001f);
    }

    void VeronikaVoice::setFrequency (float f)
    {
        static const vec4 mult = vec4fromFloats (0.5f, 1.0f, 2.0f, 4.0f);
        static const vec4 mult2 = vec4fromFloats (
            1.0f,
            Math::semitonesToFrequencyRatio(7.0f),
            Math::semitonesToFrequencyRatio(28.0f),
            1.0f
        );

        frequency = f;
        squareOctaves.setFrequency ( mult * frequency);
        squareMutations.setFrequency ( mult2 * frequency);
    }

    void VeronikaVoice::setTime (double t)
    {
        squareOctaves.setTime (t);
        squareMutations.setTime(t);
    }

    bool VeronikaVoice::isActive() { return gate > 0.0f || gateTimer.last() > 0.0f; }

    int VeronikaVoice::getNote() { return note; }

    void VeronikaVoice::handleNoteOn (Midi::MessageNoteOn message)
    {
        if (gate == 0.0f)
        {
            gate = 1.0f;
            note = message.note;

            rng.setSeed(message.note);
            for (int i = 0; i < 4; i++)
            {
                actionThresholdOctaves[i] = rng.getFloat();
                actionThresholdMutations[i] = rng.getFloat();
            }

            const float x = float(message.velocity) / 127.0f;
            const float x1 = 1.0f - x;
            const float ease = 1.0f - x1 * x1 * x1;
            const float time = std::lerp(maxVelocityGateAttack, minVelocityGateAttack, ease);
            gateTimer.setTime(time);

            setFrequency (Math::noteToFrequency (note));
        }

    }

    void VeronikaVoice::handleNoteOff (Midi::MessageNoteOff message) { gate = 0.0f; }

    vec4 VeronikaVoice::processSample(vec4& octaves, vec4& mutations)
    {
        gateTimer.process(gate);

        const vec4 actionGate1 = vec4(1.0f) & vec4::greaterThanOrEqual(gateTimer.last(), actionThresholdOctaves);
        const vec4 actionGate2 = vec4(1.0f) & vec4::greaterThanOrEqual(gateTimer.last(), actionThresholdMutations);

        actionOctaves.process(actionGate1);
        actionMutations.process(actionGate2);

        octaves = squareOctaves.processSample() * actionOctaves.last();
        mutations = squareMutations.processSample() * actionMutations.last();

        return octaves;
    }

}
