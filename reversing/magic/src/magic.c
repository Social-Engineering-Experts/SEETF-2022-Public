#include <stdio.h>
#include <string.h>
#include "Windows.h"
#include "rc4.h"

/*
def encrypt(plain) :
	key = 0x368d1306
	cipher = ''
	for i in plain :
		cipher += chr(ord(i) ^ (key & 0xff))
		key = rol(key, 8, 32)
	return cipher
*/


inline void antidebug2(){
	__try
	{
		RaiseException(DBG_PRINTEXCEPTION_C, 0, 0, 0);
	}
	__except (GetExceptionCode() == DBG_PRINTEXCEPTION_C)
	{
		return;
	}

	ExitProcess(-1);
}

void decryptInternal(char* plaintext) {
	unsigned int key = 0x368d1306;
	while (*plaintext) {
		*plaintext ^= (unsigned char)key;
		plaintext++;
		DWORD dwStart = timeGetTime();
		__asm rol key, 8
		if ((timeGetTime() - dwStart) > 1500) {
			ExitProcess(-1);
		}
		//antidebug2();
	}
	antidebug2();
}

void promptDecrypt(char* plaintext, char* key, int count) {
	for (int i = 0; i < count; i++) {
		*plaintext ^= (unsigned char)key[i % 21];
		DWORD dwStart = timeGetTime();
		plaintext++;
		if ((timeGetTime() - dwStart) > 1500) {
			ExitProcess(-1);
		}
	}
	antidebug2();
}

unsigned long djb2(unsigned char* str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

int main()
{
	// SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
	char regKey[] = "UyËGQwßVZ{äptYþ|`BÑDoXé|qEÑPsDÿvhBÛvtEä|hjßfh6";

	decryptInternal(regKey);
	HKEY hKey;
	LSTATUS result = RegOpenKeyExA(
		HKEY_LOCAL_MACHINE,
		regKey,
		NULL,
		KEY_QUERY_VALUE | KEY_WOW64_64KEY,
		&hKey
	);

	// SEEtf
	char keyName[] = "UsÈg`";
	decryptInternal(keyName);
	char valuePlain[21];
	DWORD len = 21;
	result = RegQueryValueExA(
		hKey,
		keyName,
		NULL,
		NULL,
		&valuePlain,
		&len
	);

	if (result != ERROR_SUCCESS) {
		return -1;
	}
	char valueEncrypted[21];
	char key[] = "\x14";
	RC4(key, valuePlain, valueEncrypted);
	
	// hunter_123456_qwerty
	char target[21] = { 0x49, 0x96, 0x97, 0x02, 0xa0, 0x17, 0xbe, 0xeb, 0x29, 0x48, 0x05, 0xe7, 0x61, 0xda, 0xec, 0x92, 0x29, 0xce, 0xd7, 0xe7 };
	if (strncmp(valueEncrypted, target,20)) {
		return -1;
	}
	
	//Welcome to the admin panel!
	// Please enter your 8 digit passcode:
	char prompt[] = { 0x3f,0x10,0x02,0x17,0x0a,0x1f,0x3a,0x11,0x46,0x5c,0x14,0x41,0x5e,0x3a,0x51,0x16,0x01,0x1f,0x1d,0x17,0x20,0x18,0x14,0x00,0x11,0x09,0x53,0x55,0x61,0x5e,0x56,0x55,0x46,0x53,0x7f,0x14,0x19,0x11,0x17,0x06,0x59,0x79,0x07,0x00,0x1c,0x54,0x5d,0x52,0x3b,0x58,0x55,0x5a,0x40,0x15,0x46,0x3e,0x02,0x04,0x06,0x1d,0x10,0x1c,0x3a,0x68 };
	promptDecrypt(prompt, valuePlain, 65);
	printf("%s",prompt);
	char input[9];
	fgets(input, 9, stdin);
	if (!atoi(input)) {
		printf("nope");
		return -1;
	}
	unsigned long inputHash = djb2(input);
	// 82457351
	if (inputHash == (unsigned long)0xb0241528) {
		if (IsDebuggerPresent()) {
			ExitProcess(-1);
		}
		char banner[] = "GUîvuE\xadttWãgcR¬\x19NSÿv!E\xadjiCÿ3`Zìt<\x16";
		decryptInternal(banner);
		// Access granted!
		printf("%s",banner);
		for (int i = 0; i < 8; i++) {
			DWORD c1 = GetTickCount();
			input[i] -= 10;
			if ((GetTickCount() - c1) > 1500) {
				ExitProcess(-1);
			}
		}
		char flag[] = { 0xea, 0x3a, 0x47, 0x3d, 0xa1, 0x9d, 0xd4, 0x81, 0x48, 0x81, 0x45, 0xfe, 0xf4, 0x0c, 0xdd, 0xab, 0x86, 0xc2, 0x99, 0xfd, 0x19, 0x83, 0xc7, 0x13, 0x54, 0x69, 0xaa, 0x4b, 0x4b, 0x18, 0x33, 0xd5, 0xe6, 0xd8 };
		RC4(input,flag,flag);
		if (IsDebuggerPresent()) {
			ExitProcess(-1);
		}
		printf("%s", flag);
		return 0;
	}
	printf("nope");
	return 0;
}

