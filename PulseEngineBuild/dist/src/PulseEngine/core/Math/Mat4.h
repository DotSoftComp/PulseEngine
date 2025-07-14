#ifndef MAT4PULSE_H
#define MAT4PULSE_H

#include <cstring> 

namespace PulseEngine
{
    struct PULSE_ENGINE_DLL_API Mat4
    {
        float data[4][4]; 

        Mat4()
        {
            SetIdentity();
        }

        // Constructor that sets the diagonal to a scalar value
        Mat4(float scalar)
        {
            memset(data, 0, sizeof(data));
            for (int i = 0; i < 4; ++i)
            {
                data[i][i] = scalar;
            }
        }
        

        void SetIdentity()
        {
            memset(data, 0, sizeof(data));
            for (int i = 0; i < 4; ++i)
            {
                data[i][i] = 1.0f;
            }
        }

        float* operator[](int row)
        {
            return data[row];
        }

        const float* operator[](int row) const
        {
            return data[row];
        }

        const float* Ptr() const
        {
            return &data[0][0];
        }

        float* Ptr()
        {
            return &data[0][0];
        }

        Mat4 operator*(const Mat4& other) const
        {
            Mat4 result(0.0f);
            for (int row = 0; row < 4; ++row)
            {
                for (int col = 0; col < 4; ++col)
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        result.data[row][col] += data[row][i] * other.data[i][col];
                    }
                }
            }
            return result;
        }

        Vector3 MultiplyPoint(const Vector3& v) const
        {
            float x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3];
            float y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3];
            float z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3];
            float w = data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3];

            if (w != 0.0f && w != 1.0f)
            {
                x /= w;
                y /= w;
                z /= w;
            }
            return Vector3(x, y, z);
        }

        Vector3 MultiplyVector(const Vector3& v) const
        {
            float x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z;
            float y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z;
            float z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z;
            return Vector3(x, y, z);
        }


        static Mat4 CreateTranslation(const Vector3& t)
        {
            Mat4 result(1.0f);
            result[0][3] = t.x;
            result[1][3] = t.y;
            result[2][3] = t.z;
            return result;
        }
        
        static Mat4 CreateScale(const Vector3& s)
        {
            Mat4 result(1.0f);
            result[0][0] = s.x;
            result[1][1] = s.y;
            result[2][2] = s.z;
            return result;
        }
        
        static Mat4 CreateFromEulerAngles(float pitch, float yaw, float roll)
        {
            // Create rotation matrices for each axis
            float cp = cosf(pitch), sp = sinf(pitch);
            float cy = cosf(yaw), sy = sinf(yaw);
            float cr = cosf(roll), sr = sinf(roll);
        
            Mat4 rx(1.0f);
            rx[1][1] = cp; rx[1][2] = -sp;
            rx[2][1] = sp; rx[2][2] = cp;
        
            Mat4 ry(1.0f);
            ry[0][0] = cy; ry[0][2] = sy;
            ry[2][0] = -sy; ry[2][2] = cy;
        
            Mat4 rz(1.0f);
            rz[0][0] = cr; rz[0][1] = -sr;
            rz[1][0] = sr; rz[1][1] = cr;
        
            return rz * ry * rx; // Note: order matters!
        }

    };
}


#endif