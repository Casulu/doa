#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "dlist.h"

/*
 * A graph stores nodes and edges of a directed graph.
 * After use, the function graph_kill() must
 * be called to de-allocate the dynamic memory used by the graph
 * itself. The de-allocation of any dynamic memory allocated for the
 * node names is the responsibility of the user of the graph.
 *
 * Author: Rasmus Lyxell (c19rll@cs.umu.se)
 *
 * Version information:
 *   v1.0  2019-02-21: First public version.
 *   v2.0  2020-03-30: Fixed memory error caused by the nodes array (wrong type allocated).
 */

// ====================== PUBLIC DATA TYPES ==========================

struct node {
	dlist *neighbours;
	char *label;
	bool seen;
};

struct graph{
	node **nodes;
	int numNodes;
	int numEdges;
};

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1,const node *n2)
{
	if(strcmp(n1->label, n2->label) == 0){
	    return true;
	}
	else{
            return false;
	}
}

// =================== GRAPH STRUCTURE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes)
{
	graph *g = calloc(1, sizeof(graph));
	g->numNodes = 0;
	g->nodes = calloc(max_nodes, sizeof(node));
	return g;
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g)
{
    return g->numEdges > 0;
}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s)
{
    node *n = malloc(sizeof(*n));
    n->seen = false;
    n->label = calloc(strlen(s) + 1, sizeof(char));
    strcpy(n->label, s);
    n->neighbours = dlist_empty(NULL);
    g->nodes[g->numNodes] = n;
    g->numNodes++;
    return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s)
{
    for (int i = 0; i < g->numNodes; ++i) {
        node *currNode = g->nodes[i];
        if(strcmp(s, currNode->label) == 0){
            return currNode;
        }
    }
    return NULL;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
    return n->seen;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{
    n->seen = seen;
    return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g)
{
    for (int i = 0; i < g->numNodes; ++i) {
        node *currNode = g->nodes[i];
        currNode->seen = false;
    }
    return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
    dlist_insert(n1->neighbours, n2, dlist_first(n1->neighbours));
    g->numEdges++;
    return g;
}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n)
{
    dlist *out = dlist_empty(NULL);
    dlist_pos pos = dlist_first(n->neighbours);
    while(!dlist_is_end(n->neighbours, pos)){
        dlist_insert(out, dlist_inspect(n->neighbours, pos), dlist_first(out));
        pos = dlist_next(n->neighbours, pos);
    }
    return out;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g)
{
    for (int i = g->numNodes-1; i >= 0; i--) {
        node *currNode = g->nodes[i];
        free(currNode->label);
        dlist_kill(currNode->neighbours);
        free(currNode);
    }
    free(g->nodes);
    free(g);
}
