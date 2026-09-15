#ifndef GRAPH_H
#define GRAPH_H

#include <limits.h>

#define WHITE 0// Node not visited
#define GREY 1 // Node is being visited (in progress)
#define BLACK 2// Node has been completely visited

typedef struct // Struct that represents an edge in a graph: a neighbor and its cost
{
	int neighbor;
	int cost;
} Edge;

typedef struct edge_node // A node in a linked list of edges
{
	Edge e;
	struct edge_node* next;
} EdgeNode;

typedef struct// A list of edges (used to represent adjacency lists in a graph)
{
	EdgeNode* head;
	EdgeNode* tail;
} EdgeList;

typedef struct// Represents a node's candidacy in a priority queue (used in Prim's algorithm)
{
	int computer;// The node identifier
	int min;// Minimum cost to add this node to the tree
} Candidate;

typedef struct candidate_node// List node for the priority queue
{
	Candidate c;
	struct candidate_node* next;
	struct candidate_node* prev;
} CandidateNode;

typedef struct// The priority queue structure 
{
	CandidateNode* head;
	CandidateNode* tail;
} CandidateList;


// Function declarations

EdgeList* build_net(int n);// Initializes the network as an array of edge lists

void initialize_head_and_tail(EdgeList* Net, int n);// Initializes head and tail for each adjacency list

void insert(EdgeList* Net, int a, int b, int price);// Inserts an edge between nodes a and b with a given price

void FillData(EdgeNode* node, int c, int price);// Fills an edge node with specific values

void insert_sorted(EdgeList* Net, EdgeNode* node, int src);// Inserts an edge node in sorted order into the adjacency list of src

void insert_in_middle(EdgeNode* node, EdgeList* lst);// Inserts an edge node into the middle of an edge list (used for sorted insertion)

void checkAllocation(void* ptr);// Verifies successful memory allocation

int* build_prim_tree(EdgeList net[], int n);// Builds the minimum spanning tree using Prim's algorithm

void initializeStructures(int n, unsigned char** inT, int** min, CandidateList* priority, CandidateNode*** location, int** prim);// Allocates and initializes all data structures used in Prim's algorithm

void initializeSourceNode(int source, int* min, int* prim, CandidateNode** location);// Initializes the source node's data for Prim's algorithm

void updateNeighbors(EdgeList net[], unsigned char* inT, int* min, int* prim, CandidateList* priority, CandidateNode** location, int u);// Updates neighbor information during Prim's algorithm

void InItPriorityAndLocation(CandidateList* priority, CandidateNode** location, int n);// Initializes the priority list and location array for Prim's algorithm

int DeleteMin(CandidateList* priority);// Removes and returns the node with the smallest cost from the priority list


// Bit manipulation functions (used for marking visited nodes, etc.)
void set_bit(unsigned char* bits, int i);
void clear_bit(unsigned char* bits, int i);
int get_bit(unsigned char* bits, int i);

void DecreaseKey(CandidateList* priority, CandidateNode** location, int u);// Updates the cost of a node in the priority queue

EdgeList* build_paths(int* tree, int n);// Constructs the paths tree from the MST (Minimum Spanning Tree)

void find_and_print_path(EdgeList primpaths[], int n, int first, int last);// Finds and prints the path between two nodes in the MST paths

void white_comp(int* color, int n);// Sets all nodes' colors to WHITE (unvisited)

void find_path_rec(EdgeList primpaths[], int n, int first, int last, int color[], int parent[]);// Recursively finds the path between two nodes in the MST

void print_path(int first, int last, int parent[]);// Prints the final path from source to destination


//Free functions
void freeGraph(EdgeList* net, int n);


#endif


