#pragma once

namespace Electrophilia::Dsp
{
    struct Context
    {
        float SR;
        float T;

        Context(float sampleRate);
    };
}
