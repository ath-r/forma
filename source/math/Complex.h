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
        complex<T>(T x) : re(x), im(0.0f) {}

        complex<T>& operator=(const complex<T> rhs) { re = rhs.re; im = rhs.im; return *this; };

        // Complex-Complex operations
        inline complex<T> operator + (const complex<T>& rhs) const noexcept { 
            return { re + rhs.re, im + rhs.im}; 
        }
        
        inline complex<T> operator - (const complex<T>& rhs) const noexcept { 
            return { re - rhs.re, im - rhs.im}; 
        }
        
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

        // Complex-Scalar operations
        inline complex<T> operator + (T rhs) const noexcept { 
            return { re + rhs, im }; 
        }
        
        inline complex<T> operator - (T rhs) const noexcept { 
            return { re - rhs, im }; 
        }
        
        inline complex<T> operator * (T rhs) const noexcept { 
            return { re * rhs, im * rhs }; 
        }
        
        inline complex<T> operator / (T rhs) const noexcept { 
            return { re / rhs, im / rhs }; 
        }

        // Scalar-Complex operations (as friend functions)
        friend inline complex<T> operator + (T lhs, const complex<T>& rhs) noexcept { 
            return { lhs + rhs.re, rhs.im }; 
        }
        
        friend inline complex<T> operator - (T lhs, const complex<T>& rhs) noexcept { 
            return { lhs - rhs.re, -rhs.im }; 
        }
        
        friend inline complex<T> operator * (T lhs, const complex<T>& rhs) noexcept { 
            return { lhs * rhs.re, lhs * rhs.im }; 
        }
        
        friend inline complex<T> operator / (T lhs, const complex<T>& rhs) noexcept { 
            T denom = rhs.re * rhs.re + rhs.im * rhs.im;
            return 
            {
                (lhs * rhs.re) / denom,
                (-lhs * rhs.im) / denom
            };
        }

        // Compound assignment operators for complex
        inline complex<T>& operator += (const complex<T>& rhs) noexcept { 
            re += rhs.re; im += rhs.im; return *this; 
        }
        
        inline complex<T>& operator -= (const complex<T>& rhs) noexcept { 
            re -= rhs.re; im -= rhs.im; return *this; 
        }
        
        inline complex<T>& operator *= (const complex<T>& rhs) noexcept { 
            *this = *this * rhs; return *this; 
        }
        
        inline complex<T>& operator /= (const complex<T>& rhs) noexcept { 
            *this = *this / rhs; return *this; 
        }

        // Compound assignment operators for scalars
        inline complex<T>& operator += (T rhs) noexcept { 
            re += rhs; return *this; 
        }
        
        inline complex<T>& operator -= (T rhs) noexcept { 
            re -= rhs; return *this; 
        }
        
        inline complex<T>& operator *= (T rhs) noexcept { 
            re *= rhs; im *= rhs; return *this; 
        }
        
        inline complex<T>& operator /= (T rhs) noexcept { 
            re /= rhs; im /= rhs; return *this; 
        }

        // Unary operators
        inline complex<T> operator + () const noexcept { 
            return *this; 
        }
        
        inline complex<T> operator - () const noexcept { 
            return { -re, -im }; 
        }

        // Comparison operators (optional but useful)
        inline bool operator == (const complex<T>& rhs) const noexcept { 
            return re == rhs.re && im == rhs.im; 
        }
        
        inline bool operator != (const complex<T>& rhs) const noexcept { 
            return !(*this == rhs); 
        }
        
        inline bool operator == (T rhs) const noexcept { 
            return re == rhs && im == T(0); 
        }
        
        inline bool operator != (T rhs) const noexcept { 
            return !(*this == rhs); 
        }
        
        friend inline bool operator == (T lhs, const complex<T>& rhs) noexcept { 
            return rhs == lhs; 
        }
        
        friend inline bool operator != (T lhs, const complex<T>& rhs) noexcept { 
            return rhs != lhs; 
        }
    };
}