#pragma once

#include "Clamp.h"

namespace Electrophilia::Math
{

    float fastmod1f (float x) { return x - static_cast<int> (x); }

    float fmax (float a, float b)
    {
        float r;
#ifdef __arm__
        asm ("vmaxnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
#else
        r = (a > b) ? a : b;
#endif // __arm__
        return r;
    }

    float fmin (float a, float b)
    {
        float r;
#ifdef __arm__
        asm ("vminnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
#else
        r = (a < b) ? a : b;
#endif // __arm__
        return r;
    }

    float fclamp (float in, float min, float max) { return fmin (fmax (in, min), max); }
}
