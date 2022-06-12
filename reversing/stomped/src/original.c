/*
By Sirus Shahini
	~cyn
sirus.shahini@gmail.com

Adapted for SEETF
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 256
#define KEY_LEN 4

unsigned char s[CHUNK_SIZE];
unsigned char t[CHUNK_SIZE];


void swap(unsigned char *p1, unsigned char *p2){
	unsigned char t = *p1;
	*p1 = *p2;
	*p2 = t;
}

void rc4_init(unsigned char *key, int key_len){
	int	i, j = 0;

	//Initial values of both vectors
	for (i = 0;i < CHUNK_SIZE; i++){
		s[i] = i;
		t[i] = key[i % key_len];
	}
	//Initial permutation
	for (i = 0;i < CHUNK_SIZE; i++){
		j = (j + s[i] + t[i]) % CHUNK_SIZE;
		swap(&s[i], &s[j]);
	}
}

void rc4(unsigned char *key, int key_len, char *buff, int len){
	int i = 0;
	unsigned long t1,t2;
	unsigned char val;
	unsigned char out;
	t1 = 0; t2 = 0;
	rc4_init(key,key_len);

	//process one byte at a time
	for (i = 0;i < len; i++){
		t1 = (t1 + 1) % CHUNK_SIZE;
		t2 = (t2 + s[t1]) % CHUNK_SIZE;
		swap(&s[t1], &s[t2]);
		val = (s[t1] + s[t2]) % CHUNK_SIZE;
		out = *buff ^ val;
		*buff=out;
		buff++;
	}
}

int main(int argc,char ** argv){
	char message[128];
	int message_len ;
	int i;
	unsigned char key[KEY_LEN + 1] = {4, 1, 8, 8};

	strcpy(message,"SEE{b0770m_up_pr06r4mm1n6_69899c7222d16fa315d556aa24bfb8b2}");
	message_len = strlen(message);

	for (i = 0;i < KEY_LEN; i++){
		key[i] = (char)(key[i] + 68);
	}

	key[KEY_LEN] = 0;
	printf("> Generated sample key: \033[96m%s\033[0m\n",key);
	printf("> Secret text: \033[1;94m%s\033[0m\n",message);
	printf("> Encrypted message: \033[1;94m");
	rc4(key, KEY_LEN, message, message_len);
	//print HEX encoding of the encrypted message
	for (i = 0; i < message_len; i++){
        message[i] = message[i] ^ 0x69;
        message[i] = message[i] + 43;
		printf("0x%02X,",(unsigned char)message[i]);
	}
	printf("\n\033[0m> Decrypted RC4 data with the same key: ");
    for (i = 0; i < message_len; i++)  {
        message[i] = message[i] - 43;
        message[i] = message[i] ^ 0x69;
    }
	rc4(key, KEY_LEN, message, message_len);
	printf("\033[1;92m%s\033[0m\n",message);

	return 0;
}

