#pragma once

namespace Ath::Dsp
{
    struct Context
    {
        float SR;
        float T;

        Context();

        Context(float sampleRate);
    };
}
