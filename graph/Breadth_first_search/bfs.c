/* Write a simple program to implement breadth first search using matrix. */

#include <stdio.h>
#define MAX 100
int graph[MAX][MAX];
int total_vertices;
int source, destination;
int queue_vertices[MAX];
int visited[MAX];
int front = -1, rear = -1;

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

void enqueue(int vertex) {
	if(rear < MAX) {
		if(front == -1)
			++front;
		queue_vertices[++rear] = vertex;
	}
	else
        printf("Queue is overflow.\n") ;
}

int is_empty(void) {
	if(front == -1 || front > rear)
	 return 0;
	else
	   return 1;
}

int dequeue(void) {
	if(is_empty()) {
        return queue_vertices[front++];
	}
	else
    	return 0;
}

void breadth_first_search(int start_vertex) {
	int i;
	if(start_vertex >= 1 && start_vertex <= total_vertices) {
    	visited[start_vertex] = 1;
		 do {
         	printf("%d ", start_vertex);
			for(i = 1; i <= total_vertices; ++i) {
				if(graph[start_vertex][i] == 1 && visited[i] != 1) {
					visited[i] = 1;
					enqueue(i);
				}
			}
		} while(start_vertex = dequeue());
	}
	else {
    	printf("%d vertex not in the graph.\n", start_vertex);
	}
}


int main() {
	int start_vertex;
	add_edges();
	print_graph();
	printf("Enter bfs starting vertex: ");
	scanf("%d", &start_vertex);
	breadth_first_search(start_vertex);
	getch();
	return 0;
}

