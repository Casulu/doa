#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "queue.h"

#define MAX_LINE_LENGTH 300
#define MAX_NODE_LENGTH 40
#define COMMENT_SIGN '#'

/**
 * Get first non-whitespace index in string.
 * @param s - String to get index from
 * @return Index of first non-whitespace character in string
 */
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
/**
 * Gets the last non-whitespace character in a string.
 * @param s - String to get index from
 * @return Index of last non-whitespace character in string or -1 if reading failed
 */
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

/**
 *  Checks if string only contains whitespace
 * @param s - String to check
 * @return True if string is blank. False otherwise.
 */
bool lineIsBlank(const char *s)
{
    // Line is blank if it only contained white-space chars.
    return firstNonWhiteSpace(s) < 0;
}

/**
 * Checks if string given is considered a comment
 * @param s - String to check
 * @return True if string is comment. False otherwise.
 */
bool lineIsComment(const char *s)
{
    int i = firstNonWhiteSpace(s);
    return (i >= 0 && s[i] == COMMENT_SIGN);
}

/**
 * Gets the index of the first comment sign.
 * @param s - String to get index from
 * @return Returns the index of the first comment sign if one was found or the index after the last nonwhitespace
 */
int firstCommentSign(const char *s){
    int i = 0;
    while(s[i] != COMMENT_SIGN && s[i]){
        i++;
    }

    int end = lastNonWhiteSpace(s) + 1;
    if(i > end){
        return end;
    } else{
        return i;
    }

}

/**
 * Gets the last non-whitespace character in a line while ignoring comments
 * @param s - String to get index from
 * @return Index of last non-whitespace character ignoring comments in string or -1 if reading failed
 */
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

/**
 *  Reads given path for opening. Exits via exit(EXIT_FAILURE) if reading failed
 * @param name - Path of file to read
 * @return An opened filestream to the given path.
 */
