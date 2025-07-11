#pragma once

#include <algorithm>
#include <array>
#include <cstdlib>

#include "dsp/SIMD.h"
#include "dsp/Context.h"
#include "dsp/Filter.h"
#include "math/Conversion.h"


namespace Electrophilia::Veronika
{
    class TimbreProcessor
    {
        Dsp::Context c = 48000.0f;

        std::array<vec4, 6> inputs;

        float tp1freqs[24] =
        {
            20, 46, 84, 184,
            146, 329, 621, 1317,
            380, 829, 1612, 3417,
            697, 1522, 2957, 6267,
            1107, 2416, 4695, 9948,
            1659, 3620, 7034, 13000
        };

        float tp2freqs[24] =
        {
            27, 59, 109, 232,
            190, 414, 783, 1659,
            479, 1045, 1974, 4305,
            879, 1917, 3726, 7896,
            1436, 3133, 6088, 13000,
            2280, 5120, 9948, 15000
        };

        float tp3freqs[24] =
        {
            37, 82, 146, 310,
            253, 563, 1015, 2152,
            640, 1395, 2635, 5583,
            1172, 2560, 4832, 10240,
            2031, 4431, 8610, 13000,
            4305, 9390, 13000, 15000
        };

        float tp4freqs[20] =
        {
            53, 116, 201, 439,
            359, 783, 1395, 2957,
            879, 1917, 3517, 7671,
            1708, 3835, 7034, 10000,
            3948, 8610, 13000, 13000
        };

        float tp5freqs[16] =
        {
            82, 179, 302, 640,
            538, 1173, 2031, 4305,
            1356, 2957, 5424, 10240,
            3417, 7452, 10000, 13000
        };

        float tp6freqs[12] =
        {
            136, 320, 493, 1045,
            905, 1917, 3225, 7034,
            2873, 6088, 10240, 13000
        };

        static constexpr auto db = Math::decibelsToAmplitude;

        const float tp1SquareGains[4] = {  db(-40), db(-45), db(-47), db(-50)};
        const float tp2SquareGains[4] = {  db(-45), db(-52), db(-57), db(-60)};
        const float tp3SquareGains[4] = {  db(-50), db(-57), db(-55), db(-55)};
        const float tp4SquareGains[4] = {  db(-55), db(-65), db(-65), db(-65)};
        const float tp5SquareGains[4] = {  db(-60), db(-70), db(-70), db(-70)};
        const float tp6SquareGains[4] = {  db(-65), db(-70), db(-70), db(-75)};

        const vec4 tp1SquareGain = vec4::fromRawArray(tp1SquareGains);
        const vec4 tp2SquareGain = vec4::fromRawArray(tp2SquareGains);
        const vec4 tp3SquareGain = vec4::fromRawArray(tp3SquareGains);
        const vec4 tp4SquareGain = vec4::fromRawArray(tp4SquareGains);
        const vec4 tp5SquareGain = vec4::fromRawArray(tp5SquareGains);
        const vec4 tp6SquareGain = vec4::fromRawArray(tp6SquareGains);

        template<int size>
        class FilterBank
        {
            Dsp::Context c = 48000.0f;
            std::array<Dsp::Filter1P<vec4>, size> filters;

            Dsp::Filter1P<vec4> hp;

        public:
            void setContext(Dsp::Context context)
            {
                c = context;
                hp.setContext(c);

                for (auto& filter : filters)
                {
                    filter.setContext(c);
                    filter.reset();
                }
            }

            void setFrequencies(float* freqs)
            {
                for (int i = 0; i < filters.size(); i++)
                {
                    filters[i].setCutoffFrequency(vec4::fromRawArray(&freqs[i * 4]));
                }

                hp.setCutoffFrequency(vec4::fromRawArray(freqs));
            }

            vec4 process(vec4 in)
            {
                filters[0].process(in);

                for (int i = 1; i < filters.size(); i++)
                {
                    filters[i].process(filters[i - 1].last());
                }

                const vec4 out = filters[filters.size() - 1].last();

                //return out;
                return out - hp.process(out);
            }
        };

        FilterBank<6> tp1;
        FilterBank<6> tp2;
        FilterBank<6> tp3;
        FilterBank<5> tp4;
        FilterBank<4> tp5;
        FilterBank<3> tp6;

    public:

        void setContext(Dsp::Context context)
        {
            c = context;

            tp1.setContext(c);
            tp1.setFrequencies(tp1freqs);

            tp2.setContext(c);
            tp2.setFrequencies(tp2freqs);

            tp3.setContext(c);
            tp3.setFrequencies(tp3freqs);

            tp4.setContext(c);
            tp4.setFrequencies(tp4freqs);

            tp5.setContext(c);
            tp5.setFrequencies(tp5freqs);

            tp6.setContext(c);
            tp6.setFrequencies(tp6freqs);
        }

        void addSignal(vec4 in, int midiNote)
        {
            int relativeNote = midiNote - Math::C1_MIDI_NOTE_NUMBER;
            int input = std::clamp((relativeNote + 6) / 12, 0, 5);

            float dB = 16.0f * (float(relativeNote) / 12.0f);
            float gain = Math::decibelsToAmplitude(dB);

            inputs[input] += in;
        }

        vec4 processSample()
        {
            const vec4 out1 = tp1.process(inputs[0]) + inputs[0] * tp1SquareGain;
            const vec4 out2 = tp2.process(inputs[1]) + inputs[1] * tp2SquareGain;
            const vec4 out3 = tp3.process(inputs[2]) + inputs[2] * tp3SquareGain;
            const vec4 out4 = tp4.process(inputs[3]) + inputs[3] * tp4SquareGain;
            const vec4 out5 = tp5.process(inputs[4]) + inputs[4] * tp5SquareGain;
            const vec4 out6 = tp6.process(inputs[5]) + inputs[5] * tp6SquareGain;

            for (auto& x : inputs)
            {
                x = 0.0f;
            }

            return (out1 + out2 + out3 + out4 + out5 + out6) * Math::decibelsToAmplitude(-18);

        }
    };
}
