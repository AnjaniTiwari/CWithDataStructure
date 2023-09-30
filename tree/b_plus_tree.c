/* B+ Tree implementation my own logic. */

#include <stdio.h>
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
	data *left, *right;
}data_lr;

typedef struct {
	data *left;
}data_l;

typedef struct {
	data *right;
}data_r;

typedef struct {
	data *next;
}data_next;

short tree_degree = 1;

void create_bp_tree(bp_tree** tree) {
	*tree = (bp_tree*)malloc(sizeof(bp_tree));
	if(!(*tree)) {
		printf("B+ tree memory allocation failed.\n");
	}
	(*tree)->d_array = NULL;
	(*tree)->d_index = -1;
}

int d_array_grow(bp_tree** tree) {
	 (*tree)->d_array = (data**)realloc((*tree)->d_array, (sizeof((*tree)->d_array) * (++(*tree)->d_index + 1)));
	 if(!(*tree)->d_array) {
		printf("get_data_memory allocation failed.\n");
		return 1;
	 }
	 (*tree)->d_array[(*tree)->d_index] = NULL;
	 return 0;
}

void create_id(bp_tree** tree, int id) {
	(*tree)->d_array[(*tree)->d_index] = (data*)malloc(sizeof(data));
	if(!(*tree)->d_array[(*tree)->d_index]) {
		printf("create_id allocation failed.\n");
		return;
	}
	(*tree)->d_array[(*tree)->d_index]->id = id;
	(*tree)->d_array[(*tree)->d_index]->ptr = NULL;
}

void add_id(bp_tree** tree, int id) {
	if(!d_array_grow(tree)) {
		create_id(tree, id);
	}
}

void split(bp_tree** tree) {

}

void insert(bp_tree** tree, int id) {
	if(*tree == NULL)
		create_bp_tree(tree);
	add_id(tree, id);
	if((*tree)->d_index == tree_degree-1)
        split(tree);
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