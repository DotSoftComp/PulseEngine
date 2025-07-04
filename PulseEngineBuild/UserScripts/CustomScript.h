#pragma once
#include "IScripts.h"
#include "Common/dllExport.h"
#include <vector>

struct ExposedVariable;

class MA_DLL_API CustomScript : public IScript
{
    public:
        CustomScript() { 
            AddExposedVariable(EXPOSE_VAR(speed, INT)); 
            REGISTER_VAR(speed);
        }
        virtual void OnStart() override;
        virtual void OnUpdate() override;    
        virtual const char* GetName() const override;

        int speed = 1;
};
