#include <stdio.h>
#include <stdlib.h>

int main();
void vuln();

void vuln(){
	char input[20];
	gets(input);
	if (*(long long*)((long long) (&input)+0x28) == (long long)main+0x46){
		puts("Good  boi\n");
	}
	else{
		puts("Naughty Boi\n");
		exit(-1);
	}
}


int main(){
	char input[20];
	setbuf(stdout,0);
	setbuf(stdin,0);
	puts("I will let you  overflow me.");
	vuln();
	puts("I will give you one more chance.");
	fgets(input,8,stdin);
	puts(input);
	return 0;
}
void win(){
	system("cat flag");
	exit(0);
}
