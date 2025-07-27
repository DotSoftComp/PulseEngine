#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "PulseEngine/core/Math/Mat4.h"
#include "PulseEngine/core/Math/Vector.h"
#include <cmath>

namespace PulseEngine
{
    /**
     * @brief Utility functions for mathematical operations.
     * 
     */
    namespace MathUtils
    {
        constexpr float PI = 3.14159265358979323846f;

        /**
         * @brief Convert degrees to radians.
         * 
         * @param degrees the angle in degrees.
         * @return float the angle in radians.
         */
        inline float ToRadians(float degrees)
        {
            return degrees * (PI / 180.0f);
        }

        /**
         * @brief Convert radians to degrees.
         * 
         * @param radians the angle in radians.
         * @return float the angle in degrees.
         */
        inline float ToDegrees(float radians)
        {
            return radians * (180.0f / PI);
        }

        /**
         * @brief Create a rotation matrix from Euler angles (in radians).
         * 
         * @param euler the Euler angles in radians (pitch, yaw, roll).
         * @return Mat4 the rotation matrix.
         */
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

        /**
         * @brief Make an object rotate, to look at a certain position.
         * 
         * @param position of the object
         * @param target we want to look at
         * @param up let it be the default up vector (0,1,0) or change it to another one.
         * @return Vector3 This function returns a Vector3 that represents the direction the object should face.
         */
        Vector3 LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f))
        {
            Vector3 zaxis = (position - target).Normalized(); // Forward vector
            Vector3 xaxis = up.Cross(zaxis).Normalized();    // Right vector
            Vector3 yaxis = zaxis.Cross(xaxis);              // Up vector

            return Vector3(xaxis.x, yaxis.y, zaxis.z);
        }

        /**
         * @brief Give an easy to use function to rotate an point around a target point in a 3D space with no complexity.
         * 
         * @param target The point to orbit around.
         * @param yaw The yaw angle in degrees.
         * @param pitch The pitch angle in degrees.
         * @param radius The distance from the target.
         * @return Vector3 The new position after rotation.
         */
        inline Vector3 RotateAround(const Vector3& target, float yaw, float pitch, float radius)
        {
            float yawRad = ToRadians(yaw);
            float pitchRad = ToRadians(pitch);

            Vector3 offset;
            offset.x = radius * cosf(pitchRad) * cosf(yawRad);
            offset.y = radius * sinf(pitchRad);
            offset.z = radius * cosf(pitchRad) * sinf(yawRad);

            return target - offset;
        }
    }
}


#endif