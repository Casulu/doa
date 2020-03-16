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


int compTuple(const void *tuple1, const void *tuple2)
{
    int i1 = *(int*)array_1d_inspect_value((array_1d*)tuple1, 0);
    int j1 = *(int*)array_1d_inspect_value((array_1d*)tuple1, 1);

    int i2 = *(int*)array_1d_inspect_value((array_1d*)tuple2, 0);
    int j2 = *(int*)array_1d_inspect_value((array_1d*)tuple2, 1);

    if(i1 == i2 && j1 == j2){
        return 0;
    } else{
        return -1;
    }
}

/* Return position of first non-whitespace character or -1 if only
white-space is found. */
int firstNonWhiteSpace(const char *s)
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
int lastNonWhiteSpace(const char *s)
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
bool lineIsBlank(const char *s)
{
// Line is blank if it only contained white-space chars.
    return firstNonWhiteSpace(s) < 0;
}

/* Return true if s is a comment line, i.e. first non-whitespc char is '#' */
bool lineIsComment(const char *s)
{
    int i = firstNonWhiteSpace(s);
    return (i >= 0 && s[i] == '#');
}

// Returns the index of the first comment sign or the last non-whitespace if no comment sign is found
int firstCommentSign(const char *s){
    int i = firstNonWhiteSpace(s);
    while(s[i] != '#' && s[i]){
        i++;
    }

    int end = lastNonWhiteSpace(s);
    if(i > end){
        return end;
    } else{
        return i;
    }

}

/*Opens a file for reading while checking for errors. Returns the opened FILE pointer */
FILE *readFile(const char *name){
    FILE *in;
    /* open for reading */
    in = fopen(name, "r");
    if (in == NULL) {
        fprintf(stderr, "Failed to open %s for reading: %s\n",
                name, strerror(errno));
        exit(1);
    }
    /* do stuff with the file ... */
    return in;
}


array_1d *cleanFile(FILE *map)
{
    char lineBuffer[MAX_LINE_LENGTH];
    array_1d *cleanMap;
    bool numIsRead = false;
    bool reachedEdge = false;
    int numEdges; //Number of edges
    int n1Length; //Buffer for the lenght of the first node in an edge-line

    //This block looks for the first non-ignored line and grabs the number of lines. Return
    while(!numIsRead && fgets(lineBuffer, MAX_LINE_LENGTH, map)){
        if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
            numIsRead = true;
            bool badLine = false;
            int start = firstNonWhiteSpace(lineBuffer);
            for (int i = 0; i <= firstCommentSign(lineBuffer); ++i) {
                if(!isdigit(lineBuffer[start])){
                   badLine = true;
                }
            }
            if(!badLine){
                numEdges = atoi(lineBuffer);
            } else{
                fprintf(stderr, "ERROR: First line contained %s which is not a number",
                        lineBuffer);
                exit(EXIT_FAILURE);
            }
        }
    }

    //Create an array with space for only the flight-data lines plus the line with the number of edges
    cleanMap = array_1d_create(0, numEdges, free);

    //Allocate memory for the first line with number of edges and set it to the read value
    array_1d_set_value(cleanMap, malloc(sizeof(char) * MAX_LINE_LENGTH), 0);
    sprintf(array_1d_inspect_value(cleanMap, 0), "%d", numEdges);
    int i = 1;
    while (fgets(lineBuffer, MAX_LINE_LENGTH, map)) {
        reachedEdge = false;
        char *currLine = malloc(82 * sizeof(char));
        do {
            if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
                reachedEdge = true;
            }
        } while(!reachedEdge && fgets(lineBuffer, MAX_LINE_LENGTH, map));

        //Nodnamn max 40 tecken
        if(reachedEdge) {
            int start = firstNonWhiteSpace(lineBuffer);
            int j = 0;
            while (lineBuffer[j + start] && !isspace(lineBuffer[j + start]) && j <= 39) {
                if(isalnum(lineBuffer[j + start])){
                    currLine[j] = lineBuffer[j + start];
                    j++;
                } else{
                    fprintf(stderr, "ERROR: Edge line\n%scontained non alphanumerical character '%c'", lineBuffer, lineBuffer[j + start]);
                    exit(EXIT_FAILURE);
                }
            }
            if(j > 40){
                free(currLine);
                break;
            }

            currLine[j] = ' ';
            n1Length = j + 1;
            while (isspace(lineBuffer[j + start])) {
                start++;
            }
            start--;
            j++;

            while (lineBuffer[j + start] && !isspace(lineBuffer[j + start]) && lineBuffer[j + start] != '#' && j <= 78 - n1Length -1) {
                if(isalnum(lineBuffer[j + start])){
                    currLine[j] = lineBuffer[j + start];
                    j++;
                } else{
                    fprintf(stderr, "ERROR: Edge line\n%scontained non alphanumerical character '%c'", lineBuffer, lineBuffer[j + start]);
                    exit(EXIT_FAILURE);
                }
            }
            if(j >  78 - n1Length -1){
                free(currLine);
                break;
            }
            currLine[j] = '\0';
        }
        for (int k = 0; k < i; ++k) {
            if(strcmp(currLine, array_1d_inspect_value(cleanMap, k)) == 0){
                fprintf(stderr, "File had duplicate edges of %s", currLine);
                exit(EXIT_FAILURE);
            }
        }
        if(i <= array_1d_high(cleanMap)){
            array_1d_set_value(cleanMap, currLine, i);
            i++;
        } else{
            fprintf(stderr, "ERROR: File stated %d edges but file had %d", numEdges, i);
            exit(EXIT_FAILURE);
        }
    }

    if(!array_1d_has_value(cleanMap, array_1d_high(cleanMap))){
        fprintf(stderr, "ERROR: Number of edges stated did not match number given (%d)", numEdges);
    }
    return cleanMap;

}

