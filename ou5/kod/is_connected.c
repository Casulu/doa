#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "table.h"
#include "array_1d.h"
#include "queue.h"

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
    int i = 0;
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

int lastNonWhiteSpaceNoComment(const char *s)
{
// Start at last char.
    int i = firstCommentSign(s);
// Move back until we hit beginning-of-line as long as we're
// looking at white-space.
    while (i >= 0 && isspace(s[i])) {
        i--;
    }
    if (i >= 0) {
        return i; // Return position of found a non-white-space char.
    } else {
        return -1; // Return fail.
    }
}

/*Opens a file for reading while checking for errors. Returns the opened FILE pointer */
FILE *readFile(const char *name){
    FILE *in;
    /* open for reading */
    in = fopen(name, "r");
    if (in == NULL) {
        fprintf(stderr, "ERROR: Failed to open %s for reading: %s\n",
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
    int n1Length; //Buffer for the length of the first node in an edge-line

    //This block looks for the first non-ignored line and grabs the number of lines. Return
    while(!numIsRead && fgets(lineBuffer, MAX_LINE_LENGTH, map)){
        if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
            numIsRead = true;
            bool badLine = false;
            int offset = firstNonWhiteSpace(lineBuffer);
            for (int i = 0; i <= firstCommentSign(lineBuffer); ++i) {
                if(!isdigit(lineBuffer[offset])){
                   badLine = true;
                }
            }
            if(!badLine){
                numEdges = atoi(lineBuffer);
            } else{
                fprintf(stderr, "ERROR: First line contained %s which is not a number\n",
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
        char *currLine = malloc(83 * sizeof(char));
        do {
            if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
                reachedEdge = true;
            }
        } while(!reachedEdge && fgets(lineBuffer, MAX_LINE_LENGTH, map));

        //Nodnamn max 40 tecken
        if(reachedEdge) {
            int offset = firstNonWhiteSpace(lineBuffer);
            int j = 0;
            while (lineBuffer[j + offset] && !isspace(lineBuffer[j + offset]) && j <= 40) {
                if(isalnum(lineBuffer[j + offset])){
                    currLine[j] = lineBuffer[j + offset];
                    j++;
                } else{
                    fprintf(stderr, "ERROR: Edge line\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(j > 41){
                fprintf(stderr, "ERROR: Node '%s' contains more than 40 characters", currLine);
                exit(EXIT_FAILURE);
            }

            currLine[j] = ' ';
            n1Length = j - 1;
            while (isspace(lineBuffer[j + offset + 1])) {
                offset++;
            }
            j++;
            if(lastNonWhiteSpaceNoComment(lineBuffer) == j + offset){
                fprintf(stderr, "ERROR: Line:\n %s only contains one node", lineBuffer);
                exit(EXIT_FAILURE);
            }

            while (lineBuffer[j + offset] && !isspace(lineBuffer[j + offset]) && lineBuffer[j + offset] != '#' && j - 1 - n1Length <= 40) {
                if(isalnum(lineBuffer[j + offset])){
                    currLine[j] = lineBuffer[j + offset];
                    j++;
                } else{
                    fprintf(stderr, "ERROR: Edge line:\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(j - 1 - n1Length > 40){
                fprintf(stderr, "ERROR: Second node in line '%s' contains more than 40 characters", lineBuffer);
                exit(EXIT_FAILURE);
            }
            currLine[j] = '\0';
        }
        for (int k = 0; k < i; ++k) {
            if(strcmp(currLine, array_1d_inspect_value(cleanMap, k)) == 0){
                fprintf(stderr, "ERROR: File had duplicate edges of '%s'\n", currLine);
                exit(EXIT_FAILURE);
            }
        }
        if(i <= array_1d_high(cleanMap)){
            array_1d_set_value(cleanMap, currLine, i);
            i++;
        } else{
            fprintf(stderr, "ERROR: File stated %d edges but file had %d\n", numEdges, i);
            exit(EXIT_FAILURE);
        }
    }

    if(!array_1d_has_value(cleanMap, array_1d_high(cleanMap))){
        fprintf(stderr, "ERROR: Number of edges stated did not actual number (%d)\n", numEdges);
    }
    return cleanMap;

}

array_1d *getLabels(const array_1d *cleanMap)
{
    int numEdges = atoi(array_1d_inspect_value(cleanMap, 0));
    array_1d *labels = array_1d_create(0, 2 * numEdges, free);
    int firstFreeIndex = 0;
    bool lbl1Exists;
    bool lbl2Exists;

    for (int i = 1; i < numEdges + 1; ++i) {
        char *lbl1 = calloc(41, sizeof(char));
        char *lbl2 = calloc(41, sizeof(char));
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
        char *lbl1 = calloc(41, sizeof(char));
        char *lbl2 = calloc(41, sizeof(char));
        char *currLine = array_1d_inspect_value(cleanMap, i);
        sscanf(currLine, "%s %s", lbl1, lbl2);

        node *n1 = graph_find_node(g, lbl1);
        node *n2 = graph_find_node(g, lbl2);

        if(!nodes_are_equal(n1, n2)){
            graph_insert_edge(g, n1, n2);
        }
        free(lbl1);
        free(lbl2);

    }
    return g;
}

bool find_path(graph *g, node *src, node *dest){
    if(nodes_are_equal(src, dest)){
        return true;
    }
    queue *q = queue_empty(NULL);
    dlist *neighbourSet;
    graph_node_set_seen(g, src, true);
    q = queue_enqueue(q, src);
    while(!queue_is_empty(q)){
        node *currNode = queue_front(q);
        q = queue_dequeue(q);
        neighbourSet = graph_neighbours(g, currNode);
        dlist_pos pos = dlist_first(neighbourSet);
        while(!dlist_is_end(neighbourSet, pos)){
            node *currNeighbour = dlist_inspect(neighbourSet, pos);
            if(nodes_are_equal(currNeighbour, dest)){
                dlist_kill(neighbourSet);
                queue_kill(q);
                return true;
            }
            if(!graph_node_is_seen(g, currNeighbour)){
                graph_node_set_seen(g, currNeighbour, true);
                q = queue_enqueue(q, currNeighbour);
            }
            pos = dlist_next(neighbourSet, pos);
        }
        dlist_kill(neighbourSet);
    }
    queue_kill(q);
    return false;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        exit(EXIT_FAILURE);
    }

    FILE *map = readFile(argv[1]);
    array_1d *cleanMap = cleanFile(map); //Allocates
    fclose(map);

    array_1d *labels = getLabels(cleanMap);
    int n = 0;
    while(array_1d_inspect_value(labels, n) != NULL){
        n++;
    }

    graph *g = graph_empty(n);
    g = addNodes(g, labels);
    g = addEdges(g, cleanMap);


    char input[83];
    char start[41];
    char goal[41];

    do{
        printf("Enter origin and destination (quit to exit): ");
        scanf("%[^\n]%*c", input);

        if(strcmp(input, "quit") != 0){
            int lastNW = lastNonWhiteSpace(input);
            int i = 0;
            while(!isspace(input[i]) && input[i] != '\0'){
                start[i] = input[i];
                i++;
            }
            if(lastNW != i-1){
                start[i] = '\0';
                i++;
                int j = 0;
                while(!isspace(input[i]) && input[i] != '\0'){
                    goal[j] = input[i];
                    i++;
                    j++;
                }
                goal[j] = '\0';
                if(lastNW == i-1){
                    node *src = graph_find_node(g, start);
                    node *dest = graph_find_node(g, goal);
                    if(src == NULL) {
                        printf("The node '%s' does not exist in the given map.\n\n", start);
                    } else if(dest == NULL){
                        printf("The node '%s' does not exist in the given map.\n\n", goal);
                    } else{
                        if(find_path(g, src, dest)){
                            printf("There is a path from %s to %s.\n\n", start, goal);
                        } else{
                            printf("There is no path from %s to %s.\n\n", start, goal);
                        }
                    }
                } else{
                    printf("Please enter exactly two nodes in the format 'n1 n2'.\n\n");
                }

            } else{
                printf("Please enter exactly two nodesin the format 'n1 n2'.\n\n");
            }
        }


    }while(strcmp(input, "quit") != 0);

    array_1d_kill(labels);
    graph_kill(g);
    array_1d_kill(cleanMap);

    printf("Normal exit.\n");
    return 0;
}

