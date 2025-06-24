#include "Random.h"
#include <cmath>

namespace Electrophilia::Math
{
    static int fastRandomInteger()
    {
        static int lastValue = 0;
        lastValue = lastValue * 1664525 + 1013904223;
        return lastValue;
    }

    static float fastRandomFloat() { return static_cast<float> (fastRandomInteger()) * 4.6566129e-10f; }

    static float fastRandomNormal()
    {
        const float uniform = fastRandomFloat();
        const float uniform3 = uniform * uniform * uniform;
        const float uniform6 = uniform3 * uniform3;
        const float uniform13 = uniform6 * uniform6 * uniform;

        return (fma (uniform3, 0.6666667, uniform) + uniform13) * 0.333;
    }

}
