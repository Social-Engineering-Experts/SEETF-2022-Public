#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#ifdef DEBUGDETECTOR
#define DEBUGDETECTOR __declspec(dllexport)
#else
#define DEBUGDETECTOR __declspec(dllimport)
#endif

extern "C" DEBUGDETECTOR int ad42f6697b035b7580e4fef93be20b4d(void);