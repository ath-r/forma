#pragma once

#include <algorithm>
#include <array>
#include <cstdlib>

#include "dsp/SIMD.h"
#include "dsp/Context.h"
#include "dsp/Filter.h"
#include "math/Conversion.h"

#include "math/Polynomial.h"

#include "FormaFilterNonlinearity.h"
#include "FormaHum.h"


namespace Ath::Forma
{
    class TimbreProcessor
    {
        template<int size>
        class FilterBank
        {
            Dsp::Context c = 48000.0f;
            std::array<Dsp::Filter1P<vec4>, size> filters;

            Dsp::Filter1P<vec4> hp;
            vec4 wetGain = 1.0f;
            vec4 dryGain = 0.0f;

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

            void setWetGain(vec4 gain)
            {
                wetGain = gain;
            }

            void setDryGain(vec4 gain)
            {
                dryGain = gain;
            }

            vec4 process(vec4 in)
            {
                filters[0].process(in);

                for (int i = 1; i < filters.size(); i++)
                {
                    filters[i].process(filters[i - 1].last());
                }

                const vec4 out = filters[filters.size() - 1].last() * wetGain + in * dryGain;

                //return out;
                return out - hp.process(out);
            }
        };

        Dsp::Context c = 48000.0f;

        std::array<vec4, 6> inputs;

        float tp1freqs[24] =
        {
            20, 40, 80, 160,
            100, 200, 400, 800,
            150, 300, 600, 1200,
            150, 300, 600, 1200,
            200, 400, 800, 1600,
            200, 400, 800, 1600
        };

        float tp2freqs[24] =
        {
            30, 60, 120, 240,
            150, 300, 600, 1200,
            200, 400, 800, 1600,
            200, 400, 800, 1600,
            200, 400, 800, 1600,
            200, 400, 800, 1600
        };

        float tp3freqs[24] =
        {
            60, 120, 240, 480,
            300, 600, 1200, 2400,
            300, 600, 1200, 2400,
            400, 800, 1600, 3200,
            400, 800, 1600, 3200,
            400, 800, 1600, 3200
        };

        float tp4freqs[20] =
        {
            120, 240, 480, 960,
            500, 1000, 2000, 4000,
            500, 1000, 2000, 4000,
            500, 1000, 2000, 4000,
            5500, 1000, 2000, 4000
        };

        float tp5freqs[16] =
        {
            240, 480, 960, 1920,
            500, 1000, 2000, 4000,
            500, 1000, 2000, 4000,
            500, 1000, 2000, 4000
        };

        float tp6freqs[12] =
        {
            500, 1000, 2000, 4000,
            1000, 2000, 4000, 8000,
            1000, 2000, 4000, 8000
        };

        static constexpr auto db = Math::decibelsToAmplitude;

        const vec4 tp1SquareGain = vec4fromFloats(db(-50), db(-50), db(-50), db(-50));
        const vec4 tp2SquareGain = vec4fromFloats(db(-50), db(-52), db(-57), db(-60));
        const vec4 tp3SquareGain = vec4fromFloats(db(-50), db(-57), db(-55), db(-55));
        const vec4 tp4SquareGain = vec4fromFloats(db(-55), db(-65), db(-65), db(-65));
        const vec4 tp5SquareGain = vec4fromFloats(db(-60), db(-70), db(-70), db(-70));
        const vec4 tp6SquareGain = vec4fromFloats(db(-65), db(-70), db(-70), db(-75));

        FilterBank<6> tp1;
        FilterBank<6> tp2;
        FilterBank<6> tp3;
        FilterBank<5> tp4;
        FilterBank<4> tp5;
        FilterBank<3> tp6;

        Waveshaper::FormaFilterStageNonlinearity nonlinearity;

        FormaHum hum;

    public:
        void setContext (Dsp::Context context);

        void addSignal (vec4 in, int midiNote);

        vec4 processSample();
    };
}
