#pragma once
#include "IScripts.h"
#include "dist/src/dllexport/dllExport.h"

class PULSE_ENGINE_DLL_API CustomScript : public IScript
{
    public:
        virtual void OnStart() override;
        virtual void OnUpdate() override;    
        virtual const char* GetName() const override;
};
