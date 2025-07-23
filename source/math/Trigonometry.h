#pragma once

#include <numbers>

#include "Clamp.h"

namespace Electrophilia::Math
{
    template <typename T> static constexpr T pi = std::numbers::pi_v<T>;
    template <typename T> static constexpr T tau = pi<T> * T(2.0);

    template <typename T> static constexpr T invPi = T(1.0) / pi<T>;
    template <typename T> static constexpr T invTau = T(1.0) / tau<T>;

    static constexpr float fpi = pi<float>;
    static constexpr float ftau = pi<float> * 2.0f;

    static constexpr float finvPi = invPi<float>;
    static constexpr float finvTau = invTau<float>;

    template <typename T>
    static inline T foldArgument(T x)
    {
        const T half = 0.5;
        x = fastmod1f(x + half) - half;

        return max(min(x, half - x), -half - x);
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit floats max. measured abs error is 6.78301e-05 (-80 dB). It's more precise than Juce' pade approximation
        */
    template <typename T>
    static inline T sin2pi5(T x) noexcept
    {
        const T x1 = foldArgument(x);
        const T x2 = x1 * x1;

        const T a = 6.2812800766220821491468958126456729;
        const T b = -41.0952426871208970211323332525800187;
        const T c = 73.5855147347551640956688672796423323;

        return x1 * (a + x2 * (b + c * x2));
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit floats max. measured abs error is 8.34465e-07 (-121 dB).
        */
    template <typename T>
    static inline T sin2pi7(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        const T a = 6.28316404430247135671540270030948533;
        const T b = -41.3371423711001029236311900250633048;
        const T c = 81.3407688876640676542096535737693472;
        const T d = 70.9934332720751750562132689396061123;

        return x1 * (a + x2 * (b + x2 * (c - d * x2)));
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit float max. measured abs error is 3.72529e-07 (-128 dB).
        */
    template <typename T>
    static inline T sin2pi9(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        const T a = 6.28318516008947744301885339855754539;
        const T b = -41.3416550314162780771649724741397745;
        const T c = 81.6010040732617735242889484141942461;
        const T d = -76.5497822935957426856648840708956891;
        const T e = 39.5367060657302079906898367421553316;

        return x1 * (a + x2 * (b + x2 * (c + x2 * (d + x2 * e))));
    }

    /**
        * @brief Fast tan over -pi/2 ... +pi/2 range
        */
    template <typename T>
    static inline T fastTan(T x) noexcept
    {
        const T x2 = x * x;

        const T a = -135135;
        const T b = 17325;
        const T c = -378;
        const T d = 62370;
        const T e = -3150;
        const T f = 28;

        const T numerator = x * (a + x2 * (b + x2 * (c + x2)));
        const T denominator = a + x2 * (d + x2 * (e + f * x2));
        return numerator / denominator;
    }


    template <typename T>
    static inline T chebyshev2(T x) noexcept
    {
        // 2x^2 - 1

        return x * x * 2.0 - 1.0;
    }

    template <typename T>
    static inline T chebyshev3(T x) noexcept
    {
        // 4x^3 - 3x

        return x * x * x * 4.0 - x * 3.0;
    }

    template <typename T>
    static inline T chebyshev5(T x) noexcept
    {
        // 16x^5 - 20x^3 + 5x

        const T x2 = x * x;
        const T x3 = x2 * x;
        const T x5 = x3 * x2;

        return x5 * 16.0 - x3 * 20 + x * 5;
    }

    template <typename T>
    static inline T chebyshev7(T x) noexcept
    {
        // 64x^7 - 112x^5 + 56x^3 - 7x

        const T x2 = x * x;
        const T x3 = x2 * x;
        const T x5 = x3 * x2;
        const T x7 = x5 * x2;

        return x7 * 64.0 - x5 * 112.0 + x3 * 56.0 - x * 7.0;
    }
}
