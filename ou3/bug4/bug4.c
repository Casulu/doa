#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
  Ask the user for a string and encrypt it using the Ceaser cipher
  (a.k.a. ROT-13, https://en.wikipedia.org/wiki/Caesar_cipher). The
  user terminates the program by entering the string 'quit'.
*/

char *caesar(char *inString,int n) {
	int i=0;
	char *outString=(char *) malloc(strlen(inString)+1);
	while(inString[i]!='\0') {
		outString[i]=inString[i]+n;
		i++;
	}
	outString[i]='\0';
	return outString;
}

int main(void) {
	char *str=malloc(255);
	char *outString=NULL;
	bool running=true;
	printf("Enter a string to encrypt or write 'quit' to terminate\n");
	while(running) {
		fgets(str,255,stdin);
		outString=caesar(str,1);
		if(strncmp(str,"quit",4)==0) {
			free(outString);
			exit(0);
		}
		outString=caesar(str,1);
		printf("%s\n", outString);
	}
	free(str);
	return 0;
}
