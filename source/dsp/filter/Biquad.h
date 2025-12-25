#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

#include "../../math/Complex.h"
#include "FilterMath.h"

namespace Ath::Dsp::Filter::Biquad
{
    enum class BiquadTopology
    {
        DirectForm1,
        DirectForm2,
        TransposedDirectForm1,
        TransposedDirectForm2
    };

    template <typename T>
    struct DigitalBiquadCoefficients
    {
        T b0 = 1.0;
        T b1 = 0.0;
        T b2 = 0.0;
        T a0 = 1.0;
        T a1 = 0.0;
        T a2 = 0.0;
    };

    template <typename T>
    struct AnalogBiquadCoefficients //essentially the same class as above, semantics matter
    {
        T b0 = 1.0;
        T b1 = 0.0;
        T b2 = 0.0;
        T a0 = 1.0;
        T a1 = 0.0;
        T a2 = 0.0;
    };

    template <typename T>
    static Math::complex<T> transfer(DigitalBiquadCoefficients<T> coeffs, Math::complex<T> s, T sr)
    {
        auto k = sr * 2.0f;

        auto z1 = (k - s) / (k + s);
        auto z2 = z1 * z1;

        return (coeffs.b0 + coeffs.b1 * z1 + coeffs.b2 * z2) / (coeffs.a0 + coeffs.a1 * z1 + coeffs.a2 * z2);
    }

    template <typename T>
    static DigitalBiquadCoefficients<T> bilinear(AnalogBiquadCoefficients<T> in, double sr)
    {
        const auto k = sr * 2.0;
        const auto k2 = k * k;

        T a0 = in.a0 + in.a1 * k + in.a2 * k2;

        T b0 = (in.b0 + in.b1 * k + in.b2 * k2) / a0;
        T b1 = (in.b0 * 2.0 - in.b2 * 2.0 * k2) / a0;
        T b2 = (in.b0 - in.b1 * k + in.b2 * k2) / a0;

        T a1 = (in.a0 * 2.0 - in.a2 * 2.0 * k2) / a0;
        T a2 = (in.a0 - in.a1 * k + in.a2 * k2) / a0;

        return 
        {
            b0,
            b1,
            b2,

            1.0,
            a1,
            a2
        };
    }

    template <typename T, BiquadTopology Topology = BiquadTopology::DirectForm1>
    class Biquad
    {
        T y = 0.0;

        struct Empty {};

        static const bool isDF1 = Topology == BiquadTopology::DirectForm1;
        static const bool isTDF1 = Topology == BiquadTopology::TransposedDirectForm1;
        static const bool isDF2orTDF2 = Topology == BiquadTopology::DirectForm2 || Topology == BiquadTopology::TransposedDirectForm2;

        using DF1State = std::conditional_t<isDF1, T, Empty>;
        DF1State x1;
        DF1State x2;
        DF1State y1;
        DF1State y2;

        using TDF1State = std::conditional_t<isTDF1, T, Empty>;
        TDF1State s0;
        TDF1State s1;
        TDF1State s2;
        TDF1State s3;

        using DF2state = std::conditional_t<isDF2orTDF2, T, Empty>;
        DF2state v1;
        DF2state v2;

    public:

        DigitalBiquadCoefficients<T> coeffs;

        Biquad()
        {
            reset();
        }

        inline void setCoefficients(DigitalBiquadCoefficients<T> newCoeffs)
        {
            coeffs = newCoeffs;
        }

        inline T process(T x)
        {
            if constexpr(Topology == BiquadTopology::DirectForm1)
            {
                y = coeffs.b0 * x + coeffs.b1 * x1 + coeffs.b2 * x2 - coeffs.a1 * y1 - coeffs.a2 * y2;

                x2 = x1; 
                x1 = x;

                y2 = y1; 
                y1 = y;

                return y;
            }

            if constexpr(Topology == BiquadTopology::DirectForm2)
            {
                T w = x - coeffs.a1 * v1 - coeffs.a2 * v2;
                
                y = coeffs.b0 * w + coeffs.b1 * v1 + coeffs.b2 * v2;

                v2 = v1;
                v1 = w;

                return y;
            }

            if constexpr(Topology == BiquadTopology::TransposedDirectForm1)
            {
                y = s0 + s2 + coeffs.b0*x;

                s0 = s1 + coeffs.b1 * x;
                s1 = coeffs.b2 * x;
                s2 = s3 - coeffs.a1 * y;
                s3 = -coeffs.a2 * y;

                return y;
            }

            if constexpr(Topology == BiquadTopology::TransposedDirectForm2)
            {
                y = coeffs.b0 * x + v1;
                v1 = coeffs.b1 * x - coeffs.a1 * y + v2;
                v2 = coeffs.b2 * x - coeffs.a2 * y;

                return y;
            }
        }

        inline T last()
        {
            return y;
        }

        void reset()
        {
            y = 0.0;

            if constexpr(Topology == BiquadTopology::DirectForm1)
            {
                x1 = 0.0;
                x2 = 0.0;
                y1 = 0.0;
                y2 = 0.0;
            }

            if constexpr(Topology == BiquadTopology::TransposedDirectForm1)
            {
                s0 = 0.0;
                s1 = 0.0;
                s2 = 0.0;
                s3 = 0.0;
            }

            if constexpr(isDF2orTDF2)
            {
                v1 = 0.0;
                v2 = 0.0;
            }
           
        }
    };

    template <typename T, int N, BiquadTopology Topology = BiquadTopology::TransposedDirectForm2>
    class BiquadCascade
    {
        T y = 0.0;

    public:
        std::array<Biquad<T, Topology>, N> biquads;

        T process(T x)
        {
            y = x;

            for (auto& biquad : biquads)
            {
                y = biquad.process(y);
            }

            return y;
        }

        T last()
        {
            return y;
        }
    };
}