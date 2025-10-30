#include "FormaHum.h"

#include "math/Conversion.h"
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

    float FormaHum::process()
    {
        phase += delta;
        phase -= std::floor(phase);

        float sin = Math::sin2pi9 (phase);
        float s = sin;
        s += Math::chebyshev5 (sin) * Math::DB_MINUS30;
        s += Math::chebyshev7 (sin) * Math::DB_MINUS24;
        s += Math::sin2pi9 (phase * 11.0f) * Math::DB_MINUS18;
        s += Math::sin2pi9 (phase * 13.0f) * Math::DB_MINUS30;
        s += Math::sin2pi9 (phase * 17.0f) * Math::DB_MINUS24;
        s += Math::sin2pi9 (phase * 100.0f) * Math::DB_MINUS48;
        //s /= Math::DB_MINUS18;

        auto noise = rng.getFloatBipolar();

        return filter.process(noise) + s;
    }
}
