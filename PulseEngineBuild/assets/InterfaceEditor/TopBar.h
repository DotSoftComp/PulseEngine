#ifndef TOPBAR_H
#define TOPBAR_H

#include "Common/common.h"
#include "Common/dllExport.h"

class PulseEngineBackend;

class MA_DLL_API TopBar
{
private:

public:
    TopBar() {}
    ~TopBar() {}

    void UpdateBar(PulseEngineBackend* engine);
};


#endif