
#pragma once

#include "Context.h"

namespace Electrophilia::Dsp
{
    class PhaseCounter
    {
        Context c = 48000.0f;

        double seconds = 0.0;
        double delta = 0.0;

    public:
        void setContext(Context context);

        double processSample();

        double getTime();

        void reset();
    };
}
