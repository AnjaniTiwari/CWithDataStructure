#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
/*
*   Adjacency matrix representation undirected graph.
*
*			  0
*             |  \
*   	  1   |   2
*          \  |  /
*             3
*/


void print_graph(int** graph, unsigned short row, unsigned short column) {
	unsigned short i, j;
	if(graph != NULL) {
		for(i = 0; i < row; ++i) {
			for(j = 0; j < column; ++j)
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

void connect_vertices(int** const graph, unsigned int rows) {
	unsigned int edge_count = 0, origin, desti, total_edge, total_vertices;
	unsigned short run = TRUE, flag = TRUE;
	total_vertices = rows*rows;
	total_edge = (total_vertices*(total_vertices-1))/2;
	while(run && (edge_count < total_edge)) {
		printf("Enter the two vertices between 1 and %u that you want to connect.\n"
				,rows);
		scanf("%u%u", &origin, &desti);
		if((origin > 0 && origin <= rows) && (desti > 0 && desti <= rows)) {
			graph[origin-1][desti-1] = 1;
			graph[desti-1][origin-1] = 1;
			flag = TRUE;
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
	unsigned int rows;
	int** graph = NULL;

	printf("Enter your graph grid total number of rows.\n");
	scanf("%u", &rows);
	if(rows == 0 || rows == 1)
		printf("Rows: %u invalid input.", rows);
	else
		graph = make_grid(rows, sizeof(int));
	if(graph != NULL)
		connect_vertices(graph, rows);
	return graph;
}

int main() {
	int** graph = NULL;

	graph = create_graph();

	print_graph(graph, 4, 4);

	getch();
	return 0;
}