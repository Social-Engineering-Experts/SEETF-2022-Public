// RC4 followed by binary XOR and then addition of fuzz

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 256
#define KEY_LEN 4
#define FLAG_LEN 59

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

typedef struct twoPowerStruct {
    int counter; // number of times to conduct multiplication
    int *x; // pointer to initial value called with
    int returnval; // return value
    unsigned long next; // which block to call next
} TWO_POWER_STRUCT, *TWO_POWER_STRUCT_PTR;

void entry_block(TWO_POWER_STRUCT *arg) {
    // handles entry point
    // returns 2^(initial value - 1)
    arg->returnval = 1;
    if (*(arg->x) == 0) {
        arg->next = 3;
        arg->counter = *(arg->x);
    } else {
        arg->next = 2;
        arg->counter = *(arg->x);
    }
    return;
}

void times_two(TWO_POWER_STRUCT *arg) {
    if (arg->counter == 0) {
        arg->next = 3;
    } else {
        arg->returnval = arg->returnval << 1;
        arg->counter = arg->counter - 1;
    }
}

void exit_block(TWO_POWER_STRUCT *arg) {
    // returns whichever value is generated
    arg->next = 4;
}

int two_power(int init_val) {
    TWO_POWER_STRUCT keygen;
    keygen.next = 1;
    keygen.x = &init_val;
    static void (*jump_table[4])(TWO_POWER_STRUCT_PTR) = {
        &entry_block,
        &times_two,
        &exit_block,
    };

    while (1) {
        if (keygen.next > 3) {
            return (keygen.returnval);
        } else {
            (*jump_table[keygen.next - 1])(&keygen);
        }
    }
}

int main(void) {
    int i;
    unsigned char key[KEY_LEN+1] = {2, 0, 3, 3};
    unsigned char enc_flag[] = {
        0xBF,0xFE,0x6A,0x5A,0xA7,0x82,0x8A,0x12,0xA6,0x45,0xA6,0x9D,0x78,0xF0,0x9B,0x9A,0xB9,0xC1,0xAA,0xB0,0x68,0x43,0x36,0xAB,0xCB,0xB2,0xAD,0xDD,0x1D,0x26,0x96,0x4B,0x32,0x14,0x6B,0xC5,0x78,0x73,0xE3,0xE1,0x5A,0xD6,0xA3,0xAB,0x9F,0x72,0xE0,0xEF,0x8D,0x16,0xD8,0xA4,0xEF,0x75,0xFB,0xB1,0xCE,0x1F,0x28
    };
    char user_input[64];

    for (i = 0; i < KEY_LEN; i++) {
        key[i] = two_power(key[i]) + 68;
    }

    puts("Welcome back! Enter your password.");
    fgets(user_input, 64, stdin);

    // printf("User input length: %d", strlen(user_input));

    if (strlen(user_input) != FLAG_LEN + 1) {
        puts("Incorrect! No hacking allowed.");
        return -1;
    }

    rc4(key, KEY_LEN, user_input, FLAG_LEN);

    for (i = 0; i < FLAG_LEN; i++) {
        user_input[i] = user_input[i] ^ 0x69;
        user_input[i] = user_input[i] + 43;
        // printf("0x%02X,",(unsigned char)user_input[i]);
    }

    if (memcmp(user_input, enc_flag, FLAG_LEN) == 0) {
        puts("Congratulations! There's your flag.");
        return 0;
    } else {
        puts("okay, kid im done. i doubt you even have basic knowlege of hacking. i doul boot linux so i can run my scripts. you made a big mistake of replying to my comment without using a proxy, because i'm already tracking youre ip. since ur so hacking iliterate, that means internet protocol. once i find your ip i can easily install a backdoor trojan into your pc, not to mention your email will be in my hands. dont even bother turning off your pc, because i can rout malware into your power system so i can turn your excuse of a computer on at any time. it might be a good time to cancel your credit card since ill have that too. if i wanted i could release your home information onto my secure irc chat and maybe if your unlucky someone will come knocking at your door. id highly suggest you take your little comment about me back since i am no script kiddie. i know java and c++ fluently and make my own scripts and source code. because im a nice guy ill give you a chance to take it back. you have 4 hours in unix time, clock is ticking. ill let you know when the time is up by sending you an email to [redacted] which I aquired with a java program i just wrote. see you then :)");
        return -1;
    }
}

