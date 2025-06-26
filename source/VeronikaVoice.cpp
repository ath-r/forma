#pragma once

#include "VeronikaVoice.h"
#include "math/Conversion.h"

namespace Electrophilia::Veronika
{

    void VeronikaVoice::setContext (Context context)
    {
        c = context;
        octaves.setContext(c);
        setFrequency (frequency);
    }
    void VeronikaVoice::setFrequency (float f)
    {
        static const float multipliers[4] = { 1.0f, 2.0f, 4.0f, 8.0f };
        static const vec4 mult = vec4::fromRawArray (multipliers);

        frequency = f;
        octaves.setFrequency (mult * f);
    }

    void VeronikaVoice::setTime (double t) { octaves.setTime (t); }

    bool VeronikaVoice::isActive() { return gate > 0.0f; }

    int VeronikaVoice::getNote() { return note; }

    void VeronikaVoice::handleNoteOn (Midi::MessageNoteOn message)
    {
        gate = 1.0f;
        note = message.note;

        setFrequency (Math::noteToFrequency (note));
    }

    void VeronikaVoice::handleNoteOff (Midi::MessageNoteOff message) { gate = 0.0f; }

    vec4 VeronikaVoice::processSample() { return octaves.processSample() * gate; }


}
