#pragma once


#include <Windows.h>

typedef BOOL (WINAPI* anti_dbg_1)();

typedef BOOL (WINAPI* anti_dbg_2)(
	HANDLE hProcess,
	PBOOL  pbDebuggerPresent
);

typedef HMODULE(WINAPI* ghmodule)(
	LPCWSTR lpModuleName
);

typedef HRSRC(WINAPI* frw)(
	 HMODULE hModule,
	 LPCWSTR lpName,
     LPCWSTR lpType
);

typedef HGLOBAL(WINAPI* lrw)(
	HMODULE hModule,
	HRSRC   hResInfo
);


typedef DWORD(WINAPI* szr)(
	HMODULE hModule,
	HRSRC   hResInfo
);


typedef LPVOID(WINAPI* lkr)(
	HGLOBAL hResData
);

typedef void (WINAPI* anti_dbg_3)();

typedef HANDLE (WINAPI* get_c_proc)();
