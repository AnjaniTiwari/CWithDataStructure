#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
/*
*   Adjacency matrix representation undirected graph.
*
*			  1
*             |  \
*   	  2   |   3
*          \  |  /
*             4
*/


void print_graph(int** graph, unsigned short vertices) {
	unsigned short i, j;
	if(graph != NULL) {
		for(i = 0; i < vertices; ++i) {
			for(j = 0; j < vertices; ++j)
				printf("%d ", graph[i][j]);
			printf("\n");
		}
	}
}

int** make_grid(unsigned int rows, unsigned int size) {
	unsigned int i, j;
	int** graph = (int**)calloc(rows, sizeof(int*));
	if(graph == NULL) {
		printf("Dynamic rows allocation failed.\n");
		return graph;
	}

	for(i = 0; i < rows; ++i) {
		graph[i] = (int*)calloc(rows, sizeof(int));
		if(graph == NULL) {
			printf("Dynamic columns allocation failed.\n");
			for(j = 0; j < i; ++j) {
				free(graph[j]);
            	graph[j] = NULL;
			}
			return graph;
		}
	}
	return graph;
}

void connect_vertices(int** const graph, unsigned int vertices) {
	unsigned int edge_count = 0, origin, desti, total_edge;
	unsigned short run = TRUE, flag = TRUE;

	total_edge = (vertices*(vertices-1))/2;
	while(run && (edge_count < total_edge)) {
		printf("Enter the two vertices between 1 and %u that you want to connect.\n"
				,vertices);
		scanf("%u %u", &origin, &desti);
		if((origin > 0 && origin <= vertices) && (desti > 0 && desti <= vertices)) {
			graph[origin-1][desti-1] = 1;
			graph[desti-1][origin-1] = 1;
			++edge_count;
			flag = TRUE;
			printf("Vertices(%d, %d) and (%d, %d) both are connected successfully.\n"
				   ,origin, desti, desti, origin);
		}
		else {
			printf("Invalid vertices (%u, %u)\n", origin, desti);
			printf("Try again enter 1 for exit enter 0?\n");
			scanf("%hu", &run);
			flag = FALSE;
		}
		if(flag) {
			printf("Continue press 1 exit press 0: ");
			scanf("%hu", &run);
        }
	}
}

int** create_graph(void) {
	unsigned int vertices;
	int** graph = NULL;

	printf("Enter how many vertices to create.\n");
	scanf("%u", &vertices);
	if(vertices == 0 || vertices == 1)
		printf("please enter vertices input grater than 1.");
	else
		graph = make_grid(vertices, sizeof(int));
	if(graph != NULL)
		connect_vertices(graph, vertices);

	print_graph(graph, vertices);

	return graph;
}

int main() {
	int** graph = NULL;

	graph = create_graph();

	getch();
	return 0;
}