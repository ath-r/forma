#pragma once

#include <random>

#include "Polynomial.h"

namespace Ath::Math::Random
{
    class RNG
    {
    public:

        Polynomial<float, 14> standardNormalDistributionCDF =
        {
            {
                8.47155472e+05, -5.44607728e+06,  1.55758263e+07, -2.61329842e+07,
                2.85596620e+07, -2.13472861e+07,  1.11525726e+07, -4.09061521e+06,
                1.04357194e+06, -1.80971771e+05,  2.05359845e+04, -1.44748702e+03,
                6.36541186e+01, -2.89386774e+00
            }
        };

        virtual int getInt() = 0;

        float getFloat();

        float getFloatBipolar();

        float getFloatNormal();
    };

    class LCG : public RNG
    {
        int seed = 0;

        static const int a = 1664525;
        static const int c = 1013904223;

    public:
        void setSeed(int newSeed);
        int getInt() override;
    };

    class Mersenne : public RNG
    {
        std::mt19937 rng = std::mt19937();

    public:
        void setSeed (int newSeed);
        int getInt() override;
    };
}
