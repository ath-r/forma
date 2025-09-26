#pragma once

namespace Ath::Math
{   
    template<typename T>
    static inline T easeQuad(T t) { return t * t; }
    template<typename T>
    static inline T easeCubic(T t) { return t * t * t; }
    template<typename T>
    static inline T easeQuint(T t) { return t * t * t * t * t; }

    template<typename T>
    static inline T easeOutQuad(T t)
    {
        T x1 = T(1.0f) - t;
        T x2 = x1 * x1;

        return T(1.0f) - x2;
    }

    template<typename T>
    static inline T easeOutCubic(T t)
    {
        T x1 = T(1.0f) - t;
        T x3 = x1 * x1 * x1;

        return T(1.0f) - x3;
    }

    template<typename T>
    static inline T easeOutQuint(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - easeQuint(x1);
    }

    template<typename F, typename T>
    static inline T easerp(T a, T b, T t)
    {
        T d = b - a;
        return a + F(t) * d;
    }
}