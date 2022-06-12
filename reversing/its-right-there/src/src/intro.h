#ifndef _DEMO_H_
#define _DEMO_H_

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <d3d9.h>

void intro_init( IDirect3DDevice9 *d3dDevice );
void intro_do( IDirect3DDevice9 *d3dDevice, long time, float mousepos[2]);
void intro_end( IDirect3DDevice9 *d3dDevice );

#endif
