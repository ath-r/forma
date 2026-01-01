#pragma once

#include "../Context.h"
#include "../Filter.h"
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

        T add = 0.0f;
        T mul = 1.0f;

        T out = 0.0f;

        Filter::Naive::LowPass1<T> filter;

        bool gate = false;
        bool tremolo = false;
public:
        void setContext(Context context)
        {
            c = context;
            setTime(time);

            filter.setContext(c);
            filter.setCutoffFrequency(1000);
        }

        void setTime(double t)
        {
            time = std::clamp(t, 0.01, 10.0);
            auto decayInSamples = time * c.SR;
            a = std::pow(0.1, 1.0 / decayInSamples);
        }

        void setCrescendo(bool cresc)
        {
            // cv in perc mode: y
            // cv in cresc mode: 1 - y
            add = cresc ? 1.0 : 0.0;
            mul = cresc ? -1.0 : 1.0; 
        }

        void setTremolo(bool trem)
        {
            tremolo = trem;
        }

        void handleNoteOn(Control::Midi::MessageNoteOn message)
        {
            y = 1.0;

            gate = true;
        }

        void handleNoteOff(Control::Midi::MessageNoteOff message)
        {
            gate = false;
        }

        inline T processSample()
        {
            if (tremolo)
            {
                if (gate && y < 0.01) handleNoteOff(Control::Midi::MessageNoteOff());
                if (!gate && filter.last() < 0.01) handleNoteOn(Control::Midi::MessageNoteOn());
            }

            y *= a;
            out = filter.process(y * mul + add);

            return out;
        }

        inline T getGate()
        {
            return gate ? 1.0f : 0.0f;
        }

        inline T last()
        {
            return out;
        }
    };
}