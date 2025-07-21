#pragma once

namespace Electrophilia::Math
{
    class RNG
    {
        int seed = 0;

        static const int a = 1664525;
        static const int c = 1013904223;

    public:
        void setSeed(int newSeed);

        int getInt();

        float getFloat();
    };
}
