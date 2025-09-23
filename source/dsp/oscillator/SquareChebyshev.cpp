#include "SquareChebyshev.h"
#include "../../math/Trigonometry.h"

namespace Ath::Dsp::Oscillator
{
    void SquareChebyshev::setContext (const Context context)
    {
        c = context;

        setFrequency (frequency);
    }

    void SquareChebyshev::setFrequency (const float newFrequency)
    {
        frequency = newFrequency;

        delta = frequency * c.T;

        n = Math::truncate (15000.0f / frequency);
        n = Math::max(n, 1.0f);
    }

    void SquareChebyshev::setTime (const double time)
    {
        const float t = float(time) * frequency;

        phase = t - Math::truncate (t);
    }

    float SquareChebyshev::processSample()
    {
        phase += delta;
        phase = Math::fastmod1f(phase);

        const double x = Math::sin2pi9(phase);

        double cheb_iminus1 = 1.0;
        double cheb_i = x;
        double cheb_iplus1;

        double sum = cheb_i;

        constexpr double falloff_start_frequency = 14500.0;
        constexpr double falloff_end_frequency = 15000.0;
        constexpr double falloff_a = -1.0 / (falloff_end_frequency - falloff_start_frequency);
        constexpr double falloff_b = 1 - falloff_start_frequency * falloff_a;

        const double falloff_freq = frequency * falloff_a;
        double falloff;

        static const double mult[4] = {0.0, 1.0, 0.0, -1.0};

        for (int i = 2; i < n; i++)
        {
            cheb_iplus1 = 2.0 * cheb_i * x - cheb_iminus1;

            cheb_iminus1 = cheb_i;
            cheb_i = cheb_iplus1;

            //falloff = Math::min(falloff_freq * i + falloff_b, 1.0);

            sum += cheb_i / double(i) * mult[i % 4];
        }

        return float(sum);
    }
}