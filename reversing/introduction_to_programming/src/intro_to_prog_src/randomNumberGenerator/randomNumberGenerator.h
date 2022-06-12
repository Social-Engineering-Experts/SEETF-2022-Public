#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <wincrypt.h>
#include <cstdlib>
#include <ctime>
#include <iostream>


#ifdef RNG 
#define RNG __declspec(dllexport)
#else
#define RNG __declspec(dllimport)
#endif

extern "C" RNG int d2d9ce665f66ae49556b47436a514c0f(void);
