#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <winternl.h>
#include "resource.h"

typedef void (*VOID_FUNC_PTR)(void);
//typedef int (*INT_FUNC_PTR)(int);
typedef int (__stdcall *FARPROC_1)(int param0);
typedef int (__stdcall *FARPROC_2)(int param0, int param1);
typedef int (__stdcall *FARPROC_3)(int param0, int param1, int param2);

typedef struct Node {
    int index;
    FARPROC funcPtr0;
    FARPROC_1 funcPtr1;
    FARPROC_2 funcPtr2;
    FARPROC_3 funcPtr3;
    struct Node* prev;
    struct Node* next;
} LL_NODE, * LL_NODE_PTR;

enum NUM_PARAMS {
    NO_PARAMS,
    ONE_PARAM,
    TWO_PARAMS,
    THREE_PARAMS
};

enum DLL_RESOURCE_IDS {
    PRIME_DLL = IDR_RCDATA1,
    RNG_DLL = IDR_RCDATA2,
    XOR_DLL = IDR_RCDATA3,
    DEBUG_DLL = IDR_RCDATA4
};

enum FUNCTIONS {
    DO_RNG,
    DEBUG_DETECTOR,
    PLUS_ONE,
    IS_PRIME,
    DO_XOR
};

// incomplete winternl struct redefinitions
typedef struct _FULL_PEB_LDR_DATA {
    ULONG Length;
    BOOL Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} FULL_PEB_LDR_DATA, *PFULL_PEB_LDR_DATA;

typedef struct _FULL_LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} FULL_LDR_DATA_TABLE_ENTRY, * PFULL_LDR_DATA_TABLE_ENTRY;

// indirect function call definitions
typedef HRSRC(WINAPI* id_FindResourceA)(
    _In_opt_ HMODULE hModule,
    _In_ LPCSTR lpName,
    _In_ LPCSTR lpType
    );

typedef DWORD(WINAPI* id_SizeOfResource)(
    _In_opt_ HMODULE hModule,
    _In_ HRSRC hResInfo
    );

typedef HGLOBAL(WINAPI* id_LoadResource)(
    _In_opt_ HMODULE hModule,
    _In_ HRSRC hResInfo
    );

typedef LPVOID(WINAPI* id_LockResource)(
    _In_ HGLOBAL hResData
    );

typedef DWORD(WINAPI* id_GetTempPathW)(
    _In_ DWORD nBufferLength,
    _Out_ LPWSTR lpBuffer
    );

typedef UINT(WINAPI* id_GetTempFileNameW)(
    _In_ LPCWSTR lpPathName,
    _In_opt_ LPCWSTR lpPrefixString,
    _In_ UINT uUnique,
    _Out_ LPWSTR lpTempFileName
    );

typedef HANDLE(WINAPI* id_CreateFileW)(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );

typedef BOOL(WINAPI* id_WriteFile)(
    _In_ HANDLE hFile,
    _In_ LPCVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToWrite,
    _Out_opt_ LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
    );

typedef BOOL(WINAPI* id_CloseHandle)(
    _In_ HANDLE hObject
    );

typedef HMODULE(WINAPI* id_LoadLibraryW)(
    _In_ LPCWSTR lpLibFileName
    );

typedef FARPROC(WINAPI* id_GetProcAddress)(
    _In_ HMODULE hModule,
    _In_ LPCSTR lpProcName
    );

typedef HANDLE(WINAPI* id_GetStdHandle)(
    _In_ DWORD nStdHandle
    );

typedef BOOL(WINAPI* id_WriteConsole)(
    _In_ HANDLE hConsoleOutput,
    _In_ LPCVOID lpBuffer,
    _In_ DWORD nNumberOfCharsToWrite,
    _Out_opt_ LPDWORD lpNumberOfCharsWritten,
    _Reserved_ LPVOID Reserved
    );

typedef BOOL(WINAPI* id_ReadConsole)(
    _In_ HANDLE hConsoleInput,
    _Out_ LPVOID lpBuffer,
    _In_ DWORD nNumberOfCharsToRead,
    _Out_ LPDWORD lpNumberOfCharsRead,
    _In_opt_ LPVOID pInputControl
    );