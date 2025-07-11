#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Common/dllExport.h"

namespace PulseEngine
{
    struct PULSE_ENGINE_DLL_API Vector2
    {
        float x;
        float y;

        Vector2() : x(0), y(0) {}
        Vector2(float scalar) : x(scalar), y(scalar) {}
        Vector2(float x, float y) : x(x), y(y) {}
        Vector2(const Vector2& vect) : x(vect.x), y(vect.y) {}
    };

    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3(const Vector3& vect) : x(vect.x), y(vect.y), z(vect.z) {}
        
        float& operator[](int index)
        {
            switch (index)
            {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: throw std::out_of_range("Index out of range for Vector3");
            }
        }

        // Add assignment operator+=
        Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Add assignment operator-=
        Vector3& operator-=(const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Subtract two vectors
        Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        

        // Add two vectors
        Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // Multiply vector by scalar
        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        // Multiply scalar by vector (friend function)
        friend Vector3 operator*(float scalar, const Vector3& vec)
        {
            return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
        }



        Vector3 Normalized() const
        {
            float length = std::sqrt(x * x + y * y + z * z);
            if (length == 0.0f)
            throw std::runtime_error("Cannot normalize zero-length vector");
            return Vector3(x / length, y / length, z / length);
        }

        Vector3 Cross(const Vector3& other) const
        {
            return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
            );
        }

        float Dot(const Vector3& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }
    };
}

#endif
