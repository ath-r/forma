#pragma once

namespace Ath::Math 
{
    template<typename T>
    struct complex
    {
        T re = 0.0f;
        T im = 0.0f;

        complex<T>(T r = 0.0f, T i = 0.0f) : re(r), im(i) {}
        complex<T>(const complex<T>&) = default;
        complex<T>& operator=(const complex<T> rhs) { re = rhs.re; im = rhs.im; return *this; };

        inline complex<T> operator + (const complex<T>& rhs) const noexcept { return { re + rhs.re, im + rhs.im}; }
        inline complex<T> operator - (const complex<T>& rhs) const noexcept { return { re - rhs.re, im - rhs.im}; }
        inline complex<T> operator * (const complex<T>& rhs) const noexcept 
        {
            return { 
                re * rhs.re - im * rhs.im, 
                re * rhs.im + im * rhs.re 
            };
        }
        inline complex<T> operator/(const complex<T>& rhs) const noexcept 
        {
            T denom = rhs.re * rhs.re + rhs.im * rhs.im;

            return 
            {
                (re * rhs.re + im * rhs.im) / denom,
                (im * rhs.re - re * rhs.im) / denom
            };
        }

        inline complex<T> operator += (const complex<T>& rhs) noexcept { *this = *this + rhs; return *this; }
        inline complex<T> operator -= (const complex<T>& rhs) noexcept { *this = *this - rhs; return *this; }
        inline complex<T> operator *= (const complex<T>& rhs) noexcept { *this = *this * rhs; return *this; }
        inline complex<T> operator /= (const complex<T>& rhs) noexcept { *this = *this / rhs; return *this; }
    };
}