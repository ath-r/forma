#pragma once

#include "Clamp.h"

namespace Electrophilia::Math
{

    float fastmod1f (float x) { return x - static_cast<int> (x); }

}
