#pragma once

#include "../Context.h"
#include "../../math/Conversion.h"
#include "../../control/Midi.h"
#include <algorithm>

namespace Ath::Dsp::Cv
{
    template <typename T>
    class PercussionGenerator
    {
        Context c;
        double time = 1.0;

        T a = 0.9;
        T y = 0.0;

        T a2 = 0.9;
        T y2 = 0.0;

        T add = 0.0f;
        T mul = 1.0f;

        T out = 0.0f;
public:
        void setContext(Context context)
        {
            c = context;
            setTime(time);
        }

        void setTime(double t)
        {
            time = std::clamp(t, 0.001, 10.0);
            auto decayInSamples = t * c.SR;
            a = std::pow(0.01, 1.0 / decayInSamples);
        }

        void setCrescendo(bool cresc)
        {
            // cv in perc mode: y
            // cv in cresc mode: 1 - y
            add = cresc ? 1.0 : 0.0;
            mul = cresc ? -1.0 : 1.0; 
        }

        void handleNoteOn(Control::Midi::MessageNoteOn message)
        {
            y = 1.0;
            y2 = 1.0;
            a2 = 1.0;
            setTime(time);
        }

        void handleNoteOff(Control::Midi::MessageNoteOff message)
        {
            auto decayInSamples = 0.005 * c.SR;
            a2 = std::pow(0.01, 1.0 / decayInSamples);
        }

        inline T processSample()
        {
            y *= a;
            y2 *= a2;
            out = (y * mul + add) * y2;
            return out;
        }

        inline T last()
        {
            return out;
        }
    };
}