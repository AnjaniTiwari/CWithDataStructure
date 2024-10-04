#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

typedef struct vertex vertex;

struct vertex {
	int data;
	vertex** edge_arr;
	unsigned short edge_count;
	vertex* next_vertex;
};

typedef struct {
	int* vertices;
	unsigned int size;
}visited;

typedef struct {
	vertex* arr;
	unsigned int size;
}queue;

visited visit_var;
unsigned int index;

queue vertex_queue;

vertex* get_vertex(vertex* list, unsigned short vertex_data) {
	do{
		if(list->data == vertex_data)
			break;
		list = list->next_vertex;
	}while(list);
	return list;
}

unsigned short is_edge_not_exist(int vertex_data, vertex* new_vertex) {
	int i;
	if(new_vertex->edge_arr != NULL)
		for(i = 0; i < new_vertex->edge_count; ++i)
			if(vertex_data == new_vertex->edge_arr[i]->data)
				return 0;
	return 1;
}

unsigned short show_vertices(vertex* list, vertex* new_vertex) {
	unsigned short remain_vertex = 0;
	do{
		if(is_edge_not_exist(list->data, new_vertex)) {
				if(new_vertex->data != list->data) {
					if(!remain_vertex)
						printf("\nThese are all available vertices to which the edge can conncet.\n");
					printf("%d ", list->data);
					++remain_vertex;
                }
		}
		list = list->next_vertex;
	}while(list);
	if(!remain_vertex)
    	printf("There is no vertex that can be connected to vertex %d\n", new_vertex->data);
	return remain_vertex;
}

void total_vertices(vertex* list, unsigned short* total_vertices) {
	do{
		++(*total_vertices);
		list = list->next_vertex;
	}while(list);
}

unsigned short grow_edge_arr(vertex** new_vertex) {
	(*new_vertex)->edge_arr = (vertex**)realloc((*new_vertex)->edge_arr,
							  sizeof(vertex*) * ((*new_vertex)->edge_count+1));
	if((*new_vertex)->edge_arr) {
		(*new_vertex)->edge_arr[(*new_vertex)->edge_count++] = NULL;
    	return TRUE;
	}
	else
		printf("grow_edge_arr not reallocated memory.\n");
	return FALSE;
}

void connect_edge(vertex* new_vertex, vertex** list) {
	vertex* pos_ptr = NULL;
	unsigned short desti, run = TRUE, remain_vertex;
	int vertex_data;
	do {
		remain_vertex = show_vertices(*list, new_vertex);
		if(remain_vertex) {
			printf("\nEnter vertex to make edge: ");
			scanf("%d", &vertex_data);
			if(vertex_data != new_vertex->data) {
				pos_ptr = get_vertex(*list, vertex_data);
				if(pos_ptr) {
					if(grow_edge_arr(&new_vertex)) {
						new_vertex->edge_arr[new_vertex->edge_count-1] = pos_ptr;
						printf("Point %d has been connected to point %d successfully.\n",
								new_vertex->data, vertex_data);
					}
				}
				else
					printf("%d vertex does not exist in the graph.\n", vertex_data);
			}
			else
            	printf("Invalid input.\n");
			printf("If you want to continue then press 1, to exit press 0.\n");
			scanf("%hu", &run);
		}
	} while(remain_vertex && run);
}

unsigned short not_in_list(vertex* list, int vertex_data) {
	while(list) {
		if(list->data == vertex_data)
			return 0;
		list = list->next_vertex;
	}
	return 1;
}

void add_in_list(vertex** list, vertex* new_vertex) {
	vertex* temp = *list;
	while(temp->next_vertex) {
		temp = temp->next_vertex;
	}
	temp->next_vertex = new_vertex;
}

void add_vertex(vertex** list) {
	unsigned short dst_vertex;
	vertex* new_vertex = NULL;
	int vertex_data;
	while(TRUE) {
		printf("Enter vertex data: ");
		scanf("%d", &vertex_data);
		if(*list == NULL)
        	break;
		else if(not_in_list(*list, vertex_data))
			break;
		else
			printf("\n%d invalid input try again.\n", vertex_data);
	}

	new_vertex = (vertex*)calloc(1, sizeof(vertex));
	if(!new_vertex) {
		printf("vertex allocation memory failed.\n");
		return;
	}
	new_vertex->data = vertex_data;
	if(*list != NULL) {
		connect_edge(new_vertex, list);
		add_in_list(list, new_vertex);
	}
	else
		*list = new_vertex;
}

