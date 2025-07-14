#pragma once
#include "IScripts.h"
#include "Common/dllExport.h"
#include <vector>


struct ExposedVariable;

class PULSE_ENGINE_DLL_API CustomScript : public IScript
{
    public:
        CustomScript() { 
            AddExposedVariable(EXPOSE_VAR(speed, INT)); 
            AddExposedVariable(EXPOSE_VAR(life, FLOAT)); 
            REGISTER_VAR(speed);
        }
        virtual void OnStart() override;
        virtual void OnUpdate() override;    
        virtual const char* GetName() const override;

        int speed = 1;
        float life = 1;
        float spawnTimer = 0.0f;
        float spawnDelay = 1.0f;

        Entity* spawnedEntity = nullptr;
};
