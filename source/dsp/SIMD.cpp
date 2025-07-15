#include "SIMD.h"

vec4 vec4fromFloats (float a, float b, float c, float d)
{
    vec4 y;

    y[0] = a;
    y[1] = b;
    y[2] = c;
    y[3] = d;

    return y;
}