void show_graph(vertex* list) {
	unsigned short i;
	while(list) {
		printf("Vertex: %2d Edges: ", list->data);
		for(i = 0; i < list->edge_count; ++i) {
			printf("%d ", list->edge_arr[i]->data);
		}
		printf("\n");
        list = list->next_vertex;
	}
}

void add_edge(vertex** list) {
	int vertex_data;
	unsigned short run = TRUE;
	vertex* select_vertex = NULL;
	if(*list ==   NULL) {
		printf("Graph is empty.\n");
	}
	else {
		while(run) {
			show_graph(*list);
			printf("Enter a vertex from the graph that needs to be connected to other vertices.\n");
			scanf("%d", &vertex_data);
			select_vertex = get_vertex(*list, vertex_data);
			if(select_vertex != NULL) {
				connect_edge(select_vertex, list);
				printf("Enter 1 to continue or 0 to exit.");
            }
			else {
				printf("%d invalid vertex not find in graph.\n");
				printf("try again press 1 or press exit 0.\n");
			}
			scanf("%d", &run);
		}
	}
}

void free_vertex(vertex* vertex_ptr) {
	if(vertex_ptr->edge_arr != NULL)
		free(vertex_ptr->edge_arr);
	if(vertex_ptr != NULL)
		free(vertex_ptr);
}

int remove_edge(vertex** ptr, int edge_data) {
	int i, j;
	for(i = 0; i < (*ptr)->edge_count; ++i) {
		if((*ptr)->edge_arr[i]->data == edge_data) {
			for(j = i; j < (*ptr)->edge_count; ++j)
				(*ptr)->edge_arr[j] = (*ptr)->edge_arr[j+1];
			--(*ptr)->edge_count;
			(*ptr)->edge_arr = (vertex**)realloc((*ptr)->edge_arr, sizeof(vertex*)*(*ptr)->edge_count);
			return 1;
        }
	}
	return 0;
}

void get_off_list(vertex** list, vertex* select_vertex) {
	vertex* pre_vertex = NULL;
	vertex* cur_vertex = *list;
	do{
		if(cur_vertex->next_vertex != NULL &&
		   cur_vertex->next_vertex->data == select_vertex->data) {
			pre_vertex = cur_vertex;
		}

		if(cur_vertex->data != select_vertex->data)
			remove_edge(&cur_vertex, select_vertex->data);

		cur_vertex = cur_vertex->next_vertex;
	}while(cur_vertex != NULL);
	if(pre_vertex != NULL)
		pre_vertex->next_vertex = select_vertex->next_vertex;
	else
		*list = select_vertex->next_vertex;
	printf("%d vertex is successfully deleted.\n", select_vertex->data);
	free_vertex(select_vertex);
}

void delete_vertex(vertex** list) {
	int vertex_data;
	unsigned short run = TRUE;
	vertex* select_vertex = NULL;
	if(*list ==   NULL) {
		printf("Graph is empty.\n");
	}
	else {
		while(run) {
			show_graph(*list);
			printf("Enter the vertex you want to delete.\n");
			scanf("%d", &vertex_data);
			select_vertex = get_vertex(*list, vertex_data);
			if(select_vertex != NULL) {
				get_off_list(list, select_vertex);
				printf("Enter 1 to continue or 0 to exit.");
            }
			else {
				printf("%d invalid vertex not find in graph.\n");
				printf("try again press 1 or press exit 0.\n");
			}
			scanf("%d", &run);
		}
	}
}

void free_list(vertex** list) {
    vertex* next = NULL;
	do {
		next = (*list)->next_vertex;
		if((*list)->edge_arr != NULL)
			free((*list)->edge_arr);
		free(*list);
		*list = next;
	}while(*list != NULL);

}

void delete_edge(vertex** list)  {
	int vertex_data, edge;
	unsigned short run = TRUE;
	vertex* select_vertex = NULL;
	if(*list ==   NULL) {
		printf("Graph is empty.\n");
	}
	else {
		while(run) {
			show_graph(*list);
			printf("Enter the vertex you want to delete the edge.\n");
			scanf("%d", &vertex_data);
			select_vertex = get_vertex(*list, vertex_data);
			if(select_vertex != NULL) {
				if(select_vertex->edge_arr != NULL) {
					printf("Enter which edge you want to delete.\n");
					scanf("%d", &edge);
					if(remove_edge(&select_vertex, edge) == TRUE)
						printf("%d edge is successfully deleted.\n", edge);
					else
					   printf("%d is not a edge.\n", edge);
				}
				else
					printf("%d vertex not have any edge.\n", select_vertex->data);
				printf("Enter 1 to continue or 0 to exit.");
			}
			else {
				printf("%d invalid vertex not find in graph.\n", vertex_data);
				printf("try again press 1 or press exit 0.\n");
			}
			scanf("%d", &run);
		}
	}
}

