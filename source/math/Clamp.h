#pragma once

namespace Electrophilia::Math
{
    #ifndef BASICS
    template <typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    static inline T fastmod1f (T x)
    {
        return x - static_cast<int>(x);
    };

    template <typename T>
    static inline T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    static inline T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    static inline T clamp(T x, T a, T b)
    {
        return min(max(x, a), b);
    }

    #endif
}
