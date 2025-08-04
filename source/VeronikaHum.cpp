#include "VeronikaHum.h"

#include "math/Conversion.h"
#include "math/Trigonometry.h"

#include <cmath>

namespace Electrophilia::Veronika
{

    void VeronikaHum::setContext (Dsp::Context context)
    {
        c = context;
        filter.setContext (c);
        filter.setCutoffFrequency (5000.0f);

        delta = 50.0f * c.T;
    }

    vec4 VeronikaHum::process()
    {
        phase += delta;
        phase -= floor (phase);

        ///*
        float s = Math::sin2piParabola (phase);
        s += Math::sin2piParabola (phase * 5.0f) * Math::DB_MINUS30;
        s += Math::sin2piParabola (phase * 7.0f) * Math::DB_MINUS24;
        s += Math::sin2piParabola (phase * 11.0f) * Math::DB_MINUS18;
        s += Math::sin2piParabola (phase * 13.0f) * Math::DB_MINUS30;
        s += Math::sin2piParabola (phase * 17.0f) * Math::DB_MINUS24;
        s += Math::sin2piParabola (phase * 100.0f) * Math::DB_MINUS24;

        float d = Math::dirichlet (phase, 400) * Math::DB_MINUS12;
        //*/

        return filter.process (rng.getVec4Bipolar()) * Math::DB_MINUS12 + s + d;
    }
}