FILE *readFile(const char *name){
    FILE *in;
    /* open for reading */
    in = fopen(name, "r");
    //Print error if reading failed
    if (in == NULL) {
        fprintf(stderr, "ERROR: Failed to open %s for reading: %s\n",
                name, strerror(errno));
        exit(EXIT_FAILURE);
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
    bool reachedEdge;
    bool badLine;

    bool numIsRead = false;
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

            //Start reading the read line until the max node length is passed, until whitespace is reached, or until the end of the line
            while (lineBuffer[j + offset] && !isspace(lineBuffer[j + offset]) && h <= MAX_NODE_LENGTH && lineBuffer[j + offset] != COMMENT_SIGN) {
                if(isalnum(lineBuffer[j + offset])){
                    lblBuff[h] = lineBuffer[j + offset];
                    j++;
                    h++;
                } else{
                    //Print error and exit if a non-alphanumerical character is read
                    fprintf(stderr, "ERROR: Edge line\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(h > MAX_NODE_LENGTH){
                //Print error and exit if node is longer than specified max length
                fprintf(stderr, "ERROR: First node in line:\n%scontains more than %d characters", lineBuffer, MAX_NODE_LENGTH);
                exit(EXIT_FAILURE);
            }
            if(lineBuffer[j + offset] != ' '){
                //Print error and exit if first node is last node on line
                fprintf(stderr, "ERROR: Separator other than space in Line:\n%s", lineBuffer);
                exit(EXIT_FAILURE);
            }
            if(j + offset - 1 == lastNonWhiteSpaceNoComment(lineBuffer)){
                //Print error and exit if line buffer index reached last readable position after one node
                fprintf(stderr, "ERROR: Line:\n %s only contains one node", lineBuffer);
                exit(EXIT_FAILURE);
            }
            //End string and copy to first label
            lblBuff[h] = '\0';
            strcpy(lbl1, lblBuff);
            //Insert label as node if not already in graph
            if(graph_find_node(g, lbl1) == NULL){
                graph_insert_node(g, lbl1);
            }
            //Reset label buffer index and loop line buffer index until a non-space character is found
            h = 0;
            while (lineBuffer[j + offset] == ' ') {
                j++;
            }

            //Keep reading line buffer until the max node length is passed, until whitespace is reached, until a comment is reached, or until the end of the line
            while (lineBuffer[j + offset] && !isspace(lineBuffer[j + offset]) && lineBuffer[j + offset] != COMMENT_SIGN && h <= MAX_NODE_LENGTH) {
                if(isalnum(lineBuffer[j + offset])){
                    lblBuff[h] = lineBuffer[j + offset];
                    j++;
                    h++;
                } else{
                    //Print error and exit if a non-alphanumerical character is read
                    fprintf(stderr, "ERROR: Edge line:\n%scontained non alphanumerical character '%c'\n", lineBuffer, lineBuffer[j + offset]);
                    exit(EXIT_FAILURE);
                }
            }
            if(h > MAX_NODE_LENGTH){
                //Print error and exit if node is longer than specified max length
                fprintf(stderr, "ERROR: Second node in line '%s' contains more than %d characters", lineBuffer, MAX_NODE_LENGTH);
                exit(EXIT_FAILURE);
            }
            if(j + offset - 1 != lastNonWhiteSpaceNoComment(lineBuffer)){
                //Print error and exit if line buffer index reached last readable position after one node
                fprintf(stderr, "ERROR: Line:\n %s contains more than two node", lineBuffer);
                exit(EXIT_FAILURE);
            }
            //End string and copy to first label
            lblBuff[h] = '\0';
            strcpy(lbl2, lblBuff);
            //Free label buffer. We are done with it.
            free(lblBuff);

            //Insert label as node if not already in graph
            if(graph_find_node(g, lbl2) == NULL){
                graph_insert_node(g, lbl2);
            }

            //Create pointers for the nodes corresponding to the read labels
            node *n1 = graph_find_node(g, lbl1);
            node *n2 = graph_find_node(g, lbl2);

            //Variables for looping through first nodes neighbours
            dlist *lbl1Neighbours = graph_neighbours(g, n1);
            dlist_pos pos = dlist_first(lbl1Neighbours);
            bool duplicate = false;

            //Loop through  the first nodes neighbours and check if an edge between the two nodes already exists
            while(!dlist_is_end(lbl1Neighbours, pos) && !duplicate){
                if(nodes_are_equal(dlist_inspect(lbl1Neighbours, pos), n2)){
                    duplicate = true;
                }
                pos = dlist_next(lbl1Neighbours, pos);
            }
            //Done with list
            dlist_kill(lbl1Neighbours);


            if(!duplicate){
                graph_insert_edge(g, n1, n2);
                actualNumEdges++;
            } else{
                //Print error and exit if a duplicate edge was found
                fprintf(stderr, "ERROR: File had duplicate edges of '%s %s'\n", lbl1, lbl2);
                exit(EXIT_FAILURE);
            }
            //Done with labels
            free(lbl1);
            free(lbl2);

        }
        if(actualNumEdges > numEdges){
            fprintf(stderr, "ERROR: File stated %d but had at least %d\n", numEdges, actualNumEdges);
            exit(EXIT_FAILURE);
        }
    }

    if(actualNumEdges < numEdges){
        fprintf(stderr, "ERROR: File stated %d edges but file had %d\n", numEdges, actualNumEdges);
        exit(EXIT_FAILURE);
    }
    return g;

}

/**
 * Performs a breadth first search for a given node from another give node in a given graph
 * @param g - The graph to be searched in
 * @param src - The node to start traversing from
 * @param dest - The node to search for
 * @return - True if dest is found. False otherwise
 */
bool find_path(graph *g, node *src, node *dest){
    //Node is found if both nodes are the same
    if(nodes_are_equal(src, dest)){
        return true;
    }

    if(graph_has_edges(g)){
        //Create helper queue
        queue *q = queue_empty(NULL);
        //Set start node as seen and put it in queue
        graph_node_set_seen(g, src, true);
        q = queue_enqueue(q, src);
        dlist *neighbourSet;

        //Breadth first search until node is found or all nodes have been visited
        while(!queue_is_empty(q)){
            //Take first node in queue
            node *currNode = queue_front(q);
            q = queue_dequeue(q);
            //Loop through neighbours and check for match with destination node. Add to queue and set as seen if not already seen
            neighbourSet = graph_neighbours(g, currNode);
            dlist_pos pos = dlist_first(neighbourSet);
            while(!dlist_is_end(neighbourSet, pos)){
                node *currNeighbour = dlist_inspect(neighbourSet, pos);
                if(nodes_are_equal(currNeighbour, dest)){
                    //Kill everything early, reset nodes seen status, and return true if destination node is found
                    dlist_kill(neighbourSet);
                    queue_kill(q);
                    graph_reset_seen(g);
                    return true;
                }
                if(!graph_node_is_seen(g, currNeighbour)){
                    graph_node_set_seen(g, currNeighbour, true);
                    q = queue_enqueue(q, currNeighbour);
                }
                pos = dlist_next(neighbourSet, pos);
            }
            //Done with this neighbour set. Free
            dlist_kill(neighbourSet);
        }
        //Done with queue. Reset all nodes seen status and free queue
        queue_kill(q);
        graph_reset_seen(g);
        return false;
    } else{
        return false;
    }
}

/**
 * Reads the file given in argv as a graph and takes input from user to search for paths in the file. Prints the result
 * and asks again until 'quit' is inputted.
 */
int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Number of arguments exceeded 1");
        exit(EXIT_FAILURE);
    }

    FILE *map = readFile(argv[1]);
    graph *g = interpretMap(map);
    fclose(map);

    char input[MAX_NODE_LENGTH + 2];
    char srcLbl[MAX_NODE_LENGTH + 1];
    char destLbl[MAX_NODE_LENGTH + 1];

    //Loops until user inputs
    do{
        //Clear last buffer to avoid infinite loops
        strcpy(input, "");
        printf("Enter origin and destination (quit to exit): ");
        //Scan entire entered line and discard the newline
        scanf("%[^\n]", input);
        scanf("%*c");

        //Don't search if 'quit' was inputted
        if(strcmp(input, "quit") != 0){
            int lastNW = lastNonWhiteSpace(input);
            int i = 0;
            //Copy input to label until end of input or until space is encountered
            while(input[i] != ' ' && input[i]){
                srcLbl[i] = input[i];
                i++;
            }
            //End string
            srcLbl[i] = '\0';
            //If there is more to read, keep reading
            if(i-1 != lastNW && i-1 != strlen(input)){
                i++;
                int j = 0;
                while(!isspace(input[i]) && input[i]){
                    destLbl[j] = input[i];
                    i++;
                    j++;
                }
                destLbl[j] = '\0';
                //If we have reached the end of the input (or last whitespace) after second node
                if(i-1 == lastNW || i-1 == strlen(input)){
                    node *src = graph_find_node(g, srcLbl);
                    node *dest = graph_find_node(g, destLbl);
                    if(src == NULL) {
                        printf("The node '%s' does not exist in the given map.\n\n", srcLbl);
                    } else if(dest == NULL){
                        printf("The node '%s' does not exist in the given map.\n\n", destLbl);
                    } else{
                        if(find_path(g, src, dest)){
                            printf("There is a path from %s to %s.\n\n", srcLbl, destLbl);
                        } else{
                            printf("There is no path from %s to %s.\n\n", srcLbl, destLbl);
                        }
                    }
                } else{
                    //Prints if one or zero nodes were entered
                    printf("Please enter exactly two nodes in the format 'n1 n2' (separated with one space).\n\n");
                }

            } else{
                //Prints if more than two nodes were entered
                printf("Please enter exactly two nodes in the format 'n1 n2' (separated with one space).\n\n");
            }
        }


    }while(strcmp(input, "quit") != 0);

    //Done with graph
    graph_kill(g);
    printf("Normal exit.\n");

    return 0;
}

