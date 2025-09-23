#pragma once

namespace Ath::Math
{
    #ifndef BASICS
    template <typename T> 
    static inline T sign(T val)
    {   auto cond = val >= T(0);
        return T(cond) - T(!cond);
    }

    template <typename T>
    static inline T abs (T x)
    {
        return (x >= 0) ? x : -x;
    }

    template <typename T>
    static inline T truncate (T x)
    {
        return static_cast<int>(x);
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
