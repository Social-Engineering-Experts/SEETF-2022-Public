// dllmain.cpp : Defines the entry point for the DLL application.
//#include "pch.h"
#include "debugDetector.h"

int ad42f6697b035b7580e4fef93be20b4d(void) {
	int isBeingDebugged = IsDebuggerPresent();

	if (isBeingDebugged == 0)
		return 0;
	else
		return 1;
}

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}

