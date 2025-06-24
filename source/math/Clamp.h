#pragma once

namespace Electrophilia::Math
{
    #ifndef BASICS
    template <typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    float fastmod1f (float x);

    /** efficient floating point min/max
    c/o stephen mccaul
    */
    float fmax (float a, float b);

    float fmin (float a, float b);

    /** quick fp clamp
    */
    float fclamp (float in, float min, float max);

    #endif
}
