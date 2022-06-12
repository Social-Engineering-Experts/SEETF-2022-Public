#pragma once

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files
//#include <windows.h>

#ifdef DJB2HASHING
#define DJB2HASHING __declspec(dllexport)
#else
#define DJB2HASHING __declspec(dllimport)
#endif