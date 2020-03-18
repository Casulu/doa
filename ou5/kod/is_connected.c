#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "array_1d.h"
#include "queue.h"

#define MAX_LINE_LENGTH 300
#define MAX_NODE_LENGTH 40

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

    int end = lastNonWhiteSpace(s) + 1;
    if(i > end){
        return end;
    } else{
        return i;
    }

}

int lastNonWhiteSpaceNoComment(const char *s)
{
// Start at last char.
    int i = firstCommentSign(s) - 1;
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
    //Print error if reading failed
    if (in == NULL) {
        fprintf(stderr, "ERROR: Failed to open %s for reading: %s\n",
                name, strerror(errno));
        exit(1);
    }
    return in;
}

/**
 * Reads the content of an airmap and interprets it as a graph. Also checks for bad formatting in the input and exits
 * via exit(EXIT_FAILURE) if any are detected.
 * @param map - A filestream to be read.
 * @return - A pointer to a new graph containing all nodes and edged from the airmap
 */
graph *interpretMap(FILE *map)
{
    graph *g;
    char lineBuffer[MAX_LINE_LENGTH];
    bool numIsRead = false;
    bool reachedEdge = false;
    bool badLine = false;
    int numEdges = 0;

    //This block looks for the first non-ignored line and grabs the number of lines.
    while(!numIsRead && fgets(lineBuffer, MAX_LINE_LENGTH, map)){
        if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
            numIsRead = true;

            badLine = false;
            for (int i = firstNonWhiteSpace(lineBuffer); i < firstCommentSign(lineBuffer); ++i) {
                if(!isdigit(lineBuffer[i])){
                   badLine = true;
                }
            }
            if(!badLine){
                numEdges = atoi(lineBuffer);
            } else{
                //Print error and exit if no number could be read from first line
                fprintf(stderr, "ERROR: First line contained %s which is not a number\n",
                        lineBuffer);
                exit(EXIT_FAILURE);
            }
        }
    }

    //Create the output graph
    g = graph_empty(numEdges);

    //Loop through all lines in the input file
    int actualNumEdges = 0;
    while (fgets(lineBuffer, MAX_LINE_LENGTH, map)) {
        reachedEdge = false;
        char *lblBuff = malloc((MAX_NODE_LENGTH + 1) * sizeof(char)); //Buffer for labels in file
        //Loop until we reach a line that is meant to be read
        do {
            if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
                reachedEdge = true;
            }
        } while(!reachedEdge && fgets(lineBuffer, MAX_LINE_LENGTH, map));

        if(reachedEdge) {
            //Buffers for inserting nodes
            char *lbl1 = calloc(MAX_NODE_LENGTH + 1, sizeof(char));
            char *lbl2 = calloc(MAX_NODE_LENGTH + 1, sizeof(char));

            int offset = firstNonWhiteSpace(lineBuffer); //Offset for the index of the line being read.
            int j = 0; //Index for the line being read
            int h = 0; //Index for the label buffer
            //Start reading the read line until the max length is passed, until whitespace is reached, or until the end of the line
            while (lineBuffer[j + offset] && lineBuffer[j + offset] != ' ' && h <= MAX_NODE_LENGTH) {
                if(isalnum(lineBuffer[j + offset])){
                    lblBuff[h] = lineBuffer[j + offset];
                    j++;
                    h++;
                } else{
                    //Print error and exit if a non-alphanumerical character is read outside of a comment
                    fprintf(stderr, "ERROR: Edge line\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(h > MAX_NODE_LENGTH){
                fprintf(stderr, "ERROR: First node in line:\n%scontains more than %d characters", lineBuffer, MAX_NODE_LENGTH);
                exit(EXIT_FAILURE);
            }
            lblBuff[h] = '\0';
            strcpy(lbl1, lblBuff);
            if(graph_find_node(g, lbl1) == NULL){
                graph_insert_node(g, lbl1);
            }
            h = 0;
            while (lineBuffer[j + offset] == ' ') {
                j++;
            }
            if(lastNonWhiteSpaceNoComment(lineBuffer) == j + offset){
                fprintf(stderr, "ERROR: Line:\n %s only contains one node", lineBuffer);
                exit(EXIT_FAILURE);
            }

            while (lineBuffer[j + offset] && lineBuffer[j + offset] != ' ' && lineBuffer[j + offset] != '#' && h <= MAX_NODE_LENGTH) {
                if(isalnum(lineBuffer[j + offset])){
                    lblBuff[h] = lineBuffer[j + offset];
                    j++;
                    h++;
                } else{
                    fprintf(stderr, "ERROR: Edge line:\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(h > MAX_NODE_LENGTH){
                fprintf(stderr, "ERROR: Second node in line '%s' contains more than %d characters", lineBuffer, MAX_NODE_LENGTH);
                exit(EXIT_FAILURE);
            }
            lblBuff[h] = '\0';
            strcpy(lbl2, lblBuff);
            free(lblBuff);

            if(graph_find_node(g, lbl2) == NULL){
                graph_insert_node(g, lbl2);
            }
            node *n1 = graph_find_node(g, lbl1);
            node *n2 = graph_find_node(g, lbl2);

            dlist *lbl1Neighbours = graph_neighbours(g, n1);
            dlist_pos pos = dlist_first(lbl1Neighbours);
            bool duplicate = false;
            while(!dlist_is_end(lbl1Neighbours, pos)){
                if(nodes_are_equal(dlist_inspect(lbl1Neighbours, pos), n2)){
                    duplicate = true;
                }
                pos = dlist_next(lbl1Neighbours, pos);
            }
            dlist_kill(lbl1Neighbours);

            if(!duplicate){
                graph_insert_edge(g, n1, n2);
                actualNumEdges++;
            } else{
                fprintf(stderr, "ERROR: File had duplicate edges of '%s %s'\n", lbl1, lbl2);
                exit(EXIT_FAILURE);
            }
            free(lbl1);
            free(lbl2);

        }
        if(actualNumEdges > numEdges){
            fprintf(stderr, "ERROR: File stated %d but had at least %d\n", numEdges, actualNumEdges);
            exit(EXIT_FAILURE);;
        }
    }

    if(actualNumEdges < numEdges){
        fprintf(stderr, "ERROR: File stated %d edges but file had %d\n", numEdges, actualNumEdges);
        exit(EXIT_FAILURE);;
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
        fprintf(stderr, "Number of arguments exceeded 1");
        exit(EXIT_FAILURE);
    }

    FILE *map = readFile(argv[1]);
    graph *g = interpretMap(map);
    fclose(map);

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

    graph_kill(g);

    printf("Normal exit.\n");
    return 0;
}

