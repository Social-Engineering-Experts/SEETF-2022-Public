// dllmain.cpp : Defines the entry point for the DLL application.
#include "xorDecryption.h"

constexpr int KEY[] = { 0x69, 0x420, 0x123 };

using namespace std;


int b447c27a00e3a348881b0030177000cd(int toXor, int index) {
	return toXor ^ KEY[index % 3];
}


int e6eb072262783d012216b40548166d1e(int toPlus) {
	return toPlus + 1;
}


//void test0() {
//	cout << "Hello from the DLL, index 0" << endl;
//}
//
//
//void test1() {
//	cout << "Hello from the DLL, index 1" << endl;
//}
//
//
//void test2() {
//	cout << "Hello from the DLL, index 2" << endl;
//}

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

