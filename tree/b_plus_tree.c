/* B+ Tree implementation my own logic. */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct {
	int id;
	void *ptr;
}data;

typedef struct {
	data **d_array;
	int d_index;
}bp_tree;

typedef struct {
	bp_tree *left, *right;
}bp_tree_lr;

typedef struct {
	bp_tree *left;
}bp_tree_l;

typedef struct {
	bp_tree *right;
}bp_tree_r;

typedef struct {
	data *next;
}next_data_ptr;

short tree_degree = 1;

int create_next_data_ptr(next_data_ptr** ptr);

void create_bp_tree(bp_tree** tree) {
	*tree = (bp_tree*)malloc(sizeof(bp_tree));
	if(!(*tree)) {
		printf("B+ tree memory allocation failed.\n");
	}
	(*tree)->d_array = NULL;
	(*tree)->d_index = -1;
}

int grow_d_array(bp_tree** tree, int total_grow) {
	 (*tree)->d_index += total_grow;
	 (*tree)->d_array = (data**)realloc((*tree)->d_array, (sizeof((*tree)->d_array) * ((*tree)->d_index + 1)));
	 if(!(*tree)->d_array) {
		printf("get_data_memory allocation failed.\n");
		return 1;
	 }
	 (*tree)->d_array[(*tree)->d_index] = NULL;
	 return 0;
}

int shrink_d_array(bp_tree** tree, int total_shrink) {
	(*tree)->d_index -= total_shrink;
	if((*tree)->d_index >= 0) {
		(*tree)->d_array = (data**)realloc((*tree)->d_array, (sizeof((*tree)->d_array) * ((*tree)->d_index + 1)));
		if(!(*tree)->d_array) {
			printf("get_data_memory allocation failed.\n");
			return 1;
		}
		(*tree)->d_array[(*tree)->d_index] = NULL;
	}
	else {
		free((*tree)->d_array);
		(*tree)->d_array = NULL;
		(*tree)->d_index = -1;
	}
	return 0;
}

void create_id(bp_tree** tree, int i, int id) {
	(*tree)->d_array[i] = (data*)malloc(sizeof(data));
	if(!(*tree)->d_array[i]) {
		printf("create_id allocation failed.\n");
		return;
	}
	(*tree)->d_array[i]->id = id;
	(*tree)->d_array[i]->ptr = NULL;
	if(i > 0) {
		if(!create_next_data_ptr(&(next_data_ptr*)(*tree)->d_array[i-1]->ptr))
			((next_data_ptr*)(*tree)->d_array[i-1]->ptr)->next = (*tree)->d_array[i];
	}
	if(i < (*tree)->d_index) {

	}

}

void shift(bp_tree** tree, int i, char LR) {
	int j;
	if(LR == 'L') {

	}
	else {
		for(j = (*tree)->d_index; j > i ; j--) {
			(*tree)->d_array[j] = (*tree)->d_array[j-1];
		}
	}
}

void add_id(bp_tree** tree, int i, int id) {
	if(!grow_d_array(tree, 1)) {
		 if(i < (*tree)->d_index-1)
			shift(tree, i, 'R');
		 create_id(tree, i, id);
	}
}

int create_lr_tree(bp_tree_lr** mid_tree) {
	*mid_tree = (bp_tree_lr*)malloc(sizeof(bp_tree_lr));
	if(!(*mid_tree)) {
		printf("create_lr_tree allocation failed.\n");
		return -1;
	}
	return 0;
}

int create_next_data_ptr(next_data_ptr** ptr) {
	*ptr = (next_data_ptr*)malloc(sizeof(next_data_ptr));
	if(!(*ptr)) {
		printf("create_next_data_ptr allocation failed.\n");
		return -1;
	}
	return 0;
}

void split(bp_tree** tree) {
	short mid_index = tree_degree & 1 ? tree_degree / 2 : (tree_degree / 2) - 1;
	short r_index = tree_degree - mid_index;
	short i;
	bp_tree *r_tree = NULL, *mid_tree = NULL;
	create_bp_tree(&r_tree);
	create_bp_tree(&mid_tree);
	grow_d_array(&r_tree, r_index);
	grow_d_array(&mid_tree, 1);
	for(i = 0; i <= r_tree->d_index; i++)
		r_tree->d_array[i] = (*tree)->d_array[mid_index + i];
	mid_tree->d_array[mid_tree->d_index] = (*tree)->d_array[mid_index];
	if(mid_tree->d_array[mid_tree->d_index]->ptr != NULL) {
		free(mid_tree->d_array[mid_tree->d_index]->ptr);
	}
	shrink_d_array(tree, r_index);
	if(!create_lr_tree(&(bp_tree_lr*)mid_tree->d_array[mid_tree->d_index]->ptr)) {
		if(!create_next_data_ptr(&(next_data_ptr*)(*tree)->d_array[(*tree)->d_index]->ptr)) {
			((next_data_ptr*)(*tree)->d_array[(*tree)->d_index]->ptr)->next = r_tree->d_array[0];
		}
		((bp_tree_lr*)mid_tree->d_array[mid_tree->d_index]->ptr)->left = *tree;
		((bp_tree_lr*)mid_tree->d_array[mid_tree->d_index]->ptr)->right = r_tree;
	}



}

void insert(bp_tree** tree, int id) {
	int i;
	if(*tree == NULL) {
		create_bp_tree(tree);
		add_id(tree, 0, id);
	}
	else {
		for(i = 0; i <= (*tree)->d_index; i++) {
			if(id < (*tree)->d_array[i]->id) {
				if((*tree)->d_array[i]->ptr == NULL)
					add_id(tree, i, id);
				else {

				}
				break;
			}
			else if(id > (*tree)->d_array[i]->id && i == (*tree)->d_index) {
				if((*tree)->d_array[i]->ptr == NULL)
					add_id(tree, i, id);
				else {

				}
				break;
			}
		}
		if((*tree)->d_index == tree_degree-1)
			split(tree);
	}
}

int main() {
	int id_array[] = {5, 10, 15, 25, 35, 40, 45, 50, 60};
	bp_tree* tree = NULL;
	int i, size = sizeof(id_array) / sizeof(id_array[0]);
	for(i = 0; i < size; i++)
		insert(&tree, id_array[i]);

	for(i = 0; i < size; i++)
		printf("%d ", tree->d_array[i]->id);

	getch();
	return 0;
}