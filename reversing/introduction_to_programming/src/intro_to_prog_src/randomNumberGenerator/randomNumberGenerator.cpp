// dllmain.cpp : Defines the entry point for the DLL application.
#include "randomNumberGenerator.h"

using namespace std;

int d2d9ce665f66ae49556b47436a514c0f(void) {
	int outRandomNum = 0;

	HMODULE hAdvApi = GetModuleHandleA("advapi32.dll");
	if (hAdvApi != NULL) {
		BOOLEAN(APIENTRY * RtlGenRandom)(void*, ULONG) =
			(BOOLEAN(APIENTRY*)(void*, ULONG))GetProcAddress(hAdvApi, "SystemFunction036");
		if (RtlGenRandom != NULL) {
			if (RtlGenRandom(&outRandomNum, sizeof(outRandomNum)))
				//cout << "Using RtlGenRandom" << endl;
				return outRandomNum;
		}
	}

	HCRYPTPROV hProv;
	if (CryptAcquireContextA(&hProv, NULL, MS_DEF_PROV_A, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		if (CryptGenRandom(hProv, sizeof(outRandomNum), (BYTE*)&outRandomNum))
			//cout << "Using CryptAPI" << endl;
			return outRandomNum;
	}

	srand(time(NULL));
	//cout << "Using rand()" << endl;
	return rand();
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

