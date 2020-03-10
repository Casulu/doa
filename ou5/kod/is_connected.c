#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "table.h"
#include "array_1d.h"

#define MAX_LINE_LENGTH 300


int compTuple(const void *int1, const void *int2)
{
    if(*(int*)int1 == *(int*)int2){
        return 0;
    } else if(*(int*)int1 < *(int*)int2){
        return -1;
    } else{
        return 1;
    }
}

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
    int end = last_non_white_space(s);
    if(i > end){
        return end;
    } else{
        return i;
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


array_1d *cleanFile(FILE *map)
{
    char lineBuffer[MAX_LINE_LENGTH];
    array_1d *output;
    bool numIsRead = false;
    bool reachedEdge = false;
    int n = 0;

    while(!numIsRead && fgets(lineBuffer, MAX_LINE_LENGTH, map)){
        if(!line_is_blank(lineBuffer) && !line_is_comment(lineBuffer)){
            numIsRead = true;
            bool badLine = false;
            int start = first_non_white_space(lineBuffer);
            for (int i = 0; i <= firstCommentSign(lineBuffer); ++i) {
                if(!isdigit(lineBuffer[start])){
                   badLine = true;
                }
            }
            if(!badLine){
                n = atoi(lineBuffer);
            } else{
                //error
            }
        }
    }


    output = array_1d_create(0, n, NULL);
    array_1d_set_value(output, malloc(sizeof(char) * MAX_LINE_LENGTH), 0);
    sprintf(array_1d_inspect_value(output, 0), "%d", n);
    int n1Length;

    for (int i = 1; i < n + 1; ++i) {
        reachedEdge = false;
        array_1d_set_value(output, malloc(82 * sizeof(char)), i);
        while(!reachedEdge && fgets(lineBuffer, MAX_LINE_LENGTH, map)){
            if(!line_is_blank(lineBuffer) && !line_is_comment(lineBuffer)){
                reachedEdge = true;
            }
        }
        //Nodnamn max 40 tecken
        if(reachedEdge) {
            int start = first_non_white_space(lineBuffer);
            int j = 0;
            while (lineBuffer[j + start] && !isspace(lineBuffer[j + start]) && j <= 39) {
                ((char*)array_1d_inspect_value(output, i))[j] = lineBuffer[j + start];
                j++;
            }
            if(j > 39){
                break;
            }
            n1Length = j + 1;
            while (isspace(lineBuffer[j + start])) {
                start++;
            }
            start--;
            ((char*)array_1d_inspect_value(output, i))[j] = ',';
            j++;
            while (lineBuffer[j + start] && !isspace(lineBuffer[j + start]) && lineBuffer[j + start] != '#' && j <= 78 - n1Length -1) {
                ((char*)array_1d_inspect_value(output, i))[j] = lineBuffer[j + start];
                j++;
            }
            if(j > 39){
                break;
            }
            ((char*)array_1d_inspect_value(output, i))[j] = '\0';
        }
    }
    return output;

}

array_1d *getLabels(const array_1d *cleanMap)
{
    array_1d *labels = array_1d_create(0, atoi(array_1d_inspect_value(cleanMap, 0)) + 1, NULL);
    int firstFreeIndex = 0;
    bool lbl1Exists = false;
    bool lbl2Exists = false;
    const char token[2] = ",";

    for (int i = 1; i < atoi(array_1d_inspect_value(cleanMap, 0)) + 1; ++i) {
        char *lbl1 = calloc(sizeof(char) , 41);
        char *lbl2 = calloc(sizeof(char), 41);
        strcpy(lbl1, strtok(array_1d_inspect_value(cleanMap, i), token));
        strcpy(lbl2, strtok(NULL, token));
        //sscanf((char*)array_1d_inspect_value(cleanMap, i), "%s,%s", lbl1, lbl2);
        for (int j = 0; j < firstFreeIndex && (!lbl1Exists || !lbl2Exists); ++j) {
            if(strcmp(array_1d_inspect_value(labels, j), lbl1) == 0){
                lbl1Exists = true;
            }
            if(strcmp(array_1d_inspect_value(labels, j), lbl2) == 0){
                lbl2Exists = true;
            }
        }
        if(!lbl1Exists){
            array_1d_set_value(labels, lbl1, firstFreeIndex);
            firstFreeIndex++;
        } else{
            free(lbl1);
        }
        if(!lbl2Exists){
            array_1d_set_value(labels, lbl2, firstFreeIndex);
            firstFreeIndex++;
        } else {
            free(lbl2);
        }
    }
    array_1d_set_value(labels, NULL, firstFreeIndex);
    return labels;
}


graph *addNodes(graph *g, const array_1d *labels)
{
    int i = 0;
    while(array_1d_inspect_value(labels, i) != NULL){
        graph_insert_node(g, array_1d_inspect_value(labels, i));
        i++;
    }
    return g;
}

graph *addEdges(graph *g, const array_1d *cleanMap){
    for (int i = 1; i < atoi(array_1d_inspect_value(cleanMap, 0)) + 1; ++i) {
        char lbl1[40];
        char lbl2[40];
        sscanf(array_1d_inspect_value(cleanMap, i), "%s %s", lbl1, lbl2);

        node *n1 = graph_find_node(g, lbl1);
        node *n2 = graph_find_node(g, lbl2);

        if(!nodes_are_equal(n1, n2)){
            graph_insert_edge(g, n1, n2);
        }

    }
    return g;
}

table *getMatrix(graph *g, const array_1d *labels)
{
    table *output = table_empty(compTuple , free, free);
    bool lblFound;
    int j;
    int i = 0;

    while(array_1d_inspect_value(labels, i) != NULL) {
        node *currNode = graph_find_node(g, array_1d_inspect_value(labels, i));
        dlist *currNeighbours = graph_neighbours(g, currNode);
        dlist_pos pos = dlist_first(currNeighbours);

        while(!dlist_is_end(currNeighbours, pos)){
            j = 0;
            lblFound = false;

            while(array_1d_inspect_value(labels, j) != NULL && !lblFound){
                if(nodes_are_equal(graph_find_node(g, array_1d_inspect_value(labels, j)), currNode)){
                    lblFound = true;
                }
                j++;
            }

            array_1d *tuple = array_1d_create(0, 1, free);
            int *pi = calloc(sizeof(int), 1);
            *pi = i;
            int *pj = calloc(sizeof(int), 1);
            *pj = j;

            array_1d_set_value(tuple, pi, 0);
            array_1d_set_value(tuple, pj, 1);
            int *neighbour = calloc(sizeof(int), 1);
            *neighbour = (int)lblFound;
            table_insert(output, tuple, neighbour);
        }
        i++;
    }
    return output;
}

void *tupleLookup(table *t, int i, int j)
{
    array_1d *tuple = array_1d_create(0, 1, free);
    int *pi = malloc(sizeof(*pi));
    *pi = i;
    array_1d_set_value(tuple, pi, 0);

    int *pj = malloc(sizeof(*pj));
    *pj = j;
    array_1d_set_value(tuple, pj, 1);

    void *value = table_lookup(t, tuple);
    array_1d_kill(tuple);
    return value;
}

int main(int argc, char *argv[]){
    FILE *map = readFile(argv[1]);
    array_1d *cleanMap = cleanFile(map); //Allocates
    fclose(map);

    graph *g = graph_empty(atoi(array_1d_inspect_value(cleanMap, 0)));
    array_1d *labels = getLabels(cleanMap);
    g = addNodes(g, cleanMap);
    g = addEdges(g, cleanMap);
    table *matrix = getMatrix(g, labels);
    printf("%d, %s %s", *(int*)tupleLookup(matrix, 0, 2), (char*)array_1d_inspect_value(labels, 0), (char*)array_1d_inspect_value(labels, 2));

    return 0;
}

