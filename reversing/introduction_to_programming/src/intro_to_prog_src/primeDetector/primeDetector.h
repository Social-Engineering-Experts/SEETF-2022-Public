#pragma once

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files
//#include <windows.h>

#ifdef PRIMEDETECTOR
#define PRIMEDETECTOR __declspec(dllexport)
#else
#define PRIMEDETECTOR __declspec(dllimport)
#endif

extern "C" PRIMEDETECTOR int d351cf47729ee988f0ac58b5aacc0758(int testInt);