array_1d *getLabels(const array_1d *cleanMap)
{
    int numEdges = atoi(array_1d_inspect_value(cleanMap, 0));
    array_1d *labels = array_1d_create(0, 2 * numEdges, NULL);
    int firstFreeIndex = 0;
    bool lbl1Exists;
    bool lbl2Exists;

    for (int i = 1; i < numEdges + 1; ++i) {
        char *lbl1 = calloc(sizeof(char) , 41);
        char *lbl2 = calloc(sizeof(char), 41);
        char *currLine = array_1d_inspect_value(cleanMap, i);
        sscanf(currLine, "%s %s", lbl1, lbl2);

        lbl1Exists = false;
        lbl2Exists = false;


        for (int j = 0; j < firstFreeIndex && (!lbl1Exists || !lbl2Exists); ++j) {
            char *currLabel = array_1d_inspect_value(labels, j);
            if(strcmp(currLabel, lbl1) == 0){
                lbl1Exists = true;
            }
            if(strcmp(currLabel, lbl2) == 0){
                lbl2Exists = true;
            }
        }
        if(!lbl1Exists){
            array_1d_set_value(labels, lbl1, firstFreeIndex);
            firstFreeIndex++;
        }
        if(!lbl2Exists && strcmp(lbl1, lbl2) != 0){
            array_1d_set_value(labels, lbl2, firstFreeIndex);
            firstFreeIndex++;
        } else {
            free(lbl2);
        }
        if(lbl1Exists){
            free(lbl1);
        }
    }
    array_1d_set_value(labels, NULL, firstFreeIndex);
    //for (int j = 0; j < firstFreeIndex; ++j) {
    //    printf("%s ", array_1d_inspect_value(labels, j));
    //}
    //putchar('\n');
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
        char *lbl1 = calloc(sizeof(char), 41);
        char *lbl2 = calloc(sizeof(char), 41);
        char *currLine = array_1d_inspect_value(cleanMap, i);
        sscanf(currLine, "%s %s", lbl1, lbl2);

        node *n1 = graph_find_node(g, lbl1);
        node *n2 = graph_find_node(g, lbl2);

        //printf("%s\n", lbl1);
        if(!nodes_are_equal(n1, n2)){
            graph_insert_edge(g, n1, n2);
        }
        free(lbl1);
        free(lbl2);
        //printf("\n");

    }
    return g;
}

void tupleFreeFunc(void *tuple){
    array_1d_kill(tuple);
}

table *getMatrix(graph *g, const array_1d *labels)
{
    table *output = table_empty(compTuple, tupleFreeFunc, free);
    bool lblFound;
    int j = 0;
    int i = 0;

    while(array_1d_inspect_value(labels, j) != NULL) {
        node *currNode = graph_find_node(g, array_1d_inspect_value(labels, j));
        dlist *currNeighbours = graph_neighbours(g, currNode);
        dlist_pos pos = dlist_first(currNeighbours);

        array_1d *tuple = array_1d_create(0, 1, free);
        int *pj1 = calloc(sizeof(int), 1);
        int *pj2 = calloc(sizeof(int), 1);
        *pj1 = j;
        *pj2 = j;

        array_1d_set_value(tuple, pj1, 0);
        array_1d_set_value(tuple, pj2, 1);
        int *neighbour = calloc(sizeof(int), 1);
        *neighbour = 1;
        table_insert(output, tuple, neighbour);


        while(!dlist_is_end(currNeighbours, pos)){
            i = 0;
            lblFound = false;

            while(array_1d_inspect_value(labels, i) != NULL && !lblFound){
                if(nodes_are_equal(graph_find_node(g, array_1d_inspect_value(labels, i)), dlist_inspect(currNeighbours, pos))){
                    lblFound = true;
                } else{
                    i++;
                }
            }

            array_1d *tuple = array_1d_create(0, 1, free);
            int *pi = calloc(sizeof(int), 1);
            *pi = i;
            int *pj = calloc(sizeof(int), 1);
            *pj = j;

            array_1d_set_value(tuple, pi, 0);
            array_1d_set_value(tuple, pj, 1);
            int *neighbour = calloc(sizeof(int), 1);
            *neighbour = 1;
            table_insert(output, tuple, neighbour);
            pos = dlist_next(currNeighbours, pos);
        }
        dlist_kill(currNeighbours);
        j++;
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
    g = addNodes(g, labels);
    g = addEdges(g, cleanMap);
    table *matrix = getMatrix(g, labels);
/*    int i = 0;
    int j = 0;

    printf("   ");
    while(array_1d_inspect_value(labels, i) != NULL){
        printf("%s ", (char*)array_1d_inspect_value(labels, i));
        i++;
    }
    printf("\n");
    i = 0;
    while(array_1d_inspect_value(labels, i) != NULL){
        printf("%s ", (char*)array_1d_inspect_value(labels, i));
        j = 0;
        while(array_1d_inspect_value(labels, j) != NULL){
            int *matrixValue = (int*)tupleLookup(matrix, i, j);
            if(matrixValue != NULL){
                printf("%d   ", *(int*)matrixValue);
            } else{
                printf("%d   ", 0);
            }
            j++;
        }
        printf("\n");
        i++;
    }*/

    int k = 0;
    char *killLabel = (char*)array_1d_inspect_value(labels, k);
    while(killLabel != NULL){
        free(killLabel);
        k++;
        killLabel = (char*)array_1d_inspect_value(labels, k);
    }
    array_1d_kill(labels);

    graph_kill(g);
    array_1d_kill(cleanMap);

    table_kill(matrix);
    return 0;
}

