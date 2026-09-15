#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "graph.h"


int main()
{
	int n;

	EdgeList* Net;

	int* Prim;

	EdgeList* PrimPath;

	int first, last;

	scanf("%d", &n);     // Read number of vertices

	Net = build_net(n);
	Prim = build_prim_tree(Net, n);
	PrimPath = build_paths(Prim, n);
	scanf("%d%d", &first, &last);
	find_and_print_path(PrimPath, n, first, last);
	
	free(Prim);              
	freeGraph(Net, n);        

	return 0;
}

