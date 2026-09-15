#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"



EdgeList* build_net(int n) // Builds the graph by allocating memory and reading all edges
{
	EdgeList* Net = (EdgeList*)(malloc(n * sizeof(EdgeList)));
	checkAllocation(Net);

	initialize_head_and_tail(Net, n);

	int m; //Number of edges
	scanf("%d", &m);

	if (m < 1) //If number of edges invallid
	{
		return NULL;
		exit(1);
	}


	for (int i = 0; i < m; i++) // Read and insert each edge
	{
		int a, b, price;
		int read = scanf("%d %d %d", &a, &b, &price);

		if (read != 3) // Check if 3 values were read
		{
			printf("Invalid input.\n");
			exit(1);
		}

		if (a < 0 || a >= n || b < 0 || b >= n || price < 0)// Validate edge data ranges
		{
			printf("Invalid input.\n");
			exit(1);
		}

		insert(Net, a, b, price);
	}
	return Net;
}


void initialize_head_and_tail(EdgeList* Net, int n)// Initialize all head/tail pointers to NULL
{
	for (int j = 0; j < n; j++)
	{
		Net[j].head = NULL;
		Net[j].tail = NULL;
	}
}


void insert(EdgeList* Net, int a, int b, int price)// Inserts an undirected edge between vertex a and b with given cost
{
	EdgeNode* node_a = (EdgeNode*)(malloc(sizeof(EdgeNode)));
	checkAllocation(node_a);

	EdgeNode* node_b = (EdgeNode*)(malloc(sizeof(EdgeNode)));
	checkAllocation(node_b);

	FillData(node_a, b, price); // Fill data for a ? b	

	FillData(node_b, a, price); // Fill data for b ? a 

	insert_sorted(Net, node_a, a);
	insert_sorted(Net, node_b, b);
	return;
}


void FillData(EdgeNode* node, int c, int price)// Helper function: fill edge data in node
{
	node->e.neighbor = c;
	node->e.cost = price;
	node->next = NULL;
}


void insert_sorted(EdgeList* Net, EdgeNode* node, int src)// Inserts an edge node into the edge list in sorted order by neighbor
{
	EdgeList* lst = &Net[src];

	if (lst->head == NULL)    // Case: list is empty
	{
		lst->head = lst->tail = node;
		return;
	}

	if (lst->head->e.neighbor > node->e.neighbor) // Insert at beginning
	{
		node->next = lst->head;
		lst->head = node;
		return;
	}

	if (lst->tail->e.neighbor < node->e.neighbor) // Insert at end
	{
		lst->tail->next = node;
		lst->tail = node;
		return;
	}

	insert_in_middle(node, lst);// Insert in middle
}


void insert_in_middle(EdgeNode* node, EdgeList* lst)// Insert node in the middle of the list maintaining sorted order by neighbor
{
	EdgeNode* curr = lst->head;
	EdgeNode* prev = NULL;

	while (curr != NULL)
	{
		if (curr->e.neighbor > node->e.neighbor)
		{
			prev->next = node;
			node->next = curr;
			return;
		}

		prev = curr;
		curr = curr->next;
	}
}


void checkAllocation(void* ptr)// Checks if a pointer is NULL (allocation failed)
{
	if (ptr == NULL)
	{
		printf("Invalid input.");
		exit(-1);
	}
}


int* build_prim_tree(EdgeList net[], int n)//Build Minimum Spanning Tree  using Prim's algorithm on net of n nodes
{
	if (n == 1)
	{
		int* prim = (int*)malloc(sizeof(int));
		checkAllocation(prim);
		prim[0] = -1;
		return prim;
	}

	unsigned char* inT;
	int* min;
	CandidateList priority;
	CandidateNode** location;
	int* prim;

	initializeStructures(n, &inT, &min, &priority, &location, &prim);

	InItPriorityAndLocation(&priority, location, n);


	int source = 0;
	initializeSourceNode(source, min, prim, location);//This is the first node to join the tree prim


	while (priority.head != NULL) //Grow Tree
	{
		int u = DeleteMin(&priority); //Get the lowest element in priority
		location[u] = NULL;

		if (min[u] == INT_MAX)
		{
			break;
		}
		set_bit(inT, u); //Set the bit to 1
		updateNeighbors(net, inT, min, prim, &priority, location, u);
	}
	
	for (int i = 0; i < n; i++)
	{
		if (location[i] != NULL)
			free(location[i]); 
	}
	free(location);
	free(inT);
	free(min);

	return prim;// Returns array 'prim' representing Minimum Spanning Tree parent tree
}


