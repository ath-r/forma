#include "Random.h"

namespace Ath::Math::Random
{
    float RNG::getFloat() { return std::abs(float (getInt()) * 4.6566129e-10f); }
    float RNG::getFloatBipolar() { return getFloat() * 2.0f - 1.0f; }
    float RNG::getFloatNormal() { return standardNormalDistributionCDF.evaluate (getFloat()); }

    void LCG::setSeed(int newSeed) { seed = newSeed; }
    int LCG::getInt() { seed = seed * a + c; return seed;}

    void Mersenne::setSeed (int newSeed) { rng.seed (newSeed); }
    int Mersenne::getInt() { return rng(); }
}
