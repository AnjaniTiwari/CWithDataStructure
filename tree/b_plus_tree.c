/* B+ Tree implementation my own logic. */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>



short degree;
typedef struct bp_tree bp_tree;
typedef struct node node;

typedef struct data {
	int id;
	void* db_ptr;
}data;

typedef struct node {
	data** data_arr;
	int data_index;
	node* next;
}node;

struct bp_tree {
	node* arr_node;
	bp_tree** bp_tree_arr;
	int bp_tree_index;
};

void create_bp_tree(bp_tree** tree) {
	*tree = (bp_tree*)malloc(sizeof(bp_tree));
	if(*tree == NULL) {
		printf("create_bp_tree memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node = NULL;
	(*tree)->bp_tree_arr = NULL;
	(*tree)->bp_tree_index = -1;
}

void create_node(bp_tree** tree) {
	(*tree)->arr_node = (node*)malloc(sizeof(node));
	if((*tree)->arr_node == NULL) {
		printf("create_node memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node->data_arr = NULL;
	(*tree)->arr_node->data_index = -1;
	(*tree)->arr_node->next = NULL;
}

void grow_data_array(bp_tree** tree, int size) {
	(*tree)->arr_node->data_index+=size;
	(*tree)->arr_node->data_arr = (data**)realloc((*tree)->arr_node->data_arr, sizeof(data*)*((*tree)->arr_node->data_index+1));
	if((*tree)->arr_node == NULL) {
		printf("grow_data_array memory allocation failed.\n");
		exit(1);
	}
}

void create_data(bp_tree** tree, int i, int id) {
	(*tree)->arr_node->data_arr[i] = (data*)malloc(sizeof(data));
	if((*tree)->arr_node->data_arr[i] == NULL) {
		printf("create_data memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node->data_arr[i]->id = id;
	(*tree)->arr_node->data_arr[i]->db_ptr = NULL;
}

void shift_right_data_arr(bp_tree** tree, int end) {
	int i;
	for(i = (*tree)->arr_node->data_index; i > end; i--)
		(*tree)->arr_node->data_arr[i] = (*tree)->arr_node->data_arr[i-1];
}

void grow_bp_tree_array(bp_tree** tree, int size) {
	(*tree)->bp_tree_index+=size;
	(*tree)->bp_tree_arr = (bp_tree**)realloc((*tree)->bp_tree_arr, ((*tree)->bp_tree_index+1) * sizeof(bp_tree*));
	if((*tree)->bp_tree_arr == NULL) {
		printf("grow_bp_tree_array memory allocation failed.\n");
		exit(1);
	}
}

void shrink_data_array(bp_tree** tree, int size) {
	(*tree)->arr_node->data_index-=size;
	(*tree)->arr_node->data_arr = (data**)realloc((*tree)->arr_node->data_arr, sizeof(data*)*((*tree)->arr_node->data_index+1));
	if((*tree)->arr_node == NULL) {
		printf("shrink_data_array memory allocation failed.\n");
		exit(1);
	}
}

int find_input_index(bp_tree** tree, int id) {
	int i;
	for(i = 0; i < (*tree)->arr_node->data_index; i++) {
		if(id < (*tree)->arr_node->data_arr[i]->id) {
			shift_right_data_arr(tree, i);
			break;
		}
	}
	return i;
}

void shift_right_bp_tree_arr(bp_tree** tree, int end) {
	int i;
	for(i = (*tree)->bp_tree_index; i > end; i--)
		(*tree)->bp_tree_arr[i] = (*tree)->bp_tree_arr[i-1];
}

void shift_left_bp_tree_arr(bp_tree** tree, int start) {
	int i;
	for(i = 0; i < (*tree)->bp_tree_index; i++)
		(*tree)->bp_tree_arr[i] = (*tree)->bp_tree_arr[start+i];
}

void copy_bp_tree_array(bp_tree** c_tree, int mid, bp_tree** m_tree) {
	int i;
	for(i = 0; i < mid; i++)
		(*c_tree)->bp_tree_arr[i] = (*m_tree)->bp_tree_arr[i];
}

void copy_data_arr(bp_tree** c_tree, int mid, bp_tree** m_tree) {
	int i;
	for(i = 0; i < mid; i++)
		(*c_tree)->arr_node->data_arr[i] = (*m_tree)->arr_node->data_arr[i];
}

void shrink_bp_tree_arr(bp_tree** tree, int size) {
	(*tree)->bp_tree_index-=size;
	(*tree)->bp_tree_arr = (bp_tree**)realloc((*tree)->bp_tree_arr, (sizeof(bp_tree*)*((*tree)->bp_tree_index+1)));
	if((*tree)->arr_node == NULL) {
		printf("shrink_bp_tree_arr memory allocation failed.\n");
		exit(1);
	}
}

void split_bp_tree(bp_tree** root, bp_tree** tree) {
	bp_tree* left = NULL;
	int mid, i, index = 0, flag = 0;

	mid = (*tree)->arr_node->data_index/2;

	//root
	if(*root == NULL) {
		flag = 1;
		create_bp_tree(root);
		create_node(root);
	}

	grow_data_array(root, 1);
	index = find_input_index(root, (*tree)->arr_node->data_arr[mid]->id);
	if((*tree)->bp_tree_index == -1) {
		create_data(root, index, (*tree)->arr_node->data_arr[mid]->id);
	}
	else {
		(*root)->arr_node->data_arr[index] = (*tree)->arr_node->data_arr[mid];
		(*tree)->arr_node->data_arr[mid] = NULL;
	}


	//left
	create_bp_tree(&left);
	create_node(&left);
	grow_data_array(&left, mid);
	copy_data_arr(&left, mid, tree);

	if((*tree)->bp_tree_arr != NULL) {
		grow_bp_tree_array(&left, mid+1);
		copy_bp_tree_array(&left, mid+1, tree);
		shift_left_bp_tree_arr(tree, mid+1);
		shrink_bp_tree_arr(tree, mid+1);
	}

	//right
	if((*tree)->arr_node->data_arr[mid] == NULL)
		mid++;

	for(i = 0; i <= mid; i++) {
		if(mid+i > (*tree)->arr_node->data_index)
			break;
		(*tree)->arr_node->data_arr[i] = (*tree)->arr_node->data_arr[mid+i];
	}

	shrink_data_array(tree, mid);

	if((*tree)->bp_tree_index == -1)
		left->arr_node->next = (*tree)->arr_node;

	grow_bp_tree_array(root, 1);
	if(index < (*root)->bp_tree_index)
		shift_right_bp_tree_arr(root, index);

	(*root)->bp_tree_arr[index] = left;

	if(index > 0 && (*root)->bp_tree_arr[index-1]->bp_tree_index == -1) {
		(*root)->bp_tree_arr[index-1]->arr_node->next = (*root)->bp_tree_arr[index]->arr_node;
    }

	if((*root)->bp_tree_index < index+1) {
		grow_bp_tree_array(root, 1);
		(*root)->bp_tree_arr[index+1] = *tree;
	}

	if(flag == 1)
		*tree = *root;

}

void insert_node(bp_tree** root, bp_tree** tree, int id) {
	if((*tree)->arr_node->data_arr == NULL) {
		grow_data_array(tree, 1);
		create_data(tree, 0, id);
	}
	else {
		int i;
		for(i = 0; i <= (*tree)->arr_node->data_index; i++) {
			if(id < (*tree)->arr_node->data_arr[i]->id) {
				if((*tree)->bp_tree_arr == NULL) {
					grow_data_array(tree, 1);
					shift_right_data_arr(tree, i);
					create_data(tree, i, id);
				}
				else {
					insert_node(tree, &(*tree)->bp_tree_arr[i], id);
				}
				break;
			}
			else if(i == (*tree)->arr_node->data_index) {
				if((*tree)->bp_tree_arr == NULL) {
					grow_data_array(tree, 1);
					create_data(tree, i+1, id);
				}
				else {
					insert_node(tree, &(*tree)->bp_tree_arr[i+1], id);
				}
				break;
			}
		}
		if((*tree)->arr_node->data_index == degree-1)
			split_bp_tree(root, tree);
	}
}

void insert(bp_tree** tree, int id) {
	bp_tree* root = NULL;
	if(*tree == NULL) {
		create_bp_tree(tree);
		create_node(tree);
	}
	insert_node(&root, tree, id);
}

void print_id(bp_tree* temp) {
    node* n_temp = NULL;
	int i;
	while(temp->bp_tree_index != -1)
		temp = temp->bp_tree_arr[0];
	n_temp = temp->arr_node;
	 do {
		for(i = 0; i <= n_temp->data_index; i++)
			printf("%d ", n_temp->data_arr[i]->id);
		n_temp = n_temp->next;
	}while(n_temp != NULL);
}

int main() {
	int id[] = {5, 15, 10, 25, 35, 40, 45, 50, 60};
	bp_tree* root = NULL;
	int i, size = sizeof(id) / sizeof(*id);
	degree = 3;
	
	for(i = 0; i < size; i++)
		insert(&root, id[size-1-i]);

	print_id(root);
	getch();
	return 0;
}