void initializeStructures(int n, unsigned char** inT, int** min, CandidateList* priority, CandidateNode*** location, int** prim) // Initialize all data structures for Prim algorithm
{
	*inT = (unsigned char*)calloc((n + 7) / 8, sizeof(unsigned char));
	checkAllocation(inT);

	*min = (int*)malloc(n * sizeof(int));
	checkAllocation(*min);

	priority;
	priority->head = NULL;
	priority->tail = NULL;

	*location = (CandidateNode**)malloc(n * sizeof(CandidateNode*));
	checkAllocation(*location);

	*prim = (int*)malloc(n * sizeof(int));
	checkAllocation(*prim);

	for (int i = 0; i < n; i++)
	{
		(*min)[i] = INT_MAX; // Initialize distances to infinity
		(*prim)[i] = -1;   // Initialize parents to -1 (no parent)
	}

}

void initializeSourceNode(int source, int* min, int* prim, CandidateNode** location)// Set sorce for Prim MST
{
	min[source] = 0;
	prim[source] = -1;
	location[source]->c.min = 0;
}


void updateNeighbors(EdgeList net[], unsigned char* inT, int* min, int* prim, CandidateList* priority, CandidateNode** location, int u)// Update neighbors of u after adding u to MST
{
	for (EdgeNode* curr = net[u].head; curr != NULL; curr = curr->next)
	{
		int v = curr->e.neighbor;
		int cost = curr->e.cost;
		if (get_bit(inT, v) != 1 && cost < min[v])
		{
			min[v] = cost;
			prim[v] = u;
			if (location[v] != NULL)
			{
				location[v]->c.min = cost;
				DecreaseKey(priority, location, v);
			}

		}

	}
}


void InItPriorityAndLocation(CandidateList* priority, CandidateNode** location, int n)// Initialize priority queue and location array for Prim
{
	CandidateNode* prev = NULL;

	for (int i = 0; i < n; i++)
	{
		CandidateNode* node = (CandidateNode*)(malloc(sizeof(CandidateNode)));
		checkAllocation(node);

		node->c.computer = i;
		node->c.min = INT_MAX;
		node->next = NULL;
		node->prev = prev;

		location[i] = node;

		if (prev != NULL)
			prev->next = node;

		if (i == 0)
			priority->head = node;

		if (i == n - 1)
			priority->tail = node;
		prev = node;
	}
}




int DeleteMin(CandidateList* priority)// Delete and return the node with minimum 'min' value from priority queue
{
	CandidateNode* curr = priority->head;
	CandidateNode* min_node = curr;


	while (curr != NULL)
	{
		if (curr->c.min < min_node->c.min)
			min_node = curr;
		curr = curr->next;
	}

	if (min_node->prev == NULL)
		priority->head = min_node->next;

	else
		min_node->prev->next = min_node->next;

	if (min_node->next == NULL)
		priority->tail = min_node->prev;

	else
		min_node->next->prev = min_node->prev;

	int result = min_node->c.computer;

	free(min_node);

	return result;
}

void set_bit(unsigned char* bits, int i) // Set bit i in the bit vector to 1
{
	bits[i / 8] |= (1 << (i % 8));
}

void clear_bit(unsigned char* bits, int i)// Clear bit i in the bit vector (set to 0)
{
	bits[i / 8] &= ~(1 << (i % 8));
}

int get_bit(unsigned char* bits, int i)// Get the value of bit i (returns 0 or 1)
{
	return (bits[i / 8] >> (i % 8)) & 1;
}

