#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encrypt(int a, int index) {
    // performs "random" encryption
    return (a + 69) ^ index;
}

int main(void) {
    char user_input[128];
    printf("Hello! Welcome to SEETF. Please enter the flag.\n");
    int test_arr[] = {152, 139, 136, 195, 113, 182, 126, 163, 114, 187, 115, 125, 122, 169, 116, 115, 104, 164, 182, 110, 98, 188, 97, 97, 98, 179, 103, 188, 97, 107, 184, 181, 86, 84, 137, 85, 140, 80, 91, 81, 83, 84, 93, 94, 80, 134, 137, 137, 72, 79, 73, 241,};
    size_t arr_size = sizeof(test_arr) / sizeof(int);
    
    fgets(user_input, 128, stdin);
    if (strlen(user_input) - 1 != arr_size) {
        printf("Flag wrong. Try again.");
        return 1;
    } else {
        printf("Good work! Your flag is the correct size.\n");
        printf("On to the flag check itself...\n");
        for (int j = 0; j < strlen(user_input) - 1; j++) {
            char user_char, test_char;
            test_char = test_arr[j];
            user_char = encrypt(user_input[j], j);

            if (user_char != test_char) {
                printf("Flag check failed at index: %d", j);
                return 1;
            }
        }

        printf("Success! Go get your points, champ.\n");
    }

    return 0;
}