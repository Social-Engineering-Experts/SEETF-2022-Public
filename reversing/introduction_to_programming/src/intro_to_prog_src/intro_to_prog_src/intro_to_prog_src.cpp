// intro_to_prog_src.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "definitions.h"
#pragma warning(disable: 4201)

using namespace std;


class DoublyLinkedList {
public:
    LL_NODE_PTR head;
public:
    // constructor
    DoublyLinkedList() { head = nullptr; }
    ~DoublyLinkedList() { head = nullptr; }

    void printList() {
        LL_NODE_PTR temp = head;
        if (temp != nullptr) {
            while (temp != nullptr) {
                //cout << "list index: " << temp->index << endl;
                temp = temp->next;
            }
        }
    }


    void traverseAndExecute() {
        LL_NODE_PTR temp = head;
        if (temp != nullptr) {
            while (temp != nullptr) {
                //cout << "calling func at list index: " << temp->index << endl;
                temp->funcPtr0();
                temp = temp->next;
            }
        }
    }


    int executeAtIndex(int index, int num_params, int param_0, int param_1, int param_2) {
        LL_NODE_PTR temp = head;
        int result = 0;

        if (temp != nullptr) {
            while (temp->index < index)
                temp = temp->next;

            switch (num_params) {
            case 0:
                result = temp->funcPtr0();
                break;
            case 1:
                result = temp->funcPtr1(param_0);
                break;
            case 2:
                result = temp->funcPtr2(param_0, param_1);
                break;
            case 3:
                result = temp->funcPtr3(param_0, param_1, param_2);
                break;
            }
        }

        return result;
    }


    int getSize(void) {
        LL_NODE_PTR temp = head;
        int llSize = 0;

        while (temp != nullptr) {
            ++llSize;
            temp = temp->next;

        }

        return llSize;
    }


    void insertAtEnd(int numParams, FARPROC newFunc0, FARPROC_1 newFunc1, FARPROC_2 newFunc2, FARPROC_3 newFunc3) {
        LL_NODE_PTR temp = head;
        LL_NODE_PTR newNode = new LL_NODE;
        int currSize = getSize();

        // set up new node
        newNode->next = nullptr;
        switch (numParams) {
        case 0:
            newNode->funcPtr0 = newFunc0;
            break;
        case 1:
            newNode->funcPtr1 = newFunc1;
            break;
        case 2:
            newNode->funcPtr2 = newFunc2;
            break;
        case 3:
            newNode->funcPtr3 = newFunc3;
            break;
        }

        //cout << "List size " << currSize << endl;
        if (temp == nullptr) { // empty list
            //cout << "list is empty" << endl;
            newNode->prev = nullptr;
            newNode->index = currSize;
            head = newNode;
        }
        else if (temp != nullptr) { // non-empty list
            //cout << "list is not empty" << endl;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = newNode;
            newNode->prev = temp;
            newNode->index = currSize;
        }
    }
};

DoublyLinkedList dll;


long __stdcall doROR13W(wstring toHash) {
    long hash = 0;
    long intVal;

    for (wchar_t c : toHash) {
        intVal = (long)c;
        hash = (hash >> 13) | (hash << 19);
        hash += intVal;
    }

    return hash;
}


long __stdcall doROR13A(string toHash) {
    long hash = 0;
    long intVal;

    for (char c : toHash) {
        intVal = (long)c;
        hash = (hash >> 13) | (hash << 19);
        hash += intVal;
    }

    return hash;
}


