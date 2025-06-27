#ifndef INTERFACE_EDITOR_H
#define INTERFACE_EDITOR_H

#include "Common/common.h"
#include "Common/dllExport.h"
#include <unordered_map>
#include <string>

class PulseEngineBackend;
class TopBar;
class Entity;

class MA_DLL_API InterfaceEditor
{
private:
friend class TopBar;
TopBar* topbar;
Entity* selectedEntity = nullptr;
std::unordered_map<std::string, bool> windowStates;

public:
    InterfaceEditor(PulseEngineBackend* engine);

    void Render(PulseEngineBackend *engine);
    void EngineConfigWindow(PulseEngineBackend *engine);
    void EntityAnalyzerWindow();
    void GenerateSceneDataWindow(PulseEngineBackend *engine);
    void Viewport(PulseEngineBackend *engine);
    ~InterfaceEditor() {}
};


#endif