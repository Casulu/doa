#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "dlist.h"

#define MAX_LINE_LENGTH 300

/* Return position of first non-whitespace character or -1 if only
white-space is found. */
int first_non_white_space(const char *s)
{
    int i = 0; // Start at first char.
// Advance until we hit EOL as long as we're loooking at white-space.
    while (s[i] && isspace(s[i])) {
        i++;
    }
    if (s[i]) {
        return i; // Return position of found a non-white-space char.
    } else {
        return -1; // Return fail.
    }
}
/* Return position of last non-whitespace character or -1 if only
white-space is found. */
int last_non_white_space(const char *s)
{
// Start at last char.
    int i = strlen(s) - 1;
// Move back until we hit beginning-of-line as long as we're
// loooking at white-space.
    while (i >= 0 && isspace(s[i])) {
        i--;
    }
    if (i >= 0) {
        return i; // Return position of found a non-white-space char.
    } else {
        return -1; // Return fail.
    }
}

/* Return true if s only contains whitespace */
bool line_is_blank(const char *s)
{
// Line is blank if it only contained white-space chars.
    return first_non_white_space(s) < 0;
}
/* Return true if s is a comment line, i.e. first non-whitespc char is '#' */
bool line_is_comment(const char *s)
{
    int i = first_non_white_space(s);
    return (i >= 0 && s[i] == '#');
}

int firstCommentSign(const char *s){
    int i = first_non_white_space(s);
    while(s[i] != '#' && s[i]){
        i++;
    }
    if(i > last_non_white_space(s)){
        return -1;
    } else{

    }

}

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


char **cleanFile(FILE *map)
{
    char lineBuffer[MAX_LINE_LENGTH];
    char **output;
    bool numIsRead = false;
    bool reachedEdge = false;
    int n = 0;
    while(fgets(lineBuffer, MAX_LINE_LENGTH, map) && !numIsRead){
        if(!line_is_blank(lineBuffer) && !line_is_comment(lineBuffer)){
            numIsRead = true;
            int start = first_non_white_space(lineBuffer);
            if(isdigit(lineBuffer[start])){
                n = atoi(&lineBuffer[start]);
            } else{
                //error
            }
        }
    }
    output = malloc(sizeof(**output) * (n + 1));
    output[0] = malloc(sizeof(char) * MAX_LINE_LENGTH);
    sprintf(output[0], "%d", n);
    for (int i = 0; i < n; ++i) {
        reachedEdge = false;
        output[i] = malloc(8 * sizeof(char));
        while(fgets(lineBuffer, MAX_LINE_LENGTH, map) && !reachedEdge){
            if(!line_is_blank(lineBuffer) && !line_is_comment(lineBuffer)){
                reachedEdge = true;
            }
        }
        //Nodnamn max 40 tecken
        if(reachedEdge) {
            int start = first_non_white_space(lineBuffer);
            int j = 0;
            while (lineBuffer[j + start] && isspace(lineBuffer[j + start])) {
                output[i][j] = lineBuffer[j + start];
                j++;
            }
            while (isspace(lineBuffer[j + start])) {
                start++;
            }
            while (lineBuffer[j + start] && !isspace(lineBuffer[j + start]) && lineBuffer[j + start] != '#') {
                output[i][j] = lineBuffer[j + start];
                j++;
            }
            output[i][j] = '\0';
        }
    }
    return output;

}


graph *addNodes(graph *g, const char **cleanMap)
{
    for (int i = 1; i < atoi(cleanMap[0]) + 1; ++i) {
        char lbl1[40];
        char lbl2[40];
        sscanf(cleanMap[i], "%s %s", lbl1, lbl2);
        if(graph_find_node(g, lbl1) == NULL){
            graph_insert_node(g, lbl1);
        }
        if(graph_find_node(g, lbl2) == NULL){
            graph_insert_node(g, lbl2);
        }
    }
    return g;
}

graph *addEdges(graph *g, const char **cleanMap){
    for (int i = 1; i < atoi(cleanMap[0]) + 1; ++i) {
        char lbl1[40];
        char lbl2[40];
        sscanf(cleanMap[i], "%s %s", lbl1, lbl2);
        node *n1 = graph_find_node(g, lbl1);
        node *n2 = graph_find_node(g, lbl2);
        if(!nodes_are_equal(n1, n2)){

        }

    }


    return g;
}

int main(int argc, char *argv[]){
    FILE *map = readFile(argv[1]);
    char **cleanMap = cleanFile(map); //Allocates
    fclose(map);

    graph *g = graph_empty(atoi(cleanMap[0]));
    g = addNodes(g, cleanMap);

    return 0;
}

