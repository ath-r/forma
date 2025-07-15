#pragma once

#include "VeronikaVoice.h"
#include "dsp/SIMD.h"
#include "math/Conversion.h"

namespace Electrophilia::Veronika
{

    void VeronikaVoice::setContext (Context context)
    {
        c = context;

        squareOctaves.setContext(c);
        squareMutations.setContext(c);
        setFrequency (frequency);

        gateSmoother.setContext(c);
        gateSmoother.setTime(0.005);
    }

    void VeronikaVoice::setFrequency (float f)
    {
        static const vec4 mult = vec4fromFloats (0.5f, 1.0f, 2.0f, 4.0f);
        static const vec4 mult2 = vec4fromFloats(1.0f, 1.5f, 3.0f, 1.0f);

        frequency = f;
        squareOctaves.setFrequency ( mult * frequency);
        squareMutations.setFrequency ( mult2 * frequency);
    }

    void VeronikaVoice::setTime (double t)
    {
        squareOctaves.setTime (t);
        squareMutations.setTime(t);
    }

    bool VeronikaVoice::isActive() { return gate > 0.0f || gateSmoother.last() > 0.0f; }

    int VeronikaVoice::getNote() { return note; }

    void VeronikaVoice::handleNoteOn (Midi::MessageNoteOn message)
    {
        if (gate == 0.0f)
        {
            gate = 1.0f;
            note = message.note;

            setFrequency (Math::noteToFrequency (note));
        }

    }

    void VeronikaVoice::handleNoteOff (Midi::MessageNoteOff message) { gate = 0.0f; }

    vec4 VeronikaVoice::processSample(vec4& octaves, vec4& mutations)
    {
        gateSmoother.process(gate);

        octaves = squareOctaves.processSample() * gate;
        mutations = squareMutations.processSample() * gate;

        return octaves;
    }


}
