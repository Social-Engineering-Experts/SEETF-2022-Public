#include "rc4.h"


void generateRC4SBox(uint8_t s[256], const uint8_t* key, int keylength)
{
    for (int i = 0; i < 256; i++)
        s[i] = i;

    uint8_t j = 0, swap;

    for (int i = 0; i < 256; i++)
    {
        j += s[i] + key[i % keylength]; //The addition is in fact modulo 256
        swap(s[i], s[j], swap);
    }
}

void RC4(const uint8_t* message, uint8_t* cipher, int start, int stop, uint8_t s[256], int8_t shift)
{
    uint8_t i = 0, j = 0, swap;

    for (int n = start; n < stop; n++)
    {
        j += s[++i]; //Note that i is effectively increased by one before the rest is evaluated
        swap(s[i], s[j], swap);
        if (message && cipher) {
            cipher[n] = j ^ s[(uint8_t)(s[i] + s[j])]; //Cast inside [..] is needed!
            if (shift & 0x80) {
                // if negative
                cipher[n] = (cipher[n] ^ ~message[n]) + shift;
            }
            else {
                // if positive
                cipher[n] = cipher[n] ^ ~(message[n] + shift);
            }
        }
          
    }
}

void RC4_ORG(const uint8_t* message, uint8_t* cipher, int start, int stop, uint8_t s[256])
{
    uint8_t i = 0, j = 0, swap;

    for (int n = start; n < stop; n++)
    {
        j += s[++i]; //Note that i is effectively increased by one before the rest is evaluated
        swap(s[i], s[j], swap);
        if (message && cipher)
            cipher[n] = s[(uint8_t)(s[i] + s[j])] ^ message[n]; //Cast inside [..] is needed!
    }
}