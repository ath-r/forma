#pragma once

namespace Ath::Math 
{
    template<typename T>
    struct complex
    {
        T re = 0.0f;
        T im = 0.0f;

        inline T operator+(complex rhs) const noexcept {return { re + rhs.re, im + rhs.im};}
        inline T operator-(complex rhs) const noexcept {return { re - rhs.re, im + rhs.im}; }
        inline T operator*(complex rhs) const noexcept 
        {
            return { 
                re * rhs.re - im * rhs.im, 
                re * rhs.im + im * rhs.re 
            };
        }
        inline T operator/(complex rhs) const noexcept 
        {
            T denom = rhs.re * rhs.re + rhs.im * rhs.im;

            return 
            {
                (re * rhs.re + im * rhs.im) / denom,
                (im * rhs.re - re * rhs.im) / denom
            };
        }
    };
}