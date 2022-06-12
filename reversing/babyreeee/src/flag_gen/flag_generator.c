#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encrypt(int a, int index) {
    // performs "random" encryption
    return (a + 69) ^ index;
}

int main(void) {
    char user_input[128];
    char* flag = "SEE{0n3_5m411_573p_81d215e8b81ae10f1c08168207fba396}";
    int flag_len = strlen(flag);

    for (int i = 0; i < flag_len; i++) {
        printf("%d, ", encrypt(flag[i], i));
    }

    return 0;
}