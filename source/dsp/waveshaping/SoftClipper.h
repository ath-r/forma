#pragma once

#include "ADAA1simd.h"
#include "../../math/Simd.h"

namespace Ath::Dsp::Waveshaper
{
    template <int Exp, typename T>
    constexpr T ipow(T base) {
        if constexpr (Exp == 0) return T(1);
        else if constexpr (Exp < 0) return T(1) / ipow<-Exp>(base);
        else if constexpr (Exp % 2 == 0) {
            T half = ipow<Exp/2>(base);
            return half * half;
        } else {
            return base * ipow<Exp-1>(base);
        }
    }

    template<int N, typename T>
    class SoftClipperSimd : public ADAA1simd<T>
    {
        const T k = std::pow(1.0f / float(N), 1.0f / (float(N) - 1.0f));
        const T l = f(k);
        const T rl = T(1.0f) / l;
        const T rkl = rl / k;

        const T rn1 = T(1.0f) / (T(N) + 1.0f);

        inline T f(T x) const { return x - ipow<N>(x); }
        inline T f2(T x) const { return (x * x) * 0.5f - ipow<N + 1>(x) * rn1; }

        inline T nonlinearity (T x) const noexcept override
        {
            const T y0 = -1.0f;
            const T y1 = f(x * k) * rl;
            const T y2 = 1.0f;

            return Simd::ternary(Simd::ternary(y0, y1, x < -1.0f), y2, x < 1.0f);
        }

        inline T nonlinearityAntiderivative (T x) const noexcept override
        {
            static const T offset = f2(k) * rkl - 1.0f;

            const T y0 = -x + offset;
            const T y1 = f2(x * k) * rkl;
            const T y2 = x + offset;

            return Simd::ternary(Simd::ternary(y0, y1, x < -1.0f), y2, x < 1.0f);
        }
    };
}