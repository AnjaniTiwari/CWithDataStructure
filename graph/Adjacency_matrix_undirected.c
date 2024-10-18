#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

typedef struct vertex vertex;

typedef struct {
	vertex** vertex_arr;
	unsigned int* path_length_arr;
	vertex** predecssor_vertex_arr;
	unsigned short* status_arr;
}table;

struct vertex {
	int data;
	vertex** edge_arr;
	unsigned int edge_count;
	unsigned int* edge_length_arr;
	table table_var;
	unsigned int table_var_count;
	vertex* next_vertex;
};



typedef struct {
	int* vertices;
	unsigned int size;
}visited;

typedef struct {
	vertex** arr;
	int size;
	int rear;
	int front;
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
	(*new_vertex)->edge_length_arr = (unsigned int*)realloc((*new_vertex)->edge_length_arr,
							  sizeof(unsigned int) * ((*new_vertex)->edge_count+1));
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
	unsigned int i;
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
	unsigned int size, i;
	if(vertex_queue.arr == NULL || vertex_queue.rear == vertex_queue.size-1) {
		size = vertex_queue.size == 0 ? 10 : vertex_queue.size*2;
		vertex_queue.arr = (vertex**)realloc(vertex_queue.arr,
											sizeof(vertex*) * size);
		if(!vertex_queue.arr) {
			printf("vertex queue size %d memory allocation failed.\n", size);
			return;
		}
		vertex_queue.size = size;
	}
	for(i = 0; i < start_vertex->edge_count; ++i) {
		if(visit(start_vertex->edge_arr[i]->data)) {
			vertex_queue.arr[++vertex_queue.rear] = start_vertex->edge_arr[i];
		}
	}
	while(vertex_queue.front != vertex_queue.rear) {
		breadth_first_search(vertex_queue.arr[++vertex_queue.front]);
	}
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
			   visit(start_vertex->data);
			   vertex_queue.rear = vertex_queue.front = -1;
			   breadth_first_search(start_vertex);
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

			if(vertex_queue.arr != NULL) {
				free(vertex_queue.arr);
				vertex_queue.arr = NULL;
			}
			vertex_queue.size = 0;
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

	(*source)->table_var.path_length_arr = (unsigned int*)realloc((*source)->table_var.path_length_arr,
							sizeof(unsigned int) * ((*source)->table_var_count+size));
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

unsigned int get_path_length(vertex* predecssor) {
	if(*(predecssor->table_var.predecssor_vertex_arr) != NULL) {
	   return *(predecssor->table_var.path_length_arr)	+ get_path_length(*(predecssor->table_var.predecssor_vertex_arr));
	}
	return *(predecssor->table_var.path_length_arr);
}

void add_in_table(vertex** main_source, vertex* current_ptr) {
	unsigned int i, j, index, length = -1, path_length;
	for(i = 0; i < current_ptr->edge_count; ++i) {
		for(j = 0; j < (*main_source)->table_var_count; ++j) {
			if(current_ptr->edge_arr[i] == (*main_source)->table_var.vertex_arr[j]) {
				if((*main_source)->table_var.status_arr[j] != 1) {
					path_length = get_path_length((*main_source)->table_var.predecssor_vertex_arr[index]);
					if(path_length < (*main_source)->table_var.path_length_arr[index]) {
						(*main_source)->table_var.path_length_arr[index] = path_length;
						(*main_source)->table_var.predecssor_vertex_arr[index] = current_ptr;
					}
					if((*main_source)->table_var.path_length_arr[index] < length) {
						length = (*main_source)->table_var.path_length_arr[index];
						index = j;
					}
				}
				break;
			}
		}
		if(j == (*main_source)->table_var_count) {
			grow_table(main_source, 1);
			(*main_source)->table_var.vertex_arr[(*main_source)->table_var_count-1] =
			current_ptr->edge_arr[i];
			(*main_source)->table_var.path_length_arr[(*main_source)->table_var_count-1] =
			current_ptr->edge_length_arr[i];
			(*main_source)->table_var.predecssor_vertex_arr[(*main_source)->table_var_count-1] =
			current_ptr;
		}
	}
	(*main_source)->table_var.status_arr[index] =  1;
	add_in_table(main_source, (*main_source)->table_var.vertex_arr[index]);
}

void create_table(vertex** source) {
	unsigned int i, data = -1, j;
	grow_table(source, (*source)->edge_count+1);
	(*source)->table_var.vertex_arr[0] = *source;
	(*source)->table_var.status_arr[0] = 1;
	for(i = 1; i < (*source)->edge_count; ++i) {
        (*source)->table_var.vertex_arr[i] = (*source)->edge_arr[i-1];
		(*source)->table_var.path_length_arr[i] = (*source)->edge_length_arr[i-1];
		(*source)->table_var.predecssor_vertex_arr[i] = *source;
		if((*source)->edge_length_arr[i-1] < data) {
			data = (*source)->edge_length_arr[i-1];
			j = i-1;
		}
	}
	(*source)->table_var.status_arr[j] = 1;
	add_in_table(source, (*source)->edge_arr[j]);
}

void dijkstras(vertex** list) {
	int source, destination;
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
					create_table(&source_ptr);
				}
				else
					printf("%d vertex not present in the graph.\n", destination);
			}
			else
				printf("Invalid input %d source and %d destination vertex are the same.",
						source, destination);
		}
		else
			printf("%d vertex not present in the graph.\n", source);
	}
}

void show_menu(void) {
	printf("1.Add vertex.\n");
	printf("2.Add edge.\n");
	printf("3.Delete vertex.\n");
	printf("4.Delete edge.\n");
	printf("5.Show graph.\n");
	printf("6.Depth first search.\n");
	printf("7.Breadth first search.\n");
	printf("8.Dijkstra's algorithm.\n");
	printf("9.Exit.\n");
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
			case 8: dijkstras(&list);
					break;
			case 9: run = FALSE;
					break;
			default:
					printf("%d invaild choice.\n", choice);
		}
	}

	if(list != NULL)
    	free_list(&list);
	return 0;
}
