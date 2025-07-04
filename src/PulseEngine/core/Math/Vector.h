#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Common/dllExport.h"

namespace PulseEngine
{
    struct MA_DLL_API Vector2
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
    };
}

#endif
