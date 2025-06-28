#ifndef SCRIPTSLOADER_H
#define SCRIPTSLOADER_H

#include "Common/common.h"
#include "Common/dllExport.h"

#include <map>
#include <string>


class IScript;
typedef IScript* (*CreateScriptFunc)();

class MA_DLL_API ScriptsLoader
{
    public:
        static HMODULE customScriptDll;
        static std::map<std::string, CreateScriptFunc> scriptMap;
        static IScript* GetScriptFromCallName(std::string callName);
        static void LoadDLL();
        static void LoadExportedFunctions();

};


#endif