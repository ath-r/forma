#pragma once

#include <cmath>
#include <limits>
#include <numbers>
namespace Electrophilia::Math
{


    #ifndef BASICS
    template <typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    inline float fastmod1f(float x)
    {
        return x - static_cast<int>(x);
    }

    /** efficient floating point min/max
    c/o stephen mccaul
    */
    inline float fmax(float a, float b)
    {
        float r;
    #ifdef __arm__
        asm("vmaxnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
    #else
        r = (a > b) ? a : b;
    #endif // __arm__
        return r;
    }

    inline float fmin(float a, float b)
    {
        float r;
    #ifdef __arm__
        asm("vminnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
    #else
        r = (a < b) ? a : b;
    #endif // __arm__
        return r;
    }

    /** quick fp clamp
    */
    inline float fclamp(float in, float min, float max)
    {
        return fmin(fmax(in, min), max);
    }

    #endif

    #ifndef TRIGONOMETRY

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
            auto x2 = x * x;
            auto numerator = x * (-135135 + x2 * (17325 + x2 * (-378 + x2)));
            auto denominator = -135135 + x2 * (62370 + x2 * (-3150 + 28 * x2));
            return numerator / denominator;
        }

    #endif

    #ifndef CONVERSIONS

        static constexpr int C1_MIDI_NOTE_NUMBER = 24;
        static constexpr int A4_MIDI_NOTE_NUMBER = 69;

        static float noteToFrequency(const int p, float referenceFrequency = 440.0f)
        {
            return referenceFrequency * std::pow (2.0f, static_cast<float>(p - A4_MIDI_NOTE_NUMBER) / 12.0);
        }

        static float frequencyToNote(float freq, float referenceFrequency = 440.0f)
        {
            return A4_MIDI_NOTE_NUMBER + 12.0f * std::log2(freq / referenceFrequency);
        }

        static float semitonesToFrequencyRatio(float semitones)
        {
            return std::pow(2.0f, semitones / 12.0f);
        }

        static float amplitudeToDecibels(float gain)
        {
            return 20.0f * std::log10(gain);
        }

        static float decibelsToAmplitude(float db)
        {
            return std::pow(10.0f, db / 20.0f);
        }

        static float amplitudeRatioToDecibels(float ratio)
        {
            return 10.0f * std::log10(ratio);
        }

        static float amplitudeRatioToDecibels(float v1, float v2)
        {
            return 10.0f * std::log10(v1 / v2);
        }

        static float decibelsToAmplitudeRatio(float db)
        {
            return std::pow(10.0f, db / 10.0f);
        }

    #endif

    #ifndef RANDOM

        static inline int fastRandomInteger()
        {
            static int lastValue = 0;
            lastValue = lastValue * 1664525 + 1013904223;
            return lastValue;
        }

        static inline float fastRandomFloat()
        {
            return static_cast<float>(fastRandomInteger()) * 4.6566129e-10f;
        }

        static inline float fastRandomNormal()
        {
            const float uniform = fastRandomFloat();
            const float uniform3 = uniform * uniform * uniform;
            const float uniform6 = uniform3 * uniform3;
            const float uniform13 = uniform6 * uniform6 * uniform;

            return (fma(uniform3, 0.6666667, uniform) + uniform13) * 0.333;
        }

    #endif
}