HMODULE __stdcall GetProcAddressByHash(long hash, int hashOffset) {
    PPEB pebPtr;
    PFULL_PEB_LDR_DATA pLdr;
    PLIST_ENTRY pNextModule;
    PVOID pModuleBase;
    PFULL_LDR_DATA_TABLE_ENTRY pDataTableEntry;
    PIMAGE_NT_HEADERS pNtHeader;
    PIMAGE_EXPORT_DIRECTORY pExportDir;
    DWORD dwExportDirRVA;
    DWORD dwNumFunctions;
    PDWORD pDwFunctionNameBase;
    PCSTR pFunctionName;
    USHORT usOrdinalTableIndex;

    wstring moduleName;
    long moduleHash;
    long functionHash;
    long kernel32hash = -550817435;

#if defined(_WIN64)
    pebPtr = (PPEB)__readgsqword(0x60);
#else
    pebPtr = (PPEB)__readfsdword(0x30);
#endif

    pLdr = (PFULL_PEB_LDR_DATA)pebPtr->Ldr;
    pNextModule = pLdr->InLoadOrderModuleList.Flink;
    pDataTableEntry = (PFULL_LDR_DATA_TABLE_ENTRY)pNextModule;

    while (pDataTableEntry->DllBase != NULL) {
        moduleName = pDataTableEntry->BaseDllName.Buffer;
        pModuleBase = pDataTableEntry->DllBase;
        pNtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)pModuleBase + ((PIMAGE_DOS_HEADER)pModuleBase)->e_lfanew);
        dwExportDirRVA = pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
        //wcout << moduleName << L" " << doROR13W(moduleName) << endl;
        pDataTableEntry = (PFULL_LDR_DATA_TABLE_ENTRY)pDataTableEntry->InLoadOrderLinks.Flink;

        if (dwExportDirRVA == 0)
            continue;

        long moduleHash = doROR13W(moduleName);
        if (moduleHash != kernel32hash)
            continue; // all our target funcs are from kernel32

        pExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)pModuleBase + dwExportDirRVA);
        dwNumFunctions = pExportDir->NumberOfNames;
        pDwFunctionNameBase = (PDWORD)((PCHAR)pModuleBase + pExportDir->AddressOfNames);

        for (int i = 0; i < dwNumFunctions; i++) {
            pFunctionName = (PCSTR)(*pDwFunctionNameBase + (ULONG_PTR)pModuleBase);
            functionHash = doROR13A(pFunctionName);
            functionHash += moduleHash;

            //cout << pFunctionName << " " << functionHash << endl;
            if (functionHash == hash) {
                //cout << pFunctionName << "found - hash " << functionHash << endl;
                if (hashOffset == 0) {
                    //cout << "\n\n";
                    usOrdinalTableIndex = *(PUSHORT)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfNameOrdinals) + (2 * i));
                    return (HMODULE)((ULONG_PTR)pModuleBase + *(PDWORD)(((ULONG_PTR)pModuleBase + pExportDir->AddressOfFunctions) + (4 * usOrdinalTableIndex)));
                }
                else {
                    hashOffset--;
                    //cout << "Skipping - hash offset is now " << hashOffset << endl;
                }
            }
            pDwFunctionNameBase++;
        }
    }

    return (HMODULE)INVALID_HANDLE_VALUE;
}


id_LoadLibraryW getLoadLibraryW(void) {
    long loadLibraryWHash = -499961412;
    id_LoadLibraryW FuncLoadLibraryW;
    FuncLoadLibraryW = (id_LoadLibraryW)GetProcAddressByHash(loadLibraryWHash, 1);
    return FuncLoadLibraryW;
}


id_GetProcAddress getGetProcAddress(void) {
    long getProcAddressHash = -1698477288;
    id_GetProcAddress FuncGetProcAddress;
    FuncGetProcAddress = (id_GetProcAddress)GetProcAddressByHash(getProcAddressHash, 0);
    return FuncGetProcAddress;
}


FARPROC importDLLFunction0(wstring dllName, string funcName) {
    HMODULE hDll = NULL;
    FARPROC funcAddress = NULL;
    id_LoadLibraryW FuncLoadLibraryW = getLoadLibraryW();
    id_GetProcAddress FuncGetProcAddress = getGetProcAddress();

    hDll = FuncLoadLibraryW(dllName.c_str());
    if (hDll == NULL)
        return NULL;

    funcAddress = FuncGetProcAddress(hDll, funcName.c_str());
    if (funcAddress == NULL)
        return NULL;

    return funcAddress;
}
 

FARPROC_1 importDLLFunction1(wstring dllName, string funcName) {
    HMODULE hDll = NULL;
    FARPROC_1 funcAddress = NULL;
    id_LoadLibraryW FuncLoadLibraryW = getLoadLibraryW();
    id_GetProcAddress FuncGetProcAddress = getGetProcAddress();

    hDll = FuncLoadLibraryW(dllName.c_str());
    if (hDll == NULL)
        return NULL;

    funcAddress = (FARPROC_1)FuncGetProcAddress(hDll, funcName.c_str());
    if (funcAddress == NULL)
        return NULL;

    return funcAddress;
}
 

