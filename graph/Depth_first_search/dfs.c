/* Write a simple program to implement depth first search using adjacency matrix
   using recursion and without recursion. */

#include <stdio.h>
#define MAX 100
int graph[MAX][MAX];
int total_vertices;
int source, destination;
int visited[MAX];
int top = -1;

typedef struct {
	int last_index;
	int vertex;
} stack_vertices;
stack_vertices stack[MAX];

void add_edges(void) {
	printf("Enter total number of vertices.");
	scanf("%d", &total_vertices);
	if(total_vertices < MAX) {
		while(1) {
			printf("Enter edge(0, 0) for exit: ");
			scanf("%d%d", &source, &destination);
			if((source >= 1 && source <= total_vertices) &&
			   (destination >= 1 && destination <= total_vertices)) {
					 graph[source][destination] = 1;   //undirected graph
					 graph[destination][source] = 1;
			}
			else
				break;
		}
	}
}

void print_graph(void) {
	int row, column;
	printf("\n");
	for(row = 0; row <= total_vertices; ++row) {
		for(column = 0; column <= total_vertices; ++column) {
			printf("%d ", graph[row][column]);
		}
		printf("\n");
	}
}
int is_empty(void) {
	return top == -1 ? 1 : 0;
}

void push(int vertex, int last_index) {
	if(top == MAX) {
		printf("Stack overflow.\n");
		return;
	}
	stack[++top].vertex = vertex;
	stack[top].last_index = last_index;
}

stack_vertices* pop(void) {
	if(is_empty())
		return NULL;
    return &stack[top--];
}

void depth_first_search(int start_vertex) {
	int i = 1;
	stack_vertices* temp;

	printf("%d ", start_vertex);
	visited[start_vertex] = 1;
	while(1) {
		while(i <= total_vertices) {
			if(graph[start_vertex][i] == 1 && visited[i] != 1) {
				printf("%d ", i);
				visited[i] = 1;
				push(start_vertex, i);
				start_vertex = i;
				i = 1;
			}
			else
				++i;
		}
		temp = pop();
		if(temp == NULL)
			break;
		start_vertex = temp->vertex;
		i = temp->last_index + 1;
	}
}

void depth_first_search_rec(int start_vertex) {
    int i;
	printf("%d ", start_vertex);
	visited[start_vertex] = 1;
	for(i = 1; i <= total_vertices; ++i) {
		if(graph[start_vertex][i] == 1 && visited[i] != 1) {
            depth_first_search_rec(i);
		}
	}
}

int main() {
	int start_vertex;
	add_edges();
	print_graph();
	printf("Enter dfs starting vertex: ");
	scanf("%d", &start_vertex);
	if(start_vertex > 0 || start_vertex <= total_vertices)
		depth_first_search_rec(start_vertex);
	else
    	printf("%d is invalid start vertex.\n", start_vertex);
	getch();
	return 0;
}

