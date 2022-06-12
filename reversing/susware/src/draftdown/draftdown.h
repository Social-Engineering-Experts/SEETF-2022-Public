#pragma once

#include "resource.h"
#include <iostream>
#include <stdio.h>
#include <debugapi.h>
#include <urlmon.h>
#include <WinInet.h>
#include <strsafe.h>
#include <Lmcons.h>
#include <TlHelp32.h> 
#include <time.h>
#include <bitset>
#include <fstream>
#include <sstream>


#define WIN32_LEAN_AND_MEAN        
#include <windows.h>      
#include <intrin.h>       
#include <iphlpapi.h>     
#ifndef _MSC_VER
#include <cpuid.h>
#else
#include <intrin.h>
#endif
#pragma comment(lib, "IPHLPAPI.lib")


class SuperSecret
{
public:
    SuperSecret();
    ~SuperSecret();
    void CheckSuperSecret();
    void setSuperSecret();


private:
    uint64_t secret;
    uint16_t macHash;
    uint16_t mac1, mac2;
    uint16_t volumeHash;
    uint16_t cpuHash;
    const char* machineName;
    std::string superSecretString;
    uint16_t  hashMacAddress(PIP_ADAPTER_INFO info);
    void getMacHash(uint16_t& mac1, uint16_t& mac2);
    uint16_t getVolumeHash();
    uint16_t getCpuHash();
    const char* getMachineName();

};

