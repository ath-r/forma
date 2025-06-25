#pragma once

#include <numbers>
#include <cmath>

namespace Electrophilia::Math
{
    template <typename T> static constexpr T pi = std::numbers::pi_v<T>;
    template <typename T> static constexpr T tau = pi<T> * T(2.0);

    static constexpr float fpi = pi<float>;
    static constexpr float ftau = pi<float> * 2.0f;

    template <typename T>
    static inline T foldArgument(T x)
    {
        const T half = T(0.5);
        x = fastmod1f(x + half) - half;

        return std::max(std::min(x, half - x), -half - x);
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit floats max. measured abs error is 6.78301e-05 (-80 dB). It's more precise than Juce' pade approximation
        */
    template <typename T>
    static inline T sin2pi5(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        return x1*(6.2812800766220821491468958126456729 + x2*(-41.0952426871208970211323332525800187 + 73.5855147347551640956688672796423323*x2));
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit floats max. measured abs error is 8.34465e-07 (-121 dB).
        */
    template <typename T>
    static inline T sin2pi7(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        return x1*(6.28316404430247135671540270030948533 + x2*(-41.3371423711001029236311900250633048 + x2*(81.3407688876640676542096535737693472 - 70.9934332720751750562132689396061123*x2)));
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input. With 32-bit float max. measured abs error is 3.72529e-07 (-128 dB). This is as precise as this type of approximation gets within 32-bit floats.
        */
    template <typename T>
    static inline T sin2pi9(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        return x1*(6.28318516008947744301885339855754539 + x2*(-41.3416550314162780771649724741397745 + x2*(81.6010040732617735242889484141942461 + x2*(-76.5497822935957426856648840708956891 + 39.5367060657302079906898367421553316*x2))));
    }

    /**
        * @brief Fast tan over -pi/2 ... +pi/2 range
        */
    template <typename T>
    static inline T fastTan(T x) noexcept
    {
        T x2 = x * x;
        T numerator = x * (-135135 + x2 * (17325 + x2 * (-378 + x2)));
        T denominator = -135135 + x2 * (62370 + x2 * (-3150 + 28 * x2));
        return numerator / denominator;
    }
}
