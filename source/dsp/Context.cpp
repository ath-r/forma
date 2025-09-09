#include "Context.h"

namespace Ath::Dsp
{
    Context::Context(float sampleRate) : SR(sampleRate), T(1.0f / SR) {};
}
