#include "..\SDK\inc\main.h"
#include "script.h"
#include "controller.h"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID)
{
    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
        scriptRegister(hInstance, ScriptMain);
        break;
    case DLL_PROCESS_DETACH:
        scriptUnregister(hInstance);
        break;
    }
    return TRUE;
}
