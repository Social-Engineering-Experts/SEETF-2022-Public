#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <d3d9.h>
#include "intro.h"
#include "main.h"

#ifndef RELEASE_FULLSCREEN
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
#else
static D3DPRESENT_PARAMETERS devParams = {
    XRES, YRES, D3DFMT_A8R8G8B8, 0, D3DMULTISAMPLE_NONE,
    0, D3DSWAPEFFECT_DISCARD, 0, false, true,
    D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_IMMEDIATE };
#endif

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

#ifndef RELEASE_FULLSCREEN
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
#else
    devParams.hDeviceWindow = CreateWindow("static", 0, WS_POPUP | WS_VISIBLE, 0, 0, XRES, YRES, 0, 0, 0, 0);
#endif

    if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, devParams.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &devParams, &d3dDevice) < 0)
        return;

    ShowCursor(1);

    intro_init(d3dDevice);

#ifndef RELEASE_FULLSCREEN
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
#else
    do
    {
        POINT pt; GetCursorPos(&pt); ScreenToClient(devParams.hDeviceWindow, &pt);
        float m[2] = { (float)pt.x / XRES, (float)pt.y / YRES };
        intro_do(d3dDevice, GetTickCount(), m);

    } while (!GetAsyncKeyState(VK_ESCAPE));
#endif

    ExitProcess(0);
#ifdef CLEANEXIT
    d3dDevice->Release();
    d3d->Release();
    DestroyWindow(devParams.hDeviceWindow);
#endif
}

