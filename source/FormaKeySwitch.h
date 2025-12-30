#pragma once

#include "dsp/filter/Naive.h"
#include "dsp/Context.h"

#include "control/Midi.h"
#include "math/Simd.h"

namespace Ath::Forma
{
    using namespace Ath::Dsp;
    using namespace Ath::Control;

    class FormaNeedleContacts
    {
        Context c = Context(48000.0f);

        Simd::float8 actionThreshold = 0.0f;

        const Simd::float8 minVelocityGateAttack = 0.00001f;
        const Simd::float8 maxVelocityGateAttack = 0.05f;
        \
        Simd::float8 delta = 0.0f;
        Simd::float8 value = 0.0f;
        Simd::float8 y = 0.0f;

        bool gate = false;\
    public:
        void setContext (Context context);

        void init (int keyNumber);

        bool isGateOn();

        bool isActive();

        void handleNoteOn (Midi::MessageNoteOn message);

        void handleNoteOff (Midi::MessageNoteOff message);

        Simd::float8 processSample();

        inline Simd::float8 last() { return y; }
    };
}
