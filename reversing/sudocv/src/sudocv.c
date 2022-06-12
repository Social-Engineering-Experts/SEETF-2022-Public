// gcc sudocv.c -O3 -s -o sudocv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC(m, n) (abs((m)+(n)-4)<=5)

char grid[82];
int init[81] = { 262657,263169,264193,266242,270338,278530,294916,327684,393220,524801,525313,526337,528386,532482,540674,557060,589828,655364,1049089,1049601,1050625,1052674,1056770,1064962,1081348,1114116,1179652,2097672,2098184,2099208,2101264,2105360,2113552,2129952,2162720,2228256,4194824,4195336,4196360,4198416,4202512,4210704,4227104,4259872,4325408,8389128,8389640,8390664,8392720,8396816,8405008,8421408,8454176,8519712,16777792,16778304,16779328,16781440,16785536,16793728,16810240,16843008,16908544,33555008,33555520,33556544,33558656,33562752,33570944,33587456,33620224,33685760,67109440,67109952,67110976,67113088,67117184,67125376,67141888,67174656,67240192 };
int perm[80] = { 62,15,32,78,18,15,39,65,29,29,45,71,61,17,36,72,54,78,30,20,70,69,23,61,37,52,27,66,68,30,58,33,42,76,51,36,66,48,48,53,58,69,55,50,49,63,68,61,66,50,71,67,61,74,55,75,67,68,75,74,73,80,73,68,71,67,74,72,79,73,74,77,76,76,77,79,79,78,80,80 };

const char* criteria[] = {
    "attacks lattices",
    "has stealth tactics",
    "has the latest tech",
    "hits the call stack",
    "is leet",
    "leaks assets",
    "likes haskell",
    "likes kali",
    "sets a cheat sheet",
    "takes shell access"
};

int check1() {
    int foo[256] = {};
    const int target = 0x7FFFFFF;

    for (int i = 0; i < 81; i++)
        foo[grid[i]] |= init[i];

    int good = 0;
    for (int i = 0; i < 256; i++)
        if (foo[i] == target) good++;

    return good;
}

int canFindWordStartStep(const char* word, int len, int start, int step) {
    for (int i = 0; i < len; i++)
        if (grid[start + step * i] != word[i])
            return 0;
    return 1;
}

int canFindWord(const char* word, int len) {
    for (int start = 0; start < 81; start++)
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
                if (dx || dy)
                    if (MAGIC(start % 9, dx * len))
                        if (MAGIC(start / 9, dy * len))
                            if (canFindWordStartStep(word, len, start, dy * 9 + dx))
                                return 1;
    return 0;
}

int canFindPhrase(const char* phrase) {
    const char* tmp;
    while (tmp = strchr(phrase, ' '))
    {
        if (!canFindWord(phrase, tmp - phrase)) return 0;
        phrase = tmp + 1;
    }
    return canFindWord(phrase, strlen(phrase));
}

int check2() {
    printf("Scanning against the ethical hacks checklist:\n");
    int good = 0;
    for (int i = 0; i < 10; i++) {
        int win = canFindPhrase(criteria[i]);
        good += win;
        printf("[%c] %s\n", " X"[win], criteria[i]);
    }
    return good;
}

//int (*func)(int);
int g1(int i) { return i / 9 * 18 + 8 - i; }
int g2(int i) { return i / 9 + i % 9 * 9; }

void canonical(int (*f1)(int), int (*f2)(int)) {
    if ((grid[69] & 3) == 3) return;
    for (int i = 0; i < 81; i++) {
        int j = f1(i);
        if (i < j)
            grid[i] ^= grid[j] ^= grid[i] ^= grid[j];
    }
    return canonical(f2, f1);
}

void printflag(){
    canonical(g1, g2);
    for (int i = 0; i < 80; i++)
        grid[i] ^= grid[perm[i]] ^= grid[i] ^= grid[perm[i]];

    char* text = grid + (grid[0] ^ grid[1]) + (grid[2] ^ grid[3]);
    for (int i = grid[4] ^ grid[5] ^ grid[6] ^ grid[7]; i > 0; i--)
        text[grid[4 * i + 4] ^ grid[4 * i + 5] ^ grid[4 * i + 6] ^ grid[4 * i + 7]] = '_';
    printf("SEE{%s}\n", text);
}


int main(void) {

    printf("Please submit your (sudo)cv:\n");
    fgets(grid, 82, stdin);

    //strcpy_s(grid, "etichalskkcatslehislhiekatclhtkicseaikslaehctcaehtskilhekactilstslekicahaicslhtke");
    //strcpy_s(grid, "ehsltacikkalicethstcikhsaelhitsecklalkctaieshseahlkitcclkesthaiiseakhlctathcilske");

    if (check1() < 9)
        printf("Invalid sudocv format.\n");
    else {
        int c2 = check2();
        printf("\nCandidate score: %i/10.\n", c2);

        if (c2 < 5)
            printf("You FAIL!\n");
        else {
            printf("You PASS! Here is your flag:\n");
            printflag();
        }
    }
}