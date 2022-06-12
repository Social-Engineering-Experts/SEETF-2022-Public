#pragma once

#include <iomanip> 

#define swap(v1, v2, temp) temp = v1, v1 = v2, v2 = temp;


void generateRC4SBox(uint8_t s[256], const uint8_t* key, int keylength);


void RC4(const uint8_t* message, uint8_t* cipher, int start, int stop, uint8_t s[256], int8_t shift=0);

void RC4_ORG(const uint8_t* message, uint8_t* cipher, int start, int stop, uint8_t s[256]);