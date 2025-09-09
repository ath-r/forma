#pragma once

#include <cstdlib>

namespace Ath::Waveshaper
{
    class ADAA1
    {
    public:

        static constexpr double TOL = 1.0e-5;

        void reset()
        {
            x1 = 0.0;
            ad1_x1 = 0.0;
        }

        inline double process (double x) noexcept
        {
            bool illCondition = std::abs (x - x1) < TOL;
            double ad1_x = nonlinearityAntiderivative (x);

            double y = illCondition ?
                nonlinearity (0.5 * (x + x1)) :
                (ad1_x - ad1_x1) / (x - x1);

            ad1_x1 = ad1_x;
            x1 = x;

            return y;
        }

    protected:
        virtual inline double nonlinearity (double x) const noexcept = 0;
        virtual inline double nonlinearityAntiderivative (double x) const noexcept = 0;

        double x1 = 0.0;
        double ad1_x1 = 0.0;
    };
}
