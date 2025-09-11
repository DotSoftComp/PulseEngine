#pragma once

#include "IScripts.h"
#include "Common/dllExport.h"
class PULSE_ENGINE_DLL_API ComponentPierre : public IScript
{
    public:
        ComponentPierre()
        {
            // if you need to expose variables, do it here with these macro : 
            AddExposedVariable(EXPOSE_VAR(speed, INT));
            REGISTER_VAR(speed); //both are needed to make the variable exposed in the editor
        }
        void OnStart() override;
        void OnUpdate() override;
        void OnRender() override;
        const char* GetName() const override;

        float speed = 5.0f;
};

