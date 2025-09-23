#include "Context.h"

namespace Ath::Dsp
{
    Context::Context() : SR(48000.0f), T(1.0f / 48000.0f) {};

    Context::Context(float sampleRate) : SR(sampleRate), T(1.0f / SR) {};
}
