#ifndef TOPBAR_H
#define TOPBAR_H

#include "Common/common.h"
#include "Common/dllExport.h"

class PulseEngineBackend;

class MA_DLL_API TopBar
{
private:

public:
    friend class InterfaceEditor;
    TopBar() {}
    ~TopBar() {}

    void UpdateBar(PulseEngineBackend* engine, InterfaceEditor* editor);
    void BuildGameToWindow(PulseEngineBackend *engine, InterfaceEditor *editor);
    void CompileUserScripts(InterfaceEditor *editor);
    void GenerateExecutableForWindow(PulseEngineBackend *engine);
    void CopyDllForWindow();
    void CopyAssetForWindow();
    void GenerateWindowsDirectory();
};


#endif