#include "Context.h"

namespace Electrophilia::Dsp
{
    Context::Context(float sampleRate) : SR(sampleRate), T(1.0f / SR) {};
}
