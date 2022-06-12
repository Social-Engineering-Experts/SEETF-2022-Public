#include <string.h>
#include "Windows.h"

inline void antidebug1() {
	unsigned char* tmpBuf = malloc(10);
	if (tmpBuf[10] == 0xAB && tmpBuf[11] == 0xAB) {
		free(tmpBuf);
		ExitProcess(-1);
	}
	free(tmpBuf);
}

void swap(unsigned char* a, unsigned char* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int KSA(char* key, unsigned char* S) {

    antidebug1();
    int len = strlen(key);
    int j = 0;

    for (int i = 0; i < 256; i++)
        S[i] = i;

    for (int i = 0; i < 256 ; i++) {
        j = (j + S[i] + key[i % len]) % 256;

        swap(&S[i], &S[j]);
    }

    return 0;
}

int PRGA(unsigned char* S, char* plaintext, unsigned char* ciphertext) {

    int i = 0;
    int j = 0;

    for (size_t n = 0, len = strlen(plaintext); n < len; n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % 256];

        ciphertext[n] = rnd ^ plaintext[n];

    }

    return 0;
}

int RC4(char* key, char* plaintext, unsigned char* ciphertext) {

    unsigned char S[256];
    KSA(key, S);

    PRGA(S, plaintext, ciphertext);

    return 0;
}