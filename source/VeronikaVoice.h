#pragma once

#include "dsp/SquareSimd.h"
#include "dsp/Context.h"

#include "control/Midi.h"

namespace Electrophilia::Veronika
{
    using namespace Electrophilia::Dsp;
    using namespace Electrophilia::Control;

    class VeronikaVoice
    {
        Context c = Context(48000.0f);
        Oscillator::SquareSimd octaves;

        float frequency = 440.0f;
        float gate = 0.0f;

        int note;

    public:
        void setContext (Context context);

        void setFrequency (float f);

        void setTime (double t);

        bool isActive();

        int getNote();

        void handleNoteOn (Midi::MessageNoteOn message);

        void handleNoteOff (Midi::MessageNoteOff message);

        vec4 processSample();
    };
}
