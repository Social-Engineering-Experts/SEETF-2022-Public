// dllmain.cpp : Defines the entry point for the DLL application.
#include "primeDetector.h"

int d351cf47729ee988f0ac58b5aacc0758(int testInt) {
	if (testInt <= 1) return 0;
	if (testInt <= 3) return 1;

	if (testInt % 2 == 0 || testInt % 3 == 0)
		return 0;

	for (int i = 5; i * i <= testInt; i = i + 6)
		if (testInt % i == 0 || testInt % (i + 2) == 0)
			return 0;

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

