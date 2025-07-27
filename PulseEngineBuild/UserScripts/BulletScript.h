#pragma once
#include "IScripts.h"
#include "Common/dllExport.h"
#include <vector>
#include "PulseEngine/core/Math/Vector.h"


struct ExposedVariable;

class PULSE_ENGINE_DLL_API BulletScript : public IScript
{
    public:
        BulletScript() { 
            AddExposedVariable(EXPOSE_VAR(speed, INT)); 
            REGISTER_VAR(speed);
        }
        virtual void OnStart() override;
        virtual void OnUpdate() override;    
        virtual const char* GetName() const override;

        int speed = 1;
        PulseEngine::Vector3 direction;
};
