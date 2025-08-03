#pragma once

#include <array>

namespace Electrophilia::Math
{
    template <typename T, int N>
    struct Polynomial
    {
        std::array<T, N> coefficients;

        T evaluate(T x) const
        {
            T y = coefficients[0];

            for (int i = 0; i < N; i++)
            {
                y = y * x + coefficients[i];
            }

            return y;
        }

        Polynomial<T, N + 1> antiderivative() const
        {
            Polynomial<T, N + 1> p;

            p.coefficients[N] = 0.0;

            for (int n = 0; n < N; n++)
            {
                int i = N - 1 - n;

                p.coefficients[i] = coefficients[i] * (1.0 / (n + 1.0));
            }

            return p;
        }
    };
}
