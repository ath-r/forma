#include "FormaHum.h"

#include "math/Conversion.h"
#include "math/Simd.h"
#include "math/Trigonometry.h"

#include <cmath>

namespace Ath::Forma
{
    void FormaHum::setContext (Dsp::Context context)
    {
        c = context;
        filter.setContext(c);
        filter.setCutoffFrequency(5000.0f);

        delta = 50.0f * c.T;
    }

    Simd::float8 FormaHum::process()
    {
        phase += delta;
        phase -= std::floor(phase);

        float s = Math::sin2pi9 (phase);
        s += Math::sin2pi9 (phase * 5.0f) * Math::DB_MINUS30;
        s += Math::sin2pi9 (phase * 7.0f) * Math::DB_MINUS24;
        s += Math::sin2pi9 (phase * 11.0f) * Math::DB_MINUS18;
        s += Math::sin2pi9 (phase * 13.0f) * Math::DB_MINUS30;
        s += Math::sin2pi9 (phase * 17.0f) * Math::DB_MINUS24;
        s += Math::sin2pi9 (phase * 100.0f) * Math::DB_MINUS48;
        //s /= Math::DB_MINUS18;

        Simd::float8 noise = {
            rng.getFloatBipolar(),
            rng.getFloatBipolar(),
            rng.getFloatBipolar(),
            rng.getFloatBipolar(),
            rng.getFloatBipolar(),
            rng.getFloatBipolar(),
            0.0f,
            0.0f
        };

        return filter.process(noise) + (Simd::float8(s) & Simd::mask6);
    }
}
