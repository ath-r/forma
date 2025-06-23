#pragma once

#include "math/Math.h"
#include "dsp/BlitSquare.h"
#include "dsp/Context.h"

#include "control/Midi.h"

namespace Electrophilia::Veronika
{
    using namespace Electrophilia::Dsp;
    using namespace Electrophilia::Math;
    using namespace Electrophilia::Control;

    class VeronikaVoice
    {
        Context c = Context(48000.0f);
        Oscillator::BlitSquareSimd octaves;

        float frequency = 440.0f;
        float gate = 0.0f;

    public:

        void setContext(Context context)
        {
            c = context;
            setFrequency(frequency);
        }

        void setFrequency(float f)
        {
            static const float multipliers[4] = {1.0f, 2.0f, 4.0f, 8.0f};
            static const vec4 mult = vec4::fromRawArray(multipliers);

            frequency = f;
            octaves.setFrequency(mult * f);
        }

        bool isActive()
        {
            return gate > 0.0f;
        }

        void handleNoteOn(Midi::MessageNoteOn message)
        {
            gate = 1.0f;

            setFrequency(Math::noteToFrequency(message.note));
        }

        void handleNoteOff(Midi::MessageNoteOff message)
        {
            gate = 0.0f;
        }

        vec4 processSample()
        {
            return octaves.processSample() * gate;
        }
    };
}
