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


void create_bp_tree(bp_tree** tree) {
	*tree = (bp_tree*)malloc(sizeof(bp_tree));
	if(!(*tree)) {
        printf("B+ tree memory allocation failed.\n");
	}
}

void add_id(bp_tree** tree, int id) {

}

void insert(bp_tree** tree, int id) {
	if(*tree == NULL)
		create_bp_tree(tree);
    add_id(tree, id);
}

int main() {
	int id_array[] = {5, 10, 15, 25, 35, 40, 45, 50, 60};
	bp_tree* tree = NULL;

    insert(&tree, id_array[0]);

	return 0;
}