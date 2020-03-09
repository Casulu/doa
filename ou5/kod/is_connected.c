#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "graph.h"
#include "dlist.h"

#define MAX_LINE_LENGTH 121

FILE *readFile(const char *name){
    FILE *in;
    /* open for reading */
    in = fopen(name, "r");
    if (in == NULL) {
        fprintf(stderr, "Failed to open %s for reading: %s\n",
                name, strerror(errno));
        return NULL;
    }
    /* do stuff with the file ... */
    return in;
}


int main(int argc, char *argv[]){
    FILE *map = readFile(argv[1]);
    fprintf("%s", map);
    char lineBuffer[MAX_LINE_LENGTH];
    while(fgets(lineBuffer, MAX_LINE_LENGTH, map)){
        printf("%s", lineBuffer);
    }
    return 0;
}

