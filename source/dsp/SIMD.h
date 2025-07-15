#pragma once

#include <juce_dsp/juce_dsp.h>

//Maths:


using vec4 = juce::dsp::SIMDRegister<float>;
using vec4i = juce::dsp::SIMDRegister<int>;

vec4 vec4fromFloats (float a, float b, float c, float d);

//Packed 32-bit integers to packed 32-bit floats:
#if defined(__i386__) || defined(__amd64__) || defined(_M_X64) || defined(_X86_) || defined(_M_IX86)
inline vec4 float4 (const vec4i a)
{
    return _mm_cvtepi32_ps(a.value);
}


#elif defined(_M_ARM64) || defined (__arm64__) || defined (__aarch64__)
inline vec4 float4 (const vec4i a)
{
    return vdivq_f32(l.value, r.value);
}

#else
#error "SIMD register support not implemented for this platform"
#endif

//Division for SIMDRegisters:
#if defined(__i386__) || defined(__amd64__) || defined(_M_X64) || defined(_X86_) || defined(_M_IX86)
inline vec4 operator / (const vec4& l, const vec4& r)
{
    return _mm_div_ps(l.value, r.value);
}


#elif defined(_M_ARM64) || defined (__arm64__) || defined (__aarch64__)
inline vec4 operator / (const vec4& l, const vec4& r)
{
    return vdivq_f32(l.value, r.value);
}

#else
#error "SIMD register support not implemented for this platform"
#endif

//Functions:
namespace SIMDf
{
    static inline vec4 tan(vec4 x)
    {
        auto x2 = x * x;

#define fma(x, y, z) vec4::multiplyAdd(z, y, x) // fixes ugly order of arguments in juce::SIMDRegister::multiplyAdd
        auto numerator = fma(fma(x2 - 378, x2, 17325), x2, -135135) * x;
        auto denominator = fma(fma(fma(x2, 28, -3150), x2, 62370), x2, -135135);
        return numerator / denominator;
#undef fma
    }

}
