#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
  Revised Challenge Created by Louis (L0j0) from baby_hof
*/


void setup_IO() {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
}

int print_statements(int counter) {
  printf("Welcome to the Hall of Fame!\n\n");
  printf("Number of Hall Of Famers: %d\n", counter);
  printf("What brings you in here?\n\n");
  printf("1) Add Hall of Famer\n2) View Position\n3) Exit\n");
  return 0;
}

int main() {
  int counter = 0;
  void* heap_pointer = sbrk(0);
  setup_IO();

  while (1) {
    print_statements(counter);
    printf("Choose> ");
    fflush(stdout);
    char option[3];
    if (fgets(option, 3, stdin)) {
	fflush(stdin);
	option[strcspn(option, "\n")] = 0;
    } else {
        continue;
    }
    int o = atoi(option);

    switch (o) {
        case 1: {
            printf("\nHow many points did this person score? > ");
	    fflush(stdout);
            char score[64];
            if (fgets(score, 64, stdin)) {
	       fflush(stdin);
               score[strcspn(score, "\n")] = 0;
            } else {
               break;
            }
            long unsigned size;
            char* pty;
            size = strtol(score, &pty, 10);
            char *ptr = malloc(size);
            char * chunk;
            chunk = ptr;
            printf("\nWho is this Hall of Famer > ");
	    fflush(stdout);
            char word[100];
            fgets(word, 100, stdin);
	    fflush(stdin);
            memcpy(chunk, word, 100);
            heap_pointer = ptr;
            counter++;
            break;
        }

        case 2:
            printf("\nThe position of latest addition is at %p\n", heap_pointer);
            printf("The position of PUTS is at %p\n", puts);
            break;

        case 3:
            printf("Exiting software...\n");
            return 0;

        default:
            printf("No choice Given!\n");
            break;
    }

  }

  return 0;
}