short visit(int data) {
	unsigned int size = visit_var.size;
	unsigned int i;

	if(visit_var.vertices == NULL || index == visit_var.size) {
		size = size == 0?10:size*2;
		visit_var.vertices = (int*)realloc(visit_var.vertices, sizeof(int)*size);
		if(!visit_var.vertices) {
			printf("size %d memory allocation falied.\n", visit_var.size);
			return 0;
		}
		visit_var.size = size;
	}

	for(i = 0; i < index; ++i) {
		if(visit_var.vertices[i] == data)
			return FALSE;
	}
	printf("%d ", data);
	visit_var.vertices[index++] = data;
	return TRUE;
}

void depth_first_search(vertex* start) {
	unsigned short i;
	for(i = 0; i < start->edge_count; ++i) {
		if(visit(start->edge_arr[i]->data))
			depth_first_search(start->edge_arr[i]);
	}
}

void dfs_vertex_input(vertex* list)  {
	int vertex_data;
	unsigned short run = TRUE;
	vertex* start_vertex = NULL;

	if(list ==   NULL) {
		printf("Graph is empty.\n");
	}
	else {
		while(run) {
			show_graph(list);
			printf("Enter the vertex from where you want to start dfs.\n");
			scanf("%d", &vertex_data);
			start_vertex = get_vertex(list, vertex_data);
			if(start_vertex != NULL) {
			   printf("Depth first search output.\n");
			   visit(start_vertex->data);
			   depth_first_search(start_vertex);
			   printf("\nEnter 1 to continue or 0 to exit.\n");
			}
			else {
				printf("%d invalid vertex not find in graph.\n", vertex_data);
				printf("try again press 1 or press exit 0.\n");
			}
			scanf("%d", &run);
			if(visit_var.vertices != NULL) {
				free(visit_var.vertices);
				visit_var.vertices = NULL;
			}
			visit_var.size = 0;
			index = 0;
		}
	}
}


void breadth_first_search(vertex* start_vertex) {
	if(vertex_queue.arr == NULL || )
}

void bfs_vertex_input(vertex* list)  {
	int vertex_data;
	unsigned short run = TRUE;
	vertex* start_vertex = NULL;

	if(list ==   NULL) {
		printf("Graph is empty.\n");
	}
	else {
		while(run) {
			show_graph(list);
			printf("Enter the vertex from where you want to start dfs.\n");
			scanf("%d", &vertex_data);
			start_vertex = get_vertex(list, vertex_data);
			if(start_vertex != NULL) {
			   printf("Breadth first search output.\n");
			   breath_first_search(start_vertex);
			   printf("\nEnter 1 to continue or 0 to exit.\n");
			}
			else {
				printf("%d invalid vertex not find in graph.\n", vertex_data);
				printf("try again press 1 or press exit 0.\n");
			}
			scanf("%d", &run);
			if(visit_var.vertices != NULL) {
				free(visit_var.vertices);
				visit_var.vertices = NULL;
			}
			visit_var.size = 0;
			index = 0;
		}
	}
}

void show_menu(void) {
	printf("\n1.Add vertex.\n");
	printf("2.Add edge.\n");
	printf("3.Delete vertex.\n");
	printf("4.Delete edge.\n");
	printf("5.Show graph.\n");
	printf("6.Depth first search.\n");
	printf("7.Depth first search.\n");
	printf("8.Exit.\n");
}

int main() {
	short choice, run = TRUE;
	vertex* list = NULL;

	while(run) {
		show_menu();
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch(choice) {
			case 1: add_vertex(&list);
					break;
			case 2:	add_edge(&list);
					break;
			case 3: delete_vertex(&list);
					break;
			case 4: delete_edge(&list);
                	break;
			case 5: show_graph(list);
					break;
			case 6: dfs_vertex_input(list);
					break;
			case 7: bfs_vertex_input(list);
					break;
			case 8: run = FALSE;
					break;
			default:
					printf("%d invaild choice.\n", choice);
		}
	}

	if(list != NULL)
    	free_list(&list);
	return 0;
}