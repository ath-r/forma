#pragma once

namespace Ath::Math
{
    /**
    * @brief Exponent at -1 < x < 1. Error at -1 is 2.26e-4. Error at 1 is -1.76e-4.
    */
    template <typename T>
    static T exp1(T x)
    {
        const T a6 = 0.001388888922519743;
        const T a5 = 0.008333335876047611;
        const T a4 = 0.04166664984893799;
        const T a3 = 0.1666666567325592;
        const T a2 = 0.4999999701976776;
        const T a1 = 0.9999999403953552;
        const T a0 = 1.0;

        return a0 + x * (a1 + x * (a2 + x * (a3 + x * (a4 + x * (a5 + x * a6)))));
    }
}
