#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "graph.h"
#include "queue.h"

/*
 * This program reads a given airmap file (given in program parameters) and creates a graph representation of it. The
 * program exits via exit(EXIT_FAILURE) if the input file is in the wrong format. After reading the file the user can
 * perform a breadth first search from a given node for another node. Use input is controlled and error messages are
 * written if input is wrongly formatted.
 *
 * Author: Rasmus Lyxell (c19rll@cs.umu.se)
 *
 * Version information:
 *   v1.0  2019-02-21: First public version.
 *   v2.0  2020-03-30: Split inteperetMap into three more functions.
 */

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
 * Reads from a line starting at an offset and ending at whitespace and copies the result to a character array.
 * @param line - line to read from
 * @param offset - offset to start from
 * @param output - char array to write output to
 * @return - length of word read
 */
int readToken(char *line, int offset, char *output){
    char *outBuff = malloc((MAX_NODE_LENGTH + 1) * sizeof(char));
    int j = 0;

    while (line[j + offset] && !isspace(line[j + offset]) && line[j + offset] != COMMENT_SIGN && j <= MAX_NODE_LENGTH) {
        if(isalnum(line[j + offset])){
            outBuff[j] = line[j + offset];
            j++;
        } else{
            //Print error and exit if a non-alphanumerical character is read
            fprintf(stderr, "ERROR: Edge line:\n%scontained non alphanumerical character '%c'\n", line, line[j + offset]);
            exit(EXIT_FAILURE);
        }
    }
    //End string and copy to first label
    outBuff[j] = '\0';
    strcpy(output, outBuff);
    free(outBuff);
    return j;
}


/**
 * Reads two labels from the given line and copies them to the given char arrays. Prints error and exits if there is
 * and error in formatting or number of nodes in line is not two.
 * @param line - line to read from.
 * @param lbl1 - a char array to place first label in
 * @param lbl2 - a char array to place second label in
 */
void readLabels(char line[MAX_LINE_LENGTH], char *lbl1, char *lbl2){
    int offset = firstNonWhiteSpace(line); //Offset for the index of the line being read.
    int length; //Index for the line being read

    length = readToken(line, offset, lbl1);
    if(length > MAX_NODE_LENGTH){
        //Print error and exit if node is longer than specified max length
        fprintf(stderr, "ERROR: First node in line:\n%scontains more than %d characters", line, MAX_NODE_LENGTH);
        exit(EXIT_FAILURE);
    }

    if(length + offset - 1 == lastNonWhiteSpaceNoComment(line)){
        //Print error and exit if line buffer index reached last readable position after one node
        fprintf(stderr, "ERROR: Line:\n %s only contains one node", line);
        exit(EXIT_FAILURE);
    }
    //Start reading the read line until the max node length is passed, until whitespace is reached, or until the end of the line
    if(line[length + offset] != ' '){
        //Print error and exit if first node is last node on line
        fprintf(stderr, "ERROR: Separator other than space in line:\n%s", line);
        exit(EXIT_FAILURE);
    }

    //Loop line buffer index until a non-space character is found
    while (line[length + offset] == ' ') {
        offset++;
    }
    offset = length + offset;

    length = readToken(line, offset, lbl2);
    if(length > MAX_NODE_LENGTH){
        //Print error and exit if node is longer than specified max length
        fprintf(stderr, "ERROR: Second node in line:\n%scontains more than %d characters", line, MAX_NODE_LENGTH);
        exit(EXIT_FAILURE);
    }
    //Keep reading line buffer until the max node length is passed, until whitespace is reached, until a comment is reached, or until the end of the line
    if(length + offset - 1 != lastNonWhiteSpaceNoComment(line)){
        //Print error and exit if line buffer index reached last readable position after one node
        fprintf(stderr, "ERROR: Line:\n %s contains more than two nodes", line);
        exit(EXIT_FAILURE);
    }
}

/**
 *  Reads the first valid line in a file and returns the number stated. Prints error and exits if item read is not
 *  a number.
 * @param map - file to read from
 * @return - The number stated on the first valid line
 */
int readNumEdges(FILE *map){
    char lineBuffer[MAX_LINE_LENGTH];
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
    return numEdges;
}

/**
 * Inserts and edge between two given nodes in a given graph if no duplicates are found.
 * @param g - graph to add edge to
 * @param src - source node
 * @param dst - destination ndoe
 * @return - Returns true if edge was added, false otherwise.
 */
bool insertNonDupeEdge(graph *g, node *src, node *dst){
    dlist *lbl1Neighbours = graph_neighbours(g, src);
    dlist_pos pos = dlist_first(lbl1Neighbours);
    bool duplicate = false;

    //Loop through  the first nodes neighbours and check if an edge between the two nodes already exists
    while(!dlist_is_end(lbl1Neighbours, pos) && !duplicate){
        if(nodes_are_equal(dlist_inspect(lbl1Neighbours, pos), dst)){
            duplicate = true;
        }
        pos = dlist_next(lbl1Neighbours, pos);
    }
    //Done with list
    dlist_kill(lbl1Neighbours);
    if(!duplicate){
        graph_insert_edge(g, src, dst);
    }
    return !duplicate;
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
    int numEdges = readNumEdges(map);

    //Create the output graph
    g = graph_empty(numEdges);

    //Loop through all lines in the input file
    int actualNumEdges = 0;
    while (fgets(lineBuffer, MAX_LINE_LENGTH, map)) {
        bool reachedEdge = false;
        //Loop until we reach a line that is meant to be read
        do {
            if(!lineIsBlank(lineBuffer) && !lineIsComment(lineBuffer)){
                reachedEdge = true;
            }
        } while(!reachedEdge && fgets(lineBuffer, MAX_LINE_LENGTH, map));

        
        if(reachedEdge){
            //Buffers for inserting nodes	
            char *lbl1 = calloc(MAX_NODE_LENGTH + 1, sizeof(char));
            char *lbl2 = calloc(MAX_NODE_LENGTH + 1, sizeof(char));

	        readLabels(lineBuffer, lbl1, lbl2);

            //Insert labels as nodes if not already in graph
            if(graph_find_node(g, lbl1) == NULL){
                graph_insert_node(g, lbl1);
            }
            if(graph_find_node(g, lbl2) == NULL){
                graph_insert_node(g, lbl2);
            }

            //Create pointers for the nodes corresponding to the read labels
            node *n1 = graph_find_node(g, lbl1);
            node *n2 = graph_find_node(g, lbl2);

            if(insertNonDupeEdge(g, n1, n2)){
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
    }
    if(actualNumEdges > numEdges){
        fprintf(stderr, "ERROR: File stated %d edges but had at least %d\n", numEdges, actualNumEdges);
        exit(EXIT_FAILURE);
    }
    if(actualNumEdges < numEdges){
        fprintf(stderr, "ERROR: File stated %d edges but had %d\n", numEdges, actualNumEdges);
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

