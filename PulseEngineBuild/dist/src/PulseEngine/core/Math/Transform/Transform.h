#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Common/common.h"

namespace PulseEngine 
{
    class Transform 
    {
    public:
        PulseEngine::Vector3 position; // Position in 3D space
        PulseEngine::Vector3 rotation; // Rotation in degrees (pitch, yaw, roll)
        PulseEngine::Vector3 scale;    // Scale factors for each axis

        Transform(const PulseEngine::Vector3& pos = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), 
                  const PulseEngine::Vector3& rot = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), 
                  const PulseEngine::Vector3& scl = PulseEngine::Vector3(1.0f, 1.0f, 1.0f));

        // Transformation operations
        PulseEngine::Vector3 Apply(const PulseEngine::Vector3& point) const;

        // Direction vectors
        PulseEngine::Vector3 GetForward() const;
        PulseEngine::Vector3 GetUp() const;
        PulseEngine::Vector3 GetRight() const;
    };
}

#endif
