#ifndef INTERFACE_EDITOR_H
#define INTERFACE_EDITOR_H

#include "Common/common.h"
#include "Common/dllExport.h"

class PulseEngineBackend;
class TopBar;
class Entity;

class PULSE_ENGINE_DLL_API InterfaceEditor
{
private:
TopBar* topbar;
Entity* selectedEntity = nullptr;

public:
    InterfaceEditor(PulseEngineBackend* engine);

    void Render(PulseEngineBackend *engine);
    void EntityAnalyzerWindow();
    void GenerateSceneDataWindow(PulseEngineBackend *engine);
    void Viewport(PulseEngineBackend *engine);
    ~InterfaceEditor() {}
};


#endif