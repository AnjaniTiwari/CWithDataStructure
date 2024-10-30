#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define INFINITE 99999
typedef struct vertex vertex;

typedef struct {
	vertex** vertex_arr;
	int* path_length_arr;
	vertex** predecssor_vertex_arr;
	unsigned short* status_arr;
}table;

struct vertex {
	int data;
	vertex** edge_arr;
	unsigned int edge_count;
	int* edge_length_arr;
	table table_var;
	unsigned int table_var_count;
	vertex* next_vertex;
};

vertex* get_vertex(vertex* list, unsigned short vertex_data) {
	do{
		if(list->data == vertex_data)
			break;
		list = list->next_vertex;
	}while(list);
	return list;
}

unsigned short is_edge_not_exist(int vertex_data, vertex* new_vertex) {
	unsigned int i;
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

unsigned short grow_edge_length_arr(vertex** new_vertex) {
	(*new_vertex)->edge_length_arr = (int*)realloc((*new_vertex)->edge_length_arr,
							  sizeof(int) * ((*new_vertex)->edge_count+1));
	if((*new_vertex)->edge_length_arr)
		return TRUE;
	else
		printf("grow_edge_arr not reallocated memory.\n");
	return FALSE;
}

void connect_edge(vertex* new_vertex, vertex** list) {
	vertex* pos_ptr = NULL;
	unsigned short desti, run = TRUE, remain_vertex;
	int vertex_data, length;
	do {
		remain_vertex = show_vertices(*list, new_vertex);
		if(remain_vertex) {
			printf("\nEnter vertex to make edge: ");
			scanf("%d", &vertex_data);
			if(vertex_data != new_vertex->data) {
				pos_ptr = get_vertex(*list, vertex_data);
				if(pos_ptr) {
					printf("Enter length from vertex %d to vertex %d\n",
							new_vertex->data, pos_ptr->data);
                    scanf("%d", &length);
					if(grow_edge_arr(&new_vertex)) {
						new_vertex->edge_arr[new_vertex->edge_count-1] = pos_ptr;
						if(grow_edge_length_arr(&new_vertex)) {
							new_vertex->edge_length_arr[new_vertex->edge_count-1] = length;
							printf("Point %d has been connected to point %d and length %d set successfully.\n",
									new_vertex->data, vertex_data, length);
						}
						else
                        	printf("grow_length_arr failed.\n");
					}
					else
						printf("grow_edge_arr failed.\n");
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
    short val;
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
		printf("If you want to connect edge press 1 or exit press 0.\n");
		scanf("%d", &val);
		if(val == TRUE)
			connect_edge(new_vertex, list);
		add_in_list(list, new_vertex);
	}
	else {
		*list = new_vertex;
    	printf("%d vertex is added successfully.\n", new_vertex->data);
	}
}

void show_graph(vertex* list) {
	unsigned int i;
	while(list) {
		printf("Vertex: %2d Edges: ", list->data);
		for(i = 0; i < list->edge_count; ++i) {
			printf("%d ", list->edge_arr[i]->data);
		}
        printf("Length: ");
		for(i = 0; i < list->edge_count; ++i) {
			printf("%d ", list->edge_length_arr[i]);
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
	unsigned int i, j;
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
		if((*list)->table_var.vertex_arr != NULL)
			free((*list)->table_var.vertex_arr);
		if((*list)->table_var.path_length_arr != NULL)
			free((*list)->table_var.path_length_arr);
		if((*list)->table_var.predecssor_vertex_arr != NULL)
			free((*list)->table_var.predecssor_vertex_arr);
		if((*list)->table_var.status_arr != NULL)
        	free((*list)->table_var.status_arr);
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

unsigned short grow_table(vertex** source, unsigned int size) {
    unsigned int i;
    (*source)->table_var.vertex_arr = (vertex**)realloc((*source)->table_var.vertex_arr,
							sizeof(vertex*) * ((*source)->table_var_count+size));
	if(!(*source)->table_var.vertex_arr) {
		printf("grow_table vertex_arr memory allocation failed.\n");
		return FALSE;
	}

	(*source)->table_var.path_length_arr = (int*)realloc((*source)->table_var.path_length_arr,
							sizeof(int) * ((*source)->table_var_count+size));
	if(!(*source)->table_var.path_length_arr) {
		printf("grow_table path_length_arr memory allocation failed.\n");
		return FALSE;
	}

	(*source)->table_var.predecssor_vertex_arr = (vertex**)realloc((*source)->table_var.predecssor_vertex_arr,
							sizeof(vertex*) * ((*source)->table_var_count+size));
	if(!(*source)->table_var.predecssor_vertex_arr) {
		printf("grow_table predecssor_vertex_arr memory allocation failed.\n");
		return FALSE;
	}

	(*source)->table_var.status_arr = (unsigned short*)realloc((*source)->table_var.status_arr,
							sizeof(unsigned short) * ((*source)->table_var_count+size));
	if(!(*source)->table_var.status_arr) {
		printf("grow_table status_arr memory allocation failed.\n");
		return FALSE;
	}

	i = (*source)->table_var_count;
	(*source)->table_var_count+=size;
	do {
    	(*source)->table_var.vertex_arr[i] = NULL;
		(*source)->table_var.path_length_arr[i] = 0;
		(*source)->table_var.predecssor_vertex_arr[i] = NULL;
		(*source)->table_var.status_arr[i] = 0;
		++i;
	}while(i != (*source)->table_var_count);

	return TRUE;
}

void find_shortest_path(vertex* list, int source_data, int dst_data) {
	unsigned int i;
	static int total_length;
	if(source_data == dst_data) {
		printf("%d -> ", source_data);
		return;
	}

	for(i = 0; i < list->table_var_count; ++i) {
		if(list->table_var.vertex_arr[i]->data == dst_data) {
			if(!total_length) {
				total_length = list->table_var.path_length_arr[i];
			}
			find_shortest_path(list, source_data, list->table_var.predecssor_vertex_arr[i]->data);
			if(total_length == list->table_var.path_length_arr[i]) {
            	printf("%d total lenght %d\n", dst_data, total_length);
			}
			else
				printf("%d -> ", dst_data);
			return;
		}
	}
	if(i == list->table_var_count)
		printf("%d vertex unreachable.\n", list->table_var.vertex_arr[i]->data);

}

unsigned short check_table(vertex** main_source, unsigned int index) {
	unsigned int i, j;
	int length;
    unsigned short value = FALSE;
	vertex* temp_ptr = (*main_source)->table_var.vertex_arr[index];
	for(i = 0; i < temp_ptr->edge_count; ++i) {
		for(j = 0; j < (*main_source)->table_var_count; ++j) {
			if(temp_ptr->edge_arr[i] == (*main_source)->table_var.vertex_arr[j]) {
				length = (*main_source)->table_var.path_length_arr[index]+temp_ptr->edge_length_arr[i];
				if(length < (*main_source)->table_var.path_length_arr[j]) {
					if(length < 0) {
						if((*main_source)->table_var.status_arr[j] == 1)
							return TRUE; //negative cycle
						(*main_source)->table_var.status_arr[j] = 1;
					}
					(*main_source)->table_var.path_length_arr[j] = length;
					(*main_source)->table_var.predecssor_vertex_arr[j] = temp_ptr;
					value = check_table(main_source, j);
				}
				break;
			}
		}
		if(j == (*main_source)->table_var_count) {
			grow_table(main_source, 1);
			length = (*main_source)->table_var.path_length_arr[index]+temp_ptr->edge_length_arr[i];
			(*main_source)->table_var.vertex_arr[j] = temp_ptr->edge_arr[i];
			(*main_source)->table_var.path_length_arr[j] = length;
			(*main_source)->table_var.predecssor_vertex_arr[j]  = temp_ptr;
			value = check_table(main_source, j);
		}
	}
	return value;
}

unsigned short bellman_ford_create_table(vertex** source){
	if((*source)->table_var_count == 0)
		grow_table(source, 1);
	(*source)->table_var.vertex_arr[0] = *source;
	(*source)->table_var.path_length_arr[0] = 0;
	return check_table(source, 0);
}

void bellman_ford(vertex** list) {
	int source, destination;
	unsigned int i;
	vertex* source_ptr = NULL;
	if(*list == NULL) {
		printf("List is empty.\n");
	}
	else {
		printf("Find shortest path.\n");
		printf("Enter source vertex vertex.\n");
		scanf("%d", &source);
		source_ptr = get_vertex(*list, source);
		if(source_ptr) {
			printf("Enter destination vertex vertex.\n");
			scanf("%d", &destination);
			if(source != destination) {
				if(get_vertex(*list, destination)) {
					if(bellman_ford_create_table(&source_ptr) == FALSE)
						find_shortest_path(source_ptr, source, destination);
					else {
						printf("In this graph nagative cycle detected.\n");
						for(i = 0; i < source_ptr->table_var_count; ++i) {
							source_ptr->table_var.path_length_arr[i] = INFINITE;
							source_ptr->table_var.status_arr[i] = 0;
                        }
                    }
				}
				else
					printf("%d vertex not present in the graph.\n", destination);
			}
			else
				printf("Invlid inpaut %d source and %d destination vertex are the same.",
						source, destination);
		}
		else
			printf("%d vertex not present in the graph.\n", source);
	}
}

vertex* bellman_ford_static_negative_cycle_graph(void) {
	vertex* v0, *v1, *v2, *v3;
	v0 = (vertex*)calloc(1, sizeof(vertex));
	v1 = (vertex*)calloc(1, sizeof(vertex));
	v1->data = 1;
	v2 = (vertex*)calloc(1, sizeof(vertex));
	v2->data = 2;
	v3 = (vertex*)calloc(1, sizeof(vertex));
	v3->data = 3;

	v0->edge_arr = (vertex**)calloc(1, sizeof(vertex*));
	v0->edge_arr[0] = v1;
	v0->edge_count = 1;
	v0->edge_length_arr = (int*)calloc(1, sizeof(int));
	v0->edge_length_arr[0] = 1;

	v1->edge_arr = (vertex**)calloc(1, sizeof(vertex*));
	v1->edge_arr[0] = v2;
	v1->edge_count = 1;
	v1->edge_length_arr = (int*)calloc(1, sizeof(int));
	v1->edge_length_arr[0] = -1;

	v2->edge_arr = (vertex**)calloc(1, sizeof(vertex*));
	v2->edge_arr[0] = v3;
	v2->edge_count = 1;
	v2->edge_length_arr = (int*)calloc(1, sizeof(int));
	v2->edge_length_arr[0] = -1;

	v3->edge_arr = (vertex**)calloc(1, sizeof(vertex*));
	v3->edge_arr[0] = v1;
	v3->edge_count = 1;
	v3->edge_length_arr = (int*)calloc(1, sizeof(int));
	v3->edge_length_arr[0] = -1;

	v2->next_vertex = v3;
	v1->next_vertex = v2;
	v0->next_vertex = v1;
	return v0;
}

vertex* bellman_ford_static_graph() {
	vertex* v0, *v1, *v2, *v3;
	v0 = (vertex*)calloc(1, sizeof(vertex));
	v1 = (vertex*)calloc(1, sizeof(vertex));
	v1->data = 1;
	v2 = (vertex*)calloc(1, sizeof(vertex));
	v2->data = 2;
	v3 = (vertex*)calloc(1, sizeof(vertex));
	v3->data = 3;

	v0->edge_arr = (vertex**)calloc(2, sizeof(vertex*));
	v0->edge_arr[0] = v1;
	v0->edge_arr[1] = v2;
	v0->edge_count = 2;
	v0->edge_length_arr = (int*)calloc(2, sizeof(int));
	v0->edge_length_arr[0] = 5;
	v0->edge_length_arr[1] = 2;

	v1->edge_arr = (vertex**)calloc(2, sizeof(vertex*));
	v1->edge_arr[0] = v0;
	v1->edge_arr[1] = v3;
	v1->edge_count = 2;
	v1->edge_length_arr = (int*)calloc(2, sizeof(int));
	v1->edge_length_arr[0] = 3;
	v1->edge_length_arr[1] = 4;

	v2->edge_arr = (vertex**)calloc(1, sizeof(vertex*));
	v2->edge_arr[0] = v3;
	v2->edge_count = 1;
	v2->edge_length_arr = (int*)calloc(1, sizeof(int));
	v2->edge_length_arr[0] = 6;

	v2->next_vertex = v3;
	v1->next_vertex = v2;
	v0->next_vertex = v1;
	return v0;
}

void show_menu(void) {
	printf("\n1.Add vertex.\n");
	printf("2.Add edge.\n");
	printf("3.Delete vertex.\n");
	printf("4.Delete edge.\n");
	printf("5.Show graph.\n");
	printf("6.Bellman ford algorithm.\n");
	printf("7.Exit.\n");
}

int main() {
	short choice, run = TRUE;
	vertex* list = NULL;
	list = bellman_ford_static_negative_cycle_graph();
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
			case 6: bellman_ford(&list);
					break;
			case 7: run = FALSE;
					break;
			default:
					printf("%d invaild choice.\n", choice);
		}
	}

	if(list != NULL)
    	free_list(&list);
	return 0;
}