FARPROC_2 importDLLFunction2(wstring dllName, string funcName) {
    HMODULE hDll = NULL;
    FARPROC_2 funcAddress = NULL;
    id_LoadLibraryW FuncLoadLibraryW = getLoadLibraryW();
    id_GetProcAddress FuncGetProcAddress = getGetProcAddress();

    hDll = FuncLoadLibraryW(dllName.c_str());
    if (hDll == NULL)
        return NULL;

    funcAddress = (FARPROC_2)FuncGetProcAddress(hDll, funcName.c_str());
    if (funcAddress == NULL)
        return NULL;

    return funcAddress;
}


FARPROC_3 importDLLFunction3(wstring dllName, string funcName) {
    HMODULE hDll = NULL;
    FARPROC_3 funcAddress = NULL;
    id_LoadLibraryW FuncLoadLibraryW = getLoadLibraryW();
    id_GetProcAddress FuncGetProcAddress = getGetProcAddress();

    hDll = FuncLoadLibraryW(dllName.c_str());
    if (hDll == NULL)
        return NULL;

    funcAddress = (FARPROC_3)FuncGetProcAddress(hDll, funcName.c_str());
    if (funcAddress == NULL)
        return NULL;

    return funcAddress;
}


wstring dumpAndWriteDLL(int dllId) {
    // function vars
    wstring returnMsg;
    int returnVal;
    // import resolution vars
    long findResourceAHash = -497859738;
    long sizeofResourceHash = -1673310646;
    long loadLockResourceHash = -512544310;
    long getTempPathWHash = -508350020;
    long getTempFileNameWHash = -550817373;
    long createFileWHash = -550817376;
    long writeFileHash = 1183534090;
    long closeHandleHash = -550937889;

    id_FindResourceA FuncFindResourceA;
    id_SizeOfResource FuncSizeofResource;
    id_LoadResource FuncLoadResource;
    id_LockResource FuncLockResource;
    id_GetTempPathW FuncGetTempPathW;
    id_GetTempFileNameW FuncGetTempFileNameW;
    id_CreateFileW FuncCreateFileW;
    id_WriteFile FuncWriteFile;
    id_CloseHandle FuncCloseHandle;

    // resource loading data
    HRSRC hDll;
    HGLOBAL hDllData;
    int dllSize;
    void* dllBytesPtr;
    // DLL writing data
    WCHAR tempPath[MAX_PATH] = { 0 };
    WCHAR dllPath[MAX_PATH] = { 0 };
    HANDLE hDllOut;

    FuncFindResourceA = (id_FindResourceA)GetProcAddressByHash(findResourceAHash, 0);
    FuncSizeofResource = (id_SizeOfResource)GetProcAddressByHash(sizeofResourceHash, 0);
    FuncLoadResource = (id_LoadResource)GetProcAddressByHash(loadLockResourceHash, 0);
    FuncLockResource = (id_LockResource)GetProcAddressByHash(loadLockResourceHash, 1);
    FuncGetTempPathW = (id_GetTempPathW)GetProcAddressByHash(getTempPathWHash, 1);
    FuncGetTempFileNameW = (id_GetTempFileNameW)GetProcAddressByHash(getTempFileNameWHash, 1);
    FuncCreateFileW = (id_CreateFileW)GetProcAddressByHash(createFileWHash, 1);
    FuncWriteFile = (id_WriteFile)GetProcAddressByHash(writeFileHash, 0);
    FuncCloseHandle = (id_CloseHandle)GetProcAddressByHash(closeHandleHash, 0);

    hDll = FuncFindResourceA(NULL, MAKEINTRESOURCEA(dllId), MAKEINTRESOURCEA(RT_RCDATA));
    if (hDll != NULL) {
        dllSize = FuncSizeofResource(NULL, hDll);
        hDllData = FuncLoadResource(NULL, hDll);
        if (hDllData != NULL) {
            dllBytesPtr = FuncLockResource(hDllData);

            returnVal = FuncGetTempPathW(MAX_PATH, tempPath);
            if (!(returnVal > MAX_PATH || (returnVal == 0))) {
                returnVal = FuncGetTempFileNameW(tempPath, L"dll", 0, dllPath);
                if (returnVal) {
                    //wcout << "Filename: " << dllPath << endl;

                    hDllOut = FuncCreateFileW(dllPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    FuncWriteFile(hDllOut, dllBytesPtr, dllSize, NULL, NULL);
                    FuncCloseHandle(hDllOut);

                    returnMsg = dllPath;
                }
                //else {
                //    returnMsg = L"Error getting temp file name";
                //    wcerr << returnMsg << endl;
                //}
            }
            //else {
            //    returnMsg = L"Error writing temp path to buffer";
            //    wcerr << returnMsg << endl;
            //}
        }
        //else {
        //    returnMsg = L"Error loading DLL";
        //    wcerr << returnMsg << endl;
        //}
    } 
    //else {
    //    returnMsg = wstring(L"Error finding DLL with ID ") + to_wstring(dllId);
    //    wcerr << returnMsg << endl;
    //}
    return returnMsg;
}


void setUpDLL(void) {
    // set up func pointers for importing
    FARPROC func0 = NULL;
    FARPROC_1 func1 = NULL;
    FARPROC_2 func2 = NULL;
    // set up DLL filenames
    wstring primeDllFilename;
    wstring xorDllFilename;
    wstring rngDllFilename;
    wstring debugDllFilename;

    primeDllFilename = dumpAndWriteDLL(PRIME_DLL);
    xorDllFilename = dumpAndWriteDLL(XOR_DLL);
    rngDllFilename = dumpAndWriteDLL(RNG_DLL);
    debugDllFilename = dumpAndWriteDLL(DEBUG_DLL);

    vector<pair<wstring, string>> toImport0 = {
        //{"xorDecryption.dll", "test0"},
        //{"xorDecryption.dll", "test1"},
        //{"xorDecryption.dll", "test2"},
        {rngDllFilename, "d2d9ce665f66ae49556b47436a514c0f"},
        {debugDllFilename, "ad42f6697b035b7580e4fef93be20b4d"}
    };

    vector<pair<wstring, string>> toImport1 = {
        {xorDllFilename, "e6eb072262783d012216b40548166d1e"}, // plus one
        {primeDllFilename, "d351cf47729ee988f0ac58b5aacc0758"}
    };

    vector<pair<wstring, string>> toImport2 = {
        {xorDllFilename, "b447c27a00e3a348881b0030177000cd"} // doXOR
    };

    for (pair<wstring, string> dllAndFunction : toImport0) {
        func0 = importDLLFunction0(dllAndFunction.first, dllAndFunction.second);
        if (func0 != NULL) {
            dll.insertAtEnd(NO_PARAMS, func0, NULL, NULL, NULL);
        }
        //else {
        //    cerr << "import failed for " << dllAndFunction.second << "@";
        //    wcerr << dllAndFunction.first << endl;
        //}
    }

    for (pair<wstring, string> dllAndFunction : toImport1) {
        func1 = importDLLFunction1(dllAndFunction.first, dllAndFunction.second);
        if (func1 != NULL) {
            dll.insertAtEnd(ONE_PARAM, NULL, func1, NULL, NULL);
        }
        //else {
        //    cerr << "import failed for " << dllAndFunction.second << "@";
        //    wcerr << dllAndFunction.first << endl;
        //}
    }

    for (pair<wstring, string> dllAndFunction : toImport2) {
        func2 = importDLLFunction2(dllAndFunction.first, dllAndFunction.second);
        if (func2 != NULL) {
            dll.insertAtEnd(TWO_PARAMS, NULL, NULL, func2, NULL);
        }
        //else {
        //    cerr << "import failed for " << dllAndFunction.second << "@";
        //    wcerr << dllAndFunction.first << endl;
        //}
    }

    _wremove(primeDllFilename.c_str());
    _wremove(rngDllFilename.c_str());
    _wremove(xorDllFilename.c_str());
}


void writeToConsole(string toWrite) {
    long getStdHandleHash = -508874294;
    long writeConsoleAHash = -502058586;
    id_GetStdHandle FuncGetStdHandle;
    id_WriteConsole FuncWriteConsole;
    HANDLE hStdOut;

    FuncGetStdHandle = (id_GetStdHandle)GetProcAddressByHash(getStdHandleHash, 1);
    FuncWriteConsole = (id_WriteConsole)GetProcAddressByHash(writeConsoleAHash, 4);

    hStdOut = FuncGetStdHandle(STD_OUTPUT_HANDLE);
    FuncWriteConsole(hStdOut, toWrite.c_str(), toWrite.size(), NULL, NULL);
}


string readFromConsole(void) {
    long getStdHandleHash = -508874294;
    long readConsoleAHash = -508350042;
    id_GetStdHandle FuncGetStdHandle;
    id_ReadConsole FuncReadConsole;
    HANDLE hStdIn;
    string outString;
    
    DWORD dwRead;
    char next;

    FuncGetStdHandle = (id_GetStdHandle)GetProcAddressByHash(getStdHandleHash, 1);
    FuncReadConsole = (id_ReadConsole)GetProcAddressByHash(readConsoleAHash, 2);

    hStdIn = FuncGetStdHandle(STD_INPUT_HANDLE);
    if (!FuncReadConsole(hStdIn, &next, 1, &dwRead, nullptr))
        return "";

    while (next != '\n') {
        if (next != '\r')
            outString += next;
        if (!FuncReadConsole(hStdIn, &next, 1, &dwRead, nullptr))
            return "";
    }

    return outString;
}


vector<int> simplerEncryptDecrypt(vector<int> inVec) {
    vector<int> outVec;
    int currChar;

    for (int i = 0; i < inVec.size(); i = dll.executeAtIndex(PLUS_ONE, ONE_PARAM, i, NULL, NULL)) {
        currChar = inVec[i];
        outVec.push_back(dll.executeAtIndex(DO_XOR, TWO_PARAMS, currChar, i, NULL)); // doXOR()
    }

    return outVec;
}


vector<int> encryptDecrypt(vector<int> inVec) {
    vector<int> outVec;
    int sinceLastPrime = 0;

    for (int i = 0; i < inVec.size(); i = dll.executeAtIndex(PLUS_ONE, ONE_PARAM, i, NULL, NULL)) {
        if (dll.executeAtIndex(IS_PRIME, ONE_PARAM, i, NULL, NULL) == 1) // index is prime
            sinceLastPrime = 0;
        else if (dll.executeAtIndex(DEBUG_DETECTOR, NO_PARAMS, NULL, NULL, NULL) != 0) // debugger attached
            inVec[i] = dll.executeAtIndex(DO_RNG, NO_PARAMS, NULL, NULL, NULL) % 0x43;
        else
            sinceLastPrime++;

        outVec.push_back(dll.executeAtIndex(DO_XOR, TWO_PARAMS, inVec[i], i, NULL));
    }

    return outVec;
}


string vecToString(vector<int> inVec) {
    string outString;

    for (int currChar : inVec)
        outString += (char)currChar;

    return outString;
}


vector<int> stringToVec(string inString) {
    vector<int> outVec;

    for (char currChar : inString)
        outVec.push_back((int)currChar);

    return outVec;
}


int main() {
    setUpDLL();
    int result;
    int hash;
    /*
    DLL indexes
    0 - doRNG
    1 - debugDetector
    2 - plusOne
    3 - isPrime
    4 - doXOR
    */
    //writeToConsole("abcd\n");
    vector<int> prompt = { 45, 1139, 279, 91, 1040, 281, 73, 1124, 332, 28, 1090, 335, 16, 1037, 367, 0, 1102, 328, 12, 1092, 259, 37, 1097, 336, 29, 1107, 297, 62, 1093, 260, 31, 1093, 259, 4, 1089, 327, 12, 1024, 322, 73, 1094, 326, 30, 1024, 320, 1, 1089, 333, 14, 1093, 336, 73, 1108, 332, 73, 1108, 331, 12, 1024, 320, 6, 1092, 326, 73, 1097, 333, 73, 1108, 331, 12, 1024, 339, 27, 1093, 341, 0, 1103, 342, 26, 1024, 322, 26, 1107, 330, 14, 1102, 334, 12, 1102, 343, 71, 1066, 375, 27, 1113, 259, 29, 1103, 259, 14, 1093, 343, 73, 1108, 331, 12, 1024, 339, 8, 1107, 336, 30, 1103, 337, 13, 1025, 259, 61, 1096, 326, 73, 1093, 333, 10, 1106, 346, 25, 1108, 330, 6, 1102, 259, 8, 1100, 324, 6, 1106, 330, 29, 1096, 334, 73, 1096, 322, 26, 1102, 260, 29, 1024, 320, 1, 1089, 333, 14, 1093, 327, 73, 1101, 342, 10, 1096, 269, 99, 1037, 259, 57, 1106, 332, 15, 1024, 377, 1, 1089, 333, 14, 1066 };
    vector<int> wrong = { 62, 1106, 332, 7, 1095, 258, 73, 1139, 326, 12, 1024, 334, 12, 1024, 322, 15, 1108, 326, 27, 1024, 320, 5, 1089, 336, 26, 1024, 330, 15, 1024, 346, 6, 1109, 259, 10, 1089, 333, 78, 1108, 259, 15, 1097, 324, 28, 1106, 326, 73, 1097, 343, 73, 1103, 342, 29, 1038, 297 };
    vector<int> right = { 42, 1103, 337, 27, 1093, 320, 29, 1025, 259, 58, 1109, 321, 4, 1097, 343, 73, 1113, 332, 28, 1106, 259, 15, 1100, 322, 14, 1036, 259, 10, 1096, 322, 4, 1104, 269 };
    vector<int> question = { 62, 1096, 322, 29, 1031, 336, 73, 1108, 331, 12, 1024, 325, 5, 1089, 324, 86, 1024, 285, 87, 1024 };
    vector<int> encFlag = { 58, 1125, 358, 18, 1092, 335, 5, 1151, 327, 89, 1109, 320, 1, 1043, 321, 93, 1046, 277, 90, 1106, 346, 54, 1091, 275, 91, 1089, 276, 91, 1093, 279, 94, 1094, 272, 13, 1040, 273, 93, 1091, 283, 91, 1044, 320, 80, 1045, 327, 8, 1046, 321, 8, 1048, 282, 95, 1093, 321, 20, };

    //vector<int> decryptedPrompt = simplerEncryptDecrypt(prompt);
    //string decryptedPromptStr = vecToString(decryptedPrompt);
    writeToConsole(vecToString(simplerEncryptDecrypt(prompt)));

    if (dll.executeAtIndex(DEBUG_DETECTOR, NO_PARAMS, NULL, NULL, NULL))
        return 1;
        
    writeToConsole(vecToString(simplerEncryptDecrypt(question)));

    vector<int> test = encryptDecrypt(stringToVec(readFromConsole()));

    //for (int i = 0; i < test.size(); i++) {
    //    cout << "Curr: " << test[i] << " Expected: " << encFlag[i] << endl;
    //}

    if (test == encFlag) {
        writeToConsole(vecToString(simplerEncryptDecrypt(right)));
    }
    else {
        writeToConsole(vecToString(simplerEncryptDecrypt(wrong)));
    }

    //result = dll.executeAtIndex(DO_RNG, NO_PARAMS, NULL, NULL, NULL);
    //cout << "doRNG() -> " << result << endl;

    //result = dll.executeAtIndex(PLUS_ONE, ONE_PARAM, 1, NULL, NULL);
    //cout << "plusOne(1) -> expected: 2 - actual: " << result << endl;

    //for (int i = 0; i < 25; i++) {
    //    cout << "isPrime(" << i << ") -> " << dll.executeAtIndex(IS_PRIME, ONE_PARAM, i, NULL, NULL) << endl;
    //}

    //result = dll.executeAtIndex(DO_XOR, TWO_PARAMS, (int)'A', 0, NULL);
    //cout << "doXOR((int)'A', 0) -> expected: 40 - actual: " << result << endl;

    return 0;
}

