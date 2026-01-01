#pragma once

#include <cmath>
#include <cstdlib>
#include <numbers>

namespace Ath::Math
{
    // ============================================================
    // BASIC FUNCTIONS
    // ============================================================

    template <typename T> 
    static inline T sign(T val)
    {   auto cond = val >= T(0);
        return T(cond) - T(!cond);
    }

    template <typename T> 
    static inline T sign0(T val)
    {   auto condP = val > T(0);
        auto condN = val < T(0);
        return T(condP) - T(condN);
    }

    template <typename T>
    static inline T abs (T x)
    {
        return (x >= 0) ? x : -x;
    }

    template <typename T>
    static inline T trunc (T x)
    {
        return static_cast<int>(x);
    }

    template <typename T>
    static inline T frac (T x)
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

    // ============================================================
    // POWERS
    // ============================================================

    template<typename T>
    static inline T quad(T t) { return t * t; }

    template<typename T>
    static inline T cube(T t) { return t * t * t; }

    template<typename T>
    static inline T quart(T t) { return t * t * t * t; }

    template<typename T>
    static inline T quint(T t) { return t * t * t * t * t; }

    template<typename T>
    static inline T quadInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quad(x1);
    }

    template<typename T>
    static inline T cubeInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - cube(x1);
    }

    template<typename T>
    static inline T quartInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quart(x1);
    }

    template<typename T>
    static inline T quintInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quint(x1);
    }

    template <typename T>
    constexpr T ipow(T a, int n) 
    {
        if (n == 0) return T(1.0);
        if (n < 0)  return T(1.0) / ipow(a, -n);

        if (n % 2 == 0) 
        {
            T half = ipow(a, n / 2);
            return half * half;
        }

        return a * ipow(a, n - 1);
    }

    template <typename T, int N>
    constexpr T ipow(T a) 
    {
        if constexpr (N == 0) return T(1.0);
        if constexpr (N < 0)  return T(1.0) / ipow(a, -N);

        if constexpr (N % 2 == 0) 
        {
            T half = ipow(a, N / 2);
            return half * half;
        }

        return a * ipow(a, N - 1);
    }

    template<typename T, int N>
    static inline T ipowInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - ipow<N>(x1);
    }

    // ============================================================
    // INTERPOLATION
    // ============================================================

    template <typename T>
    T logerp2 (T a, T b, T x)
    {
        const T log = std::log2(b / a);
        return a * std::pow(T(2), log * x);
    }

    template <typename T>
    T logerp10 (T a, T b, T x)
    {
        const T log = std::log10(b / a);
        return a * std::pow(T(10), log * x);
    }

    // ============================================================
    // TRIGONOMETRY
    // ============================================================

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
        x = frac(x + half) - half;

        return max(min(x, half - x), -half - x);
    }

    template <typename T>
    static inline T sin2piParabola(T x) noexcept
    {
        const T x1 = foldArgument(x);
        const T x2 = x1 * x1;
        const T xabs = abs(x1);

        return (xabs * 4.0 - x2 * 8.0) * 2.0 * sign(x1);
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
        const T d = -70.9934332720751750562132689396061123;

        return x1 * (a + x2 * (b + x2 * (c + d * x2)));
        
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

    template <typename T>
    static inline T dirichlet(T x, int n) noexcept
    {
        const auto x1 = foldArgument(x);

        const auto cos = sin2pi9(x1 + 0.25);
        const auto cos2 = cos * cos;

        if (abs(x1) < 0.01) return 1;

        return sin2pi9(x1 * n) / (x * T(n) * pi<T>);
    }

    /**
        * @brief Fast tan over -pi/2 ... +pi/2 range
        */
    template <typename T>
    static inline T fastTan(T x) noexcept
    {
        const T x2 = x * x;

        const T a = -135135.0;
        const T b = 17325.0;
        const T c = -378.0;
        const T d = 62370.0;
        const T e = -3150.0;
        const T f = 28.0;

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

    template <typename T>
    static inline T chebyshev_nplus1(T x, T t_n, T t_nminus1) noexcept
    {
        return x * t_n * T(2.0) - t_nminus1;
    }

    // ============================================================
    // NOTE / FREQUENCY
    // ============================================================

    static constexpr int C1_MIDI_NOTE_NUMBER = 24;
    static constexpr int C2_MIDI_NOTE_NUMBER = C1_MIDI_NOTE_NUMBER + 12;
    static constexpr int A4_MIDI_NOTE_NUMBER = 69;

    template <typename T>
    T noteToFrequency (int p, T referenceFrequency = T(440))
    {
        return referenceFrequency
             * std::pow(T(2), T(p - A4_MIDI_NOTE_NUMBER) / T(12));
    }

    template <typename T>
    T noteToFrequency (T p, T referenceFrequency = T(440))
    {
        return referenceFrequency
             * std::pow(T(2), (p - T(A4_MIDI_NOTE_NUMBER)) / T(12));
    }

    template <typename T>
    T frequencyToNote (T freq, T referenceFrequency = T(440))
    {
        return T(A4_MIDI_NOTE_NUMBER)
             + T(12) * std::log2(freq / referenceFrequency);
    }

    template <typename T>
    T semitonesToFrequencyRatio (T semitones)
    {
        return std::pow(T(2), semitones / T(12));
    }

    // ============================================================
    // dB CONSTANTS
    // ============================================================

    constexpr float dB(int dB)
    {
        constexpr float base = 1.1220184543019633; // 10^(1/20)
        return ipow(base, dB);
    }

    static constexpr float DB_MINUS1  = 0.891250938f;
    static constexpr float DB_MINUS2  = 0.794328235f;
    static constexpr float DB_MINUS3  = 0.707945784f;
    static constexpr float DB_MINUS6  = 0.501187234f;
    static constexpr float DB_MINUS9  = DB_MINUS3  * DB_MINUS6;
    static constexpr float DB_MINUS12 = DB_MINUS6  * DB_MINUS6;
    static constexpr float DB_MINUS18 = DB_MINUS12 * DB_MINUS6;
    static constexpr float DB_MINUS24 = DB_MINUS18 * DB_MINUS6;
    static constexpr float DB_MINUS30 = DB_MINUS24 * DB_MINUS6;
    static constexpr float DB_MINUS36 = DB_MINUS30 * DB_MINUS6;
    static constexpr float DB_MINUS42 = DB_MINUS36 * DB_MINUS6;
    static constexpr float DB_MINUS48 = DB_MINUS42 * DB_MINUS6;
    static constexpr float DB_MINUS54 = DB_MINUS48 * DB_MINUS6;
    static constexpr float DB_MINUS60 = DB_MINUS54 * DB_MINUS6;
    static constexpr float DB_MINUS66 = DB_MINUS60 * DB_MINUS6;
    static constexpr float DB_MINUS72 = DB_MINUS66 * DB_MINUS6;
    static constexpr float DB_MINUS78 = DB_MINUS72 * DB_MINUS6;
    static constexpr float DB_MINUS84 = DB_MINUS78 * DB_MINUS6;

    static constexpr float DB_MINUS50 = 0.003162278f;

    static constexpr float DB_PLUS1 = 1.0f / DB_MINUS1;
    static constexpr float DB_PLUS2 = 1.0f / DB_MINUS2;
    static constexpr float DB_PLUS3 = 1.0f / DB_MINUS3;

    // ============================================================
    // AMPLITUDE / dB CONVERSIONS
    // ============================================================

    template <typename T>
    T amplitudeToDecibels (T gain)
    {
        return T(20) * std::log10(gain);
    }

    template <typename T>
    T decibelsToAmplitude (T db)
    {
        return std::pow(T(10), db / T(20));
    }

    template <typename T>
    T amplitudeRatioToDecibels (T ratio)
    {
        return T(10) * std::log10(ratio);
    }

    template <typename T>
    T amplitudeRatioToDecibels (T v1, T v2)
    {
        return T(10) * std::log10(v1 / v2);
    }

    template <typename T>
    T decibelsToAmplitudeRatio (T db)
    {
        return std::pow(T(10), db / T(10));
    }

    template <typename T>
    T linearVolumeToLog (T x, T dB_at0)
    {
        const T dB = std::lerp(dB_at0, T(0), x);
        return decibelsToAmplitude(dB);
    }
    
}