#pragma once

/*
    This is a cut-down version of https://github.com/devoln/Simd with (supposedly) improved readability and structure
*/
#include <immintrin.h>
#include <type_traits>

#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64)) && !defined(__x86_64__)
    #define __x86_64__ 1
#endif

#define SIMD_SSE_LEVEL_NONE 0
#define SIMD_SSE_LEVEL_SSE 1
#define SIMD_SSE_LEVEL_SSE2 2
#define SIMD_SSE_LEVEL_SSE3 3
#define SIMD_SSE_LEVEL_SSSE3 4
#define SIMD_SSE_LEVEL_SSE4_1 5
#define SIMD_SSE_LEVEL_SSE4_2 6
#define SIMD_SSE_LEVEL_AVX 7
#define SIMD_SSE_LEVEL_AVX2 8

#define SIMD_SSE_LEVEL SIMD_SSE_LEVEL_AVX2

#ifdef _MSC_VER
    #define forceinline __forceinline
    #if(defined(SIMD_SSE_LEVEL) && SIMD_SSE_LEVEL != 0)
        #define SIMD_VECTORCALL __vectorcall
    #else
        #define SIMD_VECTORCALL
    #endif
#elif defined(__GNUC__)
    #define forceinline __inline__ __attribute__((always_inline))
    #define SIMD_VECTORCALL
#else
    #define forceinline inline
    #define SIMD_VECTORCALL
#endif

namespace Simd
{

    struct int4
    {
        __m128i vec;

        enum {VectorSize = 4};

        forceinline int4() = default;
        forceinline int4(int x, int y, int z, int w) noexcept: vec(_mm_set_epi32(w, z, y, x)) {}
        forceinline int4(int* p) noexcept: vec(_mm_load_si128(reinterpret_cast<__m128i*>(p))) {}
        forceinline int4(int x) noexcept: vec(_mm_set1_epi32(x)) {}

        forceinline int4 SIMD_VECTORCALL operator+(int4 rhs) const noexcept {return _mm_add_epi32(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator-(int4 rhs) const noexcept {return _mm_sub_epi32(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator*(int4 rhs) const noexcept
        {
            #if(SIMD_SSE_LEVEL >= SIMD_SSE_LEVEL_SSE4_1)
                    return _mm_mullo_epi32(vec, rhs.vec);
            #else
                    __m128i tmp1 = _mm_mul_epu32(vec, rhs.vec);
                    __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(vec, 4), _mm_srli_si128(rhs.vec, 4));
                    return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0, 0, 2, 0)));
            #endif
        }

        forceinline int4 SIMD_VECTORCALL operator/(int4 rhs) const
        {
            alignas(16) int a[4], b[4];
            _mm_store_si128(reinterpret_cast<__m128i*>(a), vec);
            _mm_store_si128(reinterpret_cast<__m128i*>(b), rhs.vec);
            return {a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]};
        }

        forceinline int4 SIMD_VECTORCALL operator&(int4 rhs) const noexcept {return _mm_and_si128(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator|(int4 rhs) const noexcept {return _mm_or_si128(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator^(int4 rhs) const noexcept {return _mm_xor_si128(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator~() const noexcept {return _mm_xor_si128(vec, _mm_set1_epi32(-1));}

        forceinline int4 SIMD_VECTORCALL operator<<(int bits) const noexcept {return _mm_slli_epi32(vec, bits);}
        forceinline int4 SIMD_VECTORCALL operator>>(int bits) const noexcept {return _mm_srai_epi32(vec, bits);}

        forceinline int4 SIMD_VECTORCALL operator<<(int4 rhs) const noexcept
        {
            #if(SIMD_SSE_LEVEL >= SIMD_SSE_LEVEL_AVX2)
                return _mm_srlv_epi32(vec, rhs.vec);
            #else
                alignas(16) int a[4], b[4];
                _mm_store_si128(reinterpret_cast<__m128i*>(a), vec);
                _mm_store_si128(reinterpret_cast<__m128i*>(b), rhs.vec);
                return {a[0] << b[0], a[1] << b[1], a[2] << b[2], a[3] << b[3]};
            #endif
        }

        forceinline int4 SIMD_VECTORCALL operator>>(int4 rhs) const noexcept
        {
        #if(SIMD_SSE_LEVEL >= SIMD_SSE_LEVEL_AVX2)
            return _mm_srav_epi32(vec, rhs.vec);
        #else
            alignas(16) int a[4], b[4];
            _mm_store_si128(reinterpret_cast<__m128i*>(a), vec);
            _mm_store_si128(reinterpret_cast<__m128i*>(b), rhs.vec);
            return {a[0] >> b[0], a[1] >> b[1], a[2] >> b[2], a[3] >> b[3]};
        #endif
        }

        forceinline int4 SIMD_VECTORCALL operator>(int4 rhs) const noexcept {return _mm_cmpgt_epi32(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator<(int4 rhs) const noexcept {return _mm_cmplt_epi32(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator>=(int4 rhs) const noexcept {return ~operator<(rhs);}
        forceinline int4 SIMD_VECTORCALL operator<=(int4 rhs) const noexcept {return ~operator>(rhs);}
        forceinline int4 SIMD_VECTORCALL operator==(int4 rhs) const noexcept {return _mm_cmpeq_epi32(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator!=(int4 rhs) const noexcept {return ~operator==(rhs);}

        forceinline int operator[](int i) const
        {
            alignas(16) int arr[4];
            _mm_store_si128(reinterpret_cast<__m128i*>(arr), vec);
            return arr[i];
        }

        forceinline int4(__m128i v) noexcept: vec(v) {}
        forceinline operator __m128i() const {return vec;}
    };

    struct float4
    {
        __m128 vec;

        enum {VectorSize = 4};

        forceinline float4() = default;
        forceinline float4(float x, float y, float z, float w) noexcept: vec(_mm_set_ps(w, z, y, x)) {}
        forceinline float4(float* p) noexcept: vec(_mm_load_ps(p)) {}
        forceinline float4(float x) noexcept: vec(_mm_set1_ps(x)) {}

        forceinline float4 SIMD_VECTORCALL operator+(float4 rhs) const noexcept {return _mm_add_ps(vec, rhs.vec);}
        forceinline float4 SIMD_VECTORCALL operator-(float4 rhs) const noexcept {return _mm_sub_ps(vec, rhs.vec);}
        forceinline float4 SIMD_VECTORCALL operator*(float4 rhs) const noexcept {return _mm_mul_ps(vec, rhs.vec);}
        forceinline float4 SIMD_VECTORCALL operator/(float4 rhs) const {return _mm_div_ps(vec, rhs.vec);}

        forceinline int4 SIMD_VECTORCALL operator>(float4 rhs) const {return _mm_cmpgt_ps(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator<(float4 rhs) const {return _mm_cmplt_ps(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator>=(float4 rhs) const {return _mm_cmpge_ps(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator<=(float4 rhs) const {return _mm_cmple_ps(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator==(float4 rhs) const noexcept {_mm_cmpeq_ps(vec, rhs.vec);}
        forceinline int4 SIMD_VECTORCALL operator!=(float4 rhs) const noexcept {_mm_cmpneq_ps(vec, rhs.vec);}

        forceinline explicit float4(const int4& v) noexcept: vec(_mm_castsi128_ps(v.vec)) {}
        forceinline explicit operator int4() const noexcept {return _mm_castps_si128(vec);}

        forceinline float operator[](int i) const
        {
            alignas(16) float arr[4];
            _mm_store_ps(arr, vec);
            return arr[i];
        }

        forceinline operator __m128() const noexcept {return vec;}
        forceinline float4(__m128 v) noexcept: vec(v) {}
    };

    struct int8
    {
        __m256i vec;

        enum {VectorSize = 8};

        forceinline int8() = default;
        forceinline int8(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) noexcept:
            vec(_mm256_set_epi32(x8, x7, x6, x5, x4, x3, x2, x1)) {}
        forceinline int8(int* p) noexcept: vec(_mm256_load_si256(reinterpret_cast<__m256i*>(p))) {}
        forceinline int8(int x) noexcept: vec(_mm256_set1_epi32(x)) {}

        forceinline int8 SIMD_VECTORCALL operator+(int8 rhs) const noexcept {return _mm256_add_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator-(int8 rhs) const noexcept {return _mm256_sub_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator*(int8 rhs) const noexcept {return _mm256_mul_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator/(int8 rhs) const
        {
            alignas(32) int a[8], b[8];
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(a), vec);
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(b), rhs.vec);
            return {a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3], a[4] / b[4], a[5] / b[5], a[6] / b[6], a[7] / b[7]};
        }

        forceinline int8 SIMD_VECTORCALL operator&(int8 rhs) const noexcept {return _mm256_and_si256(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator|(int8 rhs) const noexcept {return _mm256_or_si256(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator^(int8 rhs) const noexcept {return _mm256_xor_si256(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator~() const noexcept {return _mm256_xor_si256(vec, _mm256_set1_epi32(-1));}

        forceinline int8 SIMD_VECTORCALL operator<<(int bits) const noexcept {return _mm256_slli_epi32(vec, bits);}
        forceinline int8 SIMD_VECTORCALL operator>>(int bits) const noexcept {return _mm256_srai_epi32(vec, bits);}

        forceinline int8 SIMD_VECTORCALL operator<<(int8 rhs) const noexcept {return _mm256_srlv_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator>>(int8 rhs) const noexcept {return _mm256_srav_epi32(vec, rhs.vec);}


        forceinline int8 SIMD_VECTORCALL operator>(int8 rhs) const noexcept {return _mm256_cmpgt_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator<(int8 rhs) const noexcept {return rhs > *this;}
        forceinline int8 SIMD_VECTORCALL operator>=(int8 rhs) const noexcept {return ~(rhs > *this);}
        forceinline int8 SIMD_VECTORCALL operator<=(int8 rhs) const noexcept {return ~operator>(rhs);}
        forceinline int8 SIMD_VECTORCALL operator==(int8 rhs) const noexcept {return _mm256_cmpeq_epi32(vec, rhs.vec);}
        forceinline int8 SIMD_VECTORCALL operator!=(int8 rhs) const noexcept {return ~operator==(rhs);}

        forceinline int operator[](int i) const
        {
            alignas(32) int arr[8];
            _mm256_store_si256(reinterpret_cast<__m256i*>(arr), vec);
            return arr[i];
        }

        forceinline int8(__m256i v) noexcept: vec(v) {}
        forceinline operator __m256i() const {return vec;}
    };

    struct float8
    {
        __m256 vec;

        enum {VectorSize = 8};

        forceinline float8() = default;
        forceinline float8(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8) noexcept:
            vec(_mm256_set_ps(x8, x7, x6, x5, x4, x3, x2, x1)) {}
        forceinline float8(float* p) noexcept: vec(_mm256_load_ps(p)) {}
        forceinline float8(float x) noexcept: vec(_mm256_set1_ps(x)) {}        

        forceinline float8 SIMD_VECTORCALL operator+(float8 rhs) const noexcept {return _mm256_add_ps(vec, rhs.vec);}
        forceinline float8 SIMD_VECTORCALL operator-(float8 rhs) const noexcept {return _mm256_sub_ps(vec, rhs.vec);}
        forceinline float8 SIMD_VECTORCALL operator*(float8 rhs) const noexcept {return _mm256_mul_ps(vec, rhs.vec);}
        forceinline float8 SIMD_VECTORCALL operator/(float8 rhs) const {return _mm256_div_ps(vec, rhs.vec);}

        forceinline float8 SIMD_VECTORCALL operator+=(const float8 rhs) noexcept {vec = _mm256_add_ps(vec, rhs.vec); return *this;}
        forceinline float8 SIMD_VECTORCALL operator-=(float8 rhs) noexcept {vec = _mm256_sub_ps(vec, rhs.vec); return *this;}
        forceinline float8 SIMD_VECTORCALL operator*=(float8 rhs) noexcept {vec = _mm256_mul_ps(vec, rhs.vec); return *this;}
        forceinline float8 SIMD_VECTORCALL operator/=(float8 rhs) {vec = _mm256_div_ps(vec, rhs.vec); return *this;}

        forceinline float8 SIMD_VECTORCALL operator&(float8 rhs) const noexcept {return _mm256_and_si256(vec, rhs.vec);}
        forceinline float8 SIMD_VECTORCALL operator&(int8 rhs) const noexcept {return _mm256_and_si256(vec, float8(rhs.vec));}

    #if(SIMD_SSE_LEVEL >= SIMD_SSE_LEVEL_AVX2)
        forceinline int8 SIMD_VECTORCALL operator>(float8 rhs) const {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_GT_OQ)));}
        forceinline int8 SIMD_VECTORCALL operator<(float8 rhs) const {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_LT_OQ)));}
        forceinline int8 SIMD_VECTORCALL operator>=(float8 rhs) const {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_GE_OQ)));}
        forceinline int8 SIMD_VECTORCALL operator<=(float8 rhs) const {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_LE_OQ)));}
        forceinline int8 SIMD_VECTORCALL operator==(float8 rhs) const noexcept {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_EQ_OQ)));}
        forceinline int8 SIMD_VECTORCALL operator!=(float8 rhs) const noexcept {return int8(float8(_mm256_cmp_ps(vec, rhs.vec, _CMP_NEQ_OQ)));}

        forceinline explicit float8(const int8& v): vec(_mm256_castsi256_ps(v.vec)) {}
        forceinline explicit operator int8() const noexcept {return _mm256_castps_si256(vec);}
    #endif

        forceinline float operator[](int i) const
        {
            alignas(32) float arr[8];
            _mm256_store_ps(arr, vec);
            return arr[i];
        }

        forceinline operator __m256() const noexcept {return vec;}
        forceinline float8(__m256 v) noexcept: vec(v) {}
    };

    /* #region TYPE  PROPERTIES */
    /// Scalar type on which vector type is based
    template <class T>
    using scalarTypeOf = decltype (T()[0] + 0);

    /// Int vector type that corresponds to given vector type
    template <class T>
    using intAnalogOf = decltype (T() < T());

    /// Returns the number of scalar elements of a given vectors' type
    template <typename T>
    static constexpr int numberOfElements (T v)
    {
        return sizeof (v) / sizeof (v[0]);
    }
    /* #endregion */

    /* #region BLEND */

    template<typename T>
    static forceinline T blend(T a, T b, intAnalogOf<T> mask)
    {
        return (a & mask) + (b & ~mask);
    }
    /* #endregion */

    /* #region ROUNDING */

    template<typename T> forceinline 
    std::enable_if_t<std::is_same<scalarTypeOf<T>, float>::value, T> SIMD_VECTORCALL abs(T v) noexcept
    {
        return T(intAnalogOf<T>(v) & 0x7FFFFFFF);
    }

    forceinline float4 floor(float4 x) { return _mm_floor_ps(x); }
    forceinline float8 floor(float8 x) { return _mm256_floor_ps(x); }

    forceinline float4 ceil(float4 x) { return _mm_ceil_ps(x); }
    forceinline float8 ceil(float8 x) { return _mm256_ceil_ps(x); }

    forceinline float4 round(float4 x) { return _mm_round_ps(x, _MM_FROUND_TO_NEAREST_INT); }
    forceinline float8 round(float8 x) { return _mm256_round_ps(x, _MM_FROUND_TO_NEAREST_INT); }

    forceinline float4 trunc(float4 x) { return _mm_round_ps(x, _MM_FROUND_TO_ZERO); }
    forceinline float8 trunc(float8 x) { return _mm256_round_ps(x, _MM_FROUND_TO_ZERO); }

    template<typename T> 
    forceinline T mod1f(T x) noexcept
    {
        return x - floor(x);
    }

    template<typename T> 
    forceinline T fract(T x) noexcept
    {
        return x - floor(x);
    }
    /* #endregion */

    template<typename T> 
    forceinline T sign(T x) noexcept
    {
        intAnalogOf<T> mask = x >= T(0.0f);
        
        return blend(T(1.0f), T(-1.0f), mask);
    }

    forceinline float4 fma(float4 a, float4 b, float4 c) { return _mm_fmadd_ps(a, b, c); }
    forceinline float8 fma(float8 a, float8 b, float8 c) { return _mm256_fmadd_ps(a, b, c); }

    /* #region MIN AND MAX */
    
    forceinline float4 min (float4 a, float4 b) noexcept { return _mm_min_ps(a, b); }
    forceinline float8 min (float8 a, float8 b) noexcept { return _mm256_min_ps(a, b); }

    forceinline float4 max (float4 a, float4 b) noexcept { return _mm_max_ps(a, b); }
    forceinline float8 max (float8 a, float8 b) noexcept { return _mm256_max_ps(a, b); }

    /* #endregion */

    /* #region SHUFFLE AND PERMUTATION*/
        
        static inline float8 permute (float8 v, int8 indices) noexcept
        {
            return _mm256_permutevar8x32_ps(v, indices);
        }
    /* #endregion */

}