void DecreaseKey(CandidateList* priority, CandidateNode** location, int u)// Decrease key operation to re-sort priority queue when a node's cost decreases
{
	CandidateNode* node = location[u];

	while (node->prev != NULL && node->c.min < node->prev->c.min)
	{
		CandidateNode* node_prev = node->prev;
		CandidateNode* node_next = node->next;

		node->prev = node_prev->prev;
		node->next = node_prev;
		node_prev->prev = node;
		node_prev->next = node_next;

		if (node_next != NULL)
			node_next->prev = node_prev;

		if (node->prev != NULL)
			node->prev->next = node;

		else
			priority->head = node;

		if (priority->tail == node)
			priority->tail = node_prev;
	}

}

EdgeList* build_paths(int* tree, int n)// Build adjacency lists of MST edges from parent array
{
	EdgeList* PrimPaths = (EdgeList*)(malloc(n * sizeof(EdgeList)));
	checkAllocation(PrimPaths);

	for (int i = 0; i < n; i++)
	{
		PrimPaths[i].head = NULL;
		PrimPaths[i].tail = NULL;
	}

	for (int v = 0; v < n; v++)
	{
		int u = tree[v];

		if (u != -1)
		{
			EdgeNode* node1 = (EdgeNode*)malloc(sizeof(EdgeNode));
			checkAllocation(node1);

			node1->e.neighbor = u;
			node1->e.cost = -1;
			node1->next = NULL;


			EdgeNode* node2 = (EdgeNode*)malloc(sizeof(EdgeNode));
			checkAllocation(node2);

			node2->e.neighbor = v;
			node2->e.cost = -1;
			node2->next = NULL;

			// Insert edge v?u
			if (PrimPaths[v].head == NULL)
				PrimPaths[v].head = PrimPaths[v].tail = node1;

			else
			{
				PrimPaths[v].tail->next = node1;
				PrimPaths[v].tail = node1;
			}


			if (PrimPaths[u].head == NULL)
				PrimPaths[u].head = PrimPaths[u].tail = node2;

			else
			{
				PrimPaths[u].tail->next = node2;
				PrimPaths[u].tail = node2;
			}
		}
	}
	return PrimPaths;
}


void find_and_print_path(EdgeList primpaths[], int n, int first, int last)// Find and print path between first and last in the MST adjacency lists primpaths
{
	if (first < 0 || first >= n || last < 0 || last >= n)
	{
		printf("Invalid input.\n");
		freeGraph(primpaths, n);
		exit(1);
	}

	int* color = (int*)malloc(n * sizeof(int));
	checkAllocation(color);
	white_comp(color, n);//Every computer to white

	int* parent = (int*)malloc(n * sizeof(int));
	checkAllocation(parent);

	for (int i = 0; i < n; i++)
		parent[i] = -1;

	find_path_rec(primpaths, n, first, last, color, parent);

	if (first == last)
		printf("%d\n", first);

	else if (parent[last] == -1)
		printf("No spanning tree available.");

	else
	{
		print_path(first, last, parent);
		printf("\n");
	}

	free(color);
	free(parent);
	freeGraph(primpaths, n);

}


void white_comp(int* color, int n)// Initialize all nodes as WHIT
{
	for (int i = 0; i < n; i++)
		color[i] = WHITE;

}


void find_path_rec(EdgeList primpaths[], int n, int first, int last, int color[], int parent[])// Recursive DFS to find path from first to last
{
	color[first] = GREY;// Colors nodes GREY when processing, BLACK when done

	for (EdgeNode* curr = primpaths[first].head; curr != NULL; curr = curr->next)
	{
		int v = curr->e.neighbor;
		if (color[v] == WHITE)
		{
			parent[v] = first;

			if (v == last)//We have reached the destination
				return;
			else
				find_path_rec(primpaths, n, v, last, color, parent);//Recursive call
		}
	}
	color[first] = BLACK;
}


void print_path(int first, int last, int parent[])
{
	if (last == first)
		printf("%d ", last);

	else
	{
		print_path(first, parent[last], parent);
		printf("%d ", last);

	}
}


void freeGraph(EdgeList* net, int n)
{
	if (net == NULL)
		return;

	for (int i = 0; i < n; i++)//Free every node
	{
		EdgeNode* curr = net[i].head;
		while (curr != NULL) {
			EdgeNode* temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
	free(net);
}