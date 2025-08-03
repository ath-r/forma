#include "Random.h"

namespace Electrophilia::Math::Random
{
    float RNG::getFloat() { return float (getInt()) * 4.6566129e-10f; }
    float RNG::getFloatBipolar() { return getFloat() * 2.0f - 1.0f; }
    float RNG::getFloatNormal() { return standardNormalDistributionCDF.evaluate (getFloat()); }

    vec4 RNG::getVec4()
    {
        vec4 v;

        for (int i = 0; i < 4; i++)
        {
            v[i] = getFloat();
        }

        return v;
    }
    vec4 RNG::getVec4Bipolar() { return getVec4() * 2.0f - 1.0f; }
    vec4 RNG::getVec4Normal() { return standardNormalDistributionCDFvec4.evaluate (getVec4()); }

    void LCG::setSeed(int newSeed) { seed = newSeed; }
    int LCG::getInt() { seed = seed * a + c; return seed;}

    void Mersenne::setSeed (int newSeed) { rng.seed (newSeed); }
    int Mersenne::getInt() { return rng(); }
}
