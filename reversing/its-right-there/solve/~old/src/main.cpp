
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace Eh {
    #include <stdio.h>
}

#define XRES    660
#define YRES    480
float fullscreenQuad[] = { 0,     0,    0, 1, 0, 0,
                           XRES,  0,    0, 1, 1, 0,
                           0,	  YRES, 0, 1, 0, 1,
                           XRES,  YRES, 0, 1, 1, 1 };

static IDirect3DPixelShader9* pShader;
static IDirect3DVertexShader9* vShader;
static IDirect3DTexture9* pFontTexture;

void intro_init(IDirect3DDevice9* d3dDevice)
{
    static char g_ps30_main[0x10000];
    static char g_vs30_main[0x10000];
    DWORD b;
    HANDLE hFile = CreateFile("rsrc/ps30.bin", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ReadFile(hFile, g_ps30_main, 0x10000, &b, NULL);
    CloseHandle(hFile);
    hFile = CreateFile("rsrc/vs30.bin", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ReadFile(hFile, g_vs30_main, 0x10000, &b, NULL);
    CloseHandle(hFile);

    d3dDevice->CreatePixelShader((const DWORD*)g_ps30_main, &pShader);
    d3dDevice->CreateVertexShader((const DWORD*)g_vs30_main, &vShader);

    D3DXCreateTextureFromFileA(d3dDevice, "rsrc/font.jpg", &pFontTexture);
    d3dDevice->SetTexture(0, pFontTexture);

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

static D3DPRESENT_PARAMETERS devParams =
{
    0, 0,   // xres, yres
    D3DFMT_A8R8G8B8,
    0,      // backBufferCount;
    D3DMULTISAMPLE_NONE,
    0,      // multiSampleQuality;
    D3DSWAPEFFECT_DISCARD,
    0,      // hDeviceWindow;
    true,   // windowed
    true,
    D3DFMT_D24S8,
    0,  // flags;
    0,  // fullScreen_RefreshRateInHz;
    D3DPRESENT_INTERVAL_IMMEDIATE   // presentationInterval
};

extern "C" int _fltused = 0;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
        return(0);

    if (uMsg == WM_CLOSE || uMsg == WM_DESTROY || (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE))
    {
        PostQuitMessage(0);
        return(0);
    }

    if (uMsg == WM_CHAR || uMsg == WM_KEYDOWN)
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return(0);
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void entrypoint(void)
{
    IDirect3DDevice9* d3dDevice;

    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d)
        return;

    DWORD           dwExStyle, dwStyle;
    DEVMODE         dmScreenSettings;
    RECT            rec;
    WNDCLASS        wc;

    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "static";
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(0x00785838);

    RegisterClass(&wc);

    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;

    rec.left = 0;
    rec.top = 0;
    rec.right = XRES;
    rec.bottom = YRES;

    AdjustWindowRect(&rec, dwStyle, 0);

    devParams.hDeviceWindow = CreateWindowEx(dwExStyle, wc.lpszClassName, "Hewwo", dwStyle,
        (GetSystemMetrics(SM_CXSCREEN) - rec.right + rec.left) >> 1,
        (GetSystemMetrics(SM_CYSCREEN) - rec.bottom + rec.top) >> 1,
        rec.right - rec.left, rec.bottom - rec.top, 0, 0, wc.hInstance, 0);

    if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, devParams.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &devParams, &d3dDevice) < 0)
        return;

    ShowCursor(1);

    intro_init(d3dDevice);

    MSG msg; bool done = 0;
    do
    {
        POINT pt; GetCursorPos(&pt); ScreenToClient(devParams.hDeviceWindow, &pt);
        float m[2] = { (float)pt.x / XRES, (float)pt.y / YRES };

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) done = 1;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        intro_do(d3dDevice, GetTickCount(), m);
    } while (!done);

    ExitProcess(0);
}

