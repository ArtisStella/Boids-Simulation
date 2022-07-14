#pragma once
#include "pi.h"
#include <math.h>
#include <stdint.h>

namespace my {
    template <class Num_T>
    class Vector2 {
    public:
        Vector2();
        Vector2(Num_T x, Num_T y);
        Vector2(const Vector2& other);
        Vector2(Num_T value);
        ~Vector2() {}

    public:
        Vector2 operator+(const Vector2& rhs) const;
        Vector2 operator-(const Vector2& rhs) const;
        Vector2 operator*(Num_T value) const;
        float operator*(const Vector2& rhs) const;

        Vector2& operator+=(const Vector2& rhs);
        Vector2& operator-=(const Vector2& rhs);
        Vector2& operator*=(Num_T value);

        bool operator==(const Vector2& rhs) const;
        bool operator!=(const Vector2& rhs) const;

        operator Vector2<int32_t>() const;
        operator Vector2<uint32_t>() const;
        operator Vector2<float>() const;

    public:
        Vector2 Rotate(float angle) const;
        void RotateIp(float angle);

        float AngleTo(const Vector2& other) const;
        float AngleTo(Num_T x, Num_T y) const;

        Vector2<float> Normalize() const;
        void NormalizeIp();

        float Dot(const Vector2& other) const;
        float Dot(Num_T x, Num_T y) const;

        float DistanceTo(const Vector2& other) const;
        float DistanceTo(Num_T x, Num_T y) const;

        float Length() const;
        float LengthSquared() const;

    public:
        Num_T x;
        Num_T y;
    };

    template <class Num_T>
    Vector2<Num_T>::Vector2() {
        x = (Num_T)0;
        y = (Num_T)0;
    }
    template <class Num_T>
    Vector2<Num_T>::Vector2(Num_T x, Num_T y) {
        this->x = x;
        this->y = y;
    }
    template <class Num_T>
    Vector2<Num_T>::Vector2(const Vector2& other) {
        x = (Num_T)other.x;
        y = (Num_T)other.y;
    }
    template <class Num_T>
    Vector2<Num_T>::Vector2(Num_T value) {
        x = value;
        y = value;
    }

    template <class Num_T>
    Vector2<Num_T> Vector2<Num_T>::operator+(const Vector2& rhs) const {
        return { x + rhs.x, y + rhs.y };
    }
    template <class Num_T>
    Vector2<Num_T> Vector2<Num_T>::operator-(const Vector2& rhs) const {
        return { x - rhs.x, y - rhs.y };
    }
    template <class Num_T>
    Vector2<Num_T> Vector2<Num_T>::operator*(Num_T value) const {
        return { x * value, y * value };
    }
    template <class Num_T>
    float Vector2<Num_T>::operator*(const Vector2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    template <class Num_T>
    Vector2<Num_T>& Vector2<Num_T>::operator+=(const Vector2& rhs) {
        return *this = *this + rhs;
    }
    template <class Num_T>
    Vector2<Num_T>& Vector2<Num_T>::operator-=(const Vector2& rhs) {
        return *this = *this - rhs;
    }
    template <class Num_T>
    Vector2<Num_T>& Vector2<Num_T>::operator*=(Num_T value) {
        return *this = *this * value;
    }

    template <class Num_T>
    bool Vector2<Num_T>::operator==(const Vector2<Num_T>& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    template <class Num_T>
    bool Vector2<Num_T>::operator!=(const Vector2<Num_T>& rhs) const {
        return !(*this == rhs);
    }
    template <class Num_T>
    Vector2<Num_T>::operator Vector2<int32_t>() const {
        return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) };
    }
    template <class Num_T>
    Vector2<Num_T>::operator Vector2<uint32_t>() const {
        return { static_cast<uint32_t>(this->x), static_cast<uint32_t>(this->y) };
    }
    template <class Num_T>
    Vector2<Num_T>::operator Vector2<float>() const {
        return { static_cast<float>(this->x), static_cast<float>(this->y) };
    }

    template <class Num_T>
    Vector2<Num_T> Vector2<Num_T>::Rotate(float angle) const {
        angle = fmod(angle, 360.f);
        angle = DEG2RAD(angle);
        float sinAngle = sin(angle);
        float cosAngle = cos(angle);

        float tmpX = (cosAngle * x - sinAngle * y);
        float tmpY = (sinAngle * x + cosAngle * y);
        return Vector2<Num_T>(tmpX, tmpY);
    }
    template <class Num_T>
    void Vector2<Num_T>::RotateIp(float angle) {
        Vector2<float> newV = Rotate(angle);
        x = newV.x;
        y = newV.y;
    }

    template <class Num_T>
    float Vector2<Num_T>::AngleTo(const Vector2& other) const {
        return AngleTo(other.x, other.y);
    }
    template <class Num_T>
    float Vector2<Num_T>::AngleTo(Num_T x, Num_T y) const {
        return RAD2DEG(atan2(y, x) - atan2(this->y, this->x));
    }

    template <class Num_T>
    Vector2<float> Vector2<Num_T>::Normalize() const {
        float s = Length();
        if (s == 0.f) {
            return { 0.f, 0.f };
        }
        return { x / s, y / s };
    }
    template <class Num_T>
    void Vector2<Num_T>::NormalizeIp() {
        Vector2<float> newV = Normalize();
        x = newV.x;
        y = newV.y;
    }

    template <class Num_T>
    float Vector2<Num_T>::Dot(const Vector2& other) const {
        return Dot(other.x, other.y);
    }
    template <class Num_T>
    float Vector2<Num_T>::Dot(Num_T x, Num_T y) const {
        return (this->x * x) + (this->y * y);
    }

    template <class Num_T>
    float Vector2<Num_T>::DistanceTo(const Vector2& other) const {
        return DistanceTo(other.x, other.y);
    }
    template <class Num_T>
    float Vector2<Num_T>::DistanceTo(Num_T x, Num_T y) const {
        return sqrtf(powf(this->x - x, 2.0f) + powf(this->y - y, 2.0f));
    }

    template <class Num_T>
    float Vector2<Num_T>::Length() const {
        return sqrtf(LengthSquared());
    }
    template <class Num_T>
    float Vector2<Num_T>::LengthSquared() const {
        return x * x + y * y;
    }

    typedef Vector2<int32_t> Vector2i;
    typedef Vector2<uint32_t> Vector2u;
    typedef Vector2<float> Vector2f;
}
