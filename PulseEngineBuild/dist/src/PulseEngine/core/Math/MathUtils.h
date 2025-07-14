#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "PulseEngine/core/Math/Mat4.h"
#include "PulseEngine/core/Math/Vector.h"
#include <cmath>

namespace PulseEngine
{
    namespace MathUtils
    {
        constexpr float PI = 3.14159265358979323846f;

        inline float ToRadians(float degrees)
        {
            return degrees * (PI / 180.0f);
        }

        inline float ToDegrees(float radians)
        {
            return radians * (180.0f / PI);
        }

        Mat4 CreateRotationMatrix(const Vector3& euler)
        {
            float pitch = ToRadians(euler.x); // X-axis
            float yaw   = ToRadians(euler.y); // Y-axis
            float roll  = ToRadians(euler.z); // Z-axis
        
            float cx = cosf(pitch), sx = sinf(pitch);
            float cy = cosf(yaw),   sy = sinf(yaw);
            float cz = cosf(roll),  sz = sinf(roll);
        
            Mat4 mat;
        
            mat[0][0] = cy * cz;
            mat[0][1] = sx * sy * cz - cx * sz;
            mat[0][2] = cx * sy * cz + sx * sz;
            mat[0][3] = 0.0f;
        
            mat[1][0] = cy * sz;
            mat[1][1] = sx * sy * sz + cx * cz;
            mat[1][2] = cx * sy * sz - sx * cz;
            mat[1][3] = 0.0f;
        
            mat[2][0] = -sy;
            mat[2][1] = sx * cy;
            mat[2][2] = cx * cy;
            mat[2][3] = 0.0f;
        
            mat[3][0] = 0.0f;
            mat[3][1] = 0.0f;
            mat[3][2] = 0.0f;
            mat[3][3] = 1.0f;
        
            return mat;
        }

    }
}


#endif