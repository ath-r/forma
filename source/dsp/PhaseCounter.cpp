#pragma once

#include "PhaseCounter.h"

namespace Ath::Dsp
{
    void PhaseCounter::setContext(Context context)
    {
        c = context;

        delta = 1.0 / double(c.SR);
    }

    double PhaseCounter::processSample()
    {
        seconds += delta;

        return seconds;
    }

    double PhaseCounter::getTime() { return seconds; }

    void PhaseCounter::reset() { seconds = 0.0; }
}
