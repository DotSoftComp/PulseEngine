#ifndef MAT4PULSE_H
#define MAT4PULSE_H

#include "Common/common.h"
#include <cstring> 

namespace PulseEngine
{
    struct MA_DLL_API Mat4
    {
        float data[4][4]; 

        Mat4()
        {
            SetIdentity();
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
        
    };
}


#endif