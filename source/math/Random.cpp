#include "Random.h"

namespace Electrophilia::Math
{
    void RNG::setSeed(int newSeed)
    {
        seed = newSeed;
    };

    int RNG::getInt()
    {
        seed = seed * a + c;
        return seed;
    };

    float RNG::getFloat()
    {
        return float(getInt()) * 4.6566129e-10f;
    };


}
