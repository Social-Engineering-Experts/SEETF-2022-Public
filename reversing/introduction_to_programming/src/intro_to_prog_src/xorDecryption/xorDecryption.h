#pragma once
//#include <iostream>

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files
//#include <windows.h>

#ifdef XORDECRYPTION
#define XORDECRYPTION __declspec(dllexport)
#else
#define XORDECRYPTION __declspec(dllimport)
#endif

extern "C" XORDECRYPTION int b447c27a00e3a348881b0030177000cd(int toXor, int index);
//extern "C" XORDECRYPTION void test0();
//extern "C" XORDECRYPTION void test1();
//extern "C" XORDECRYPTION void test2();
extern "C" XORDECRYPTION int e6eb072262783d012216b40548166d1e(int toPlus);