#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "main.h"

#include "_pixel_gen.h"
#include "_vertex_gen.h"
#include "_fonttexture_gen.h"

//------------------------------------------------------------------------------------------------------------------

float fullscreenQuad[] = { 0,     0,    0, 1, 0, 0,
                           XRES,  0,    0, 1, 1, 0,
                           0,	  YRES, 0, 1, 0, 1,
                           XRES,  YRES, 0, 1, 1, 1 };

//------------------------------------------------------------------------------------------------------------------

static IDirect3DPixelShader9* pShader;
static IDirect3DVertexShader9* vShader;
static IDirect3DTexture9* pFontTexture;

//------------------------------------------------------------------------------------------------------------------

void intro_init(IDirect3DDevice9* d3dDevice)
{
    d3dDevice->CreatePixelShader((const DWORD*)g_ps30_main, &pShader);
    d3dDevice->CreateVertexShader((const DWORD*)g_vs30_main, &vShader);

    D3DXCreateTextureFromFileInMemory(d3dDevice, bufferFontTexture, sizeof(bufferFontTexture), &pFontTexture);
    d3dDevice->SetTexture(0, pFontTexture); // pFontTexture.Sample(FontTextureSampler, u)
    
    float res[2] = { (float)XRES, (float)YRES };
    d3dDevice->SetPixelShaderConstantF(1, res, 1);
    d3dDevice->SetPixelShader(pShader);
    d3dDevice->SetVertexShader(vShader);
    d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
}

static float input[4];
void intro_do(IDirect3DDevice9* d3dDevice, long time, float mousepos[2])
{
    input[0] = (float)time / 1000.0f;
    input[1] = mousepos[0]; input[2] = mousepos[1];
    
    d3dDevice->SetPixelShaderConstantF(0, input, 1);

    d3dDevice->BeginScene();
    d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fullscreenQuad, 6 * sizeof(float));
    d3dDevice->EndScene();
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}