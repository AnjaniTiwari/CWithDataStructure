/* B+ Tree implementation my own logic.
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>



short degree;
typedef struct bp_tree bp_tree;
typedef struct node{
	int value;
	bp_tree* left;
	bp_tree* right;
}node;

struct bp_tree{
	node** arr_node;
	int arr_node_index;
};

void create_tree(bp_tree** tree) {
	*tree = (bp_tree*)malloc(sizeof(bp_tree));
	if(*tree == NULL) {
		printf("In create_tree memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node = NULL;
	(*tree)->arr_node_index = -1;
}

void grow_node_array(bp_tree** tree, int index) {
	(*tree)->arr_node_index+=index;
   (*tree)->arr_node = (node**)realloc((*tree)->arr_node, (sizeof(node*)*((*tree)->arr_node_index+1)));
	if((*tree)->arr_node == NULL) {
		printf("In create_tree memory allocation failed.\n");
    	(*tree)->arr_node_index-=index;
		exit(1);
	}
}

void set_node_value(bp_tree** tree, int i, int value) {
	(*tree)->arr_node[i] = (node*)malloc(sizeof(node));
	if((*tree)->arr_node[i] == NULL) {
		printf("In set_node_value memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node[i]->value = value;
	(*tree)->arr_node[i]->left = NULL;
	(*tree)->arr_node[i]->right = NULL;
}

void shift_right(bp_tree** tree, int index) {
	int i;
	for(i = (*tree)->arr_node_index; i > index; i--)
		(*tree)->arr_node[i] = (*tree)->arr_node[i-1];
}

void insert_left(bp_tree** left, bp_tree** tree, int index) {
	int i;
	for(i = 0; i <= index; i++) {
		(*left)->arr_node[i] = (*tree)->arr_node[i];
		(*tree)->arr_node[i] = NULL;
	}
}

void shift_left(bp_tree** tree, int index) {
	int i;
	for(i = 0; i <= (*tree)->arr_node_index-index; i++)
		(*tree)->arr_node[i] = (*tree)->arr_node[index+i];
}

void shrink_node_array(bp_tree** tree, int shrink_nodes) {
	(*tree)->arr_node_index-=shrink_nodes;
	(*tree)->arr_node = (node**)realloc((*tree)->arr_node, sizeof(node*)* ((*tree)->arr_node_index+1));
	if((*tree)->arr_node == NULL) {
		printf("In shrink_node_array memory reallocation failed.\n");
		(*tree)->arr_node_index+=shrink_nodes;
		exit(1);
	}
}

void left_split_leaf_node(bp_tree** tree) {
	int mid, i;
	bp_tree* root = NULL, *right = NULL;

	mid = (*tree)->arr_node_index/2;

	create_tree(&root);
	grow_node_array(&root, 1);
	set_node_value(&root, 0, (*tree)->arr_node[mid]->value);

	create_tree(&right);
	grow_node_array(&right, (*tree)->arr_node_index-mid);
	for(i = 0; i < (*tree)->arr_node_index-mid; i++) {
		right->arr_node[i] = (*tree)->arr_node[mid+1+i];
		(*tree)->arr_node[mid+1+i] = NULL;
	}

	shrink_node_array(tree, (*tree)->arr_node_index-mid);

	root->arr_node[0]->left = *tree;
	root->arr_node[0]->left->arr_node[root->arr_node[0]->left->arr_node_index]->right = right;
	root->arr_node[0]->right = right;

	*tree = root;

}

void right_split_leaf_node(bp_tree** tree) {
	int mid;
	bp_tree* root = NULL, *left = NULL;

	mid = (*tree)->arr_node_index/2;

	create_tree(&left);
	grow_node_array(&left, mid);
	insert_left(&left, tree, mid-1);

	create_tree(&root);
	grow_node_array(&root, 1);
	set_node_value(&root, 0, (*tree)->arr_node[mid]->value);

	shift_left(tree, mid);
	shrink_node_array(tree, mid);

	root->arr_node[0]->left = left;
	root->arr_node[0]->left->arr_node[root->arr_node[0]->left->arr_node_index]->right = *tree;
	root->arr_node[0]->right = *tree;

	*tree = root;
}

void insert_node(bp_tree** tree, int value, int i) {
	grow_node_array(tree, 1);
	if(value < (*tree)->arr_node[i]->value)
		shift_right(tree, i);
	else
		i++;
	set_node_value(tree, i, value);
}

void split_node(bp_tree** tree) {
	int mid, i, count = 0;
	bp_tree* left = NULL, *right = NULL;

	mid = (*tree)->arr_node_index/2;

	if((*tree)->arr_node[mid]->left != NULL) {
		(*tree)->arr_node[mid-1]->right = (*tree)->arr_node[mid]->left;
		(*tree)->arr_node[mid]->left = NULL;
    }

	if((*tree)->arr_node[mid]->right != NULL) {
		(*tree)->arr_node[mid+1]->left = (*tree)->arr_node[mid]->right;
    	(*tree)->arr_node[mid]->right = NULL;
	}

	create_tree(&left);
	grow_node_array(&left, mid);
	for(i = 0; i < mid; i++) {
		left->arr_node[i] = (*tree)->arr_node[i];
		(*tree)->arr_node[i] = NULL;
		count++;
	}

	create_tree(&right);
	grow_node_array(&right, (*tree)->arr_node_index-mid);
	for(i = 0; (mid+1+i) <= (*tree)->arr_node_index; i++) {
		right->arr_node[i] = (*tree)->arr_node[mid+1+i];
		(*tree)->arr_node[mid+1+i] = NULL;
		count++;
	}

	(*tree)->arr_node[0] = (*tree)->arr_node[mid];
	(*tree)->arr_node[mid] = NULL;
	shrink_node_array(tree, count);

	(*tree)->arr_node[0]->left = left;
	(*tree)->arr_node[0]->right = right;
}

void insert(bp_tree** tree, int value) {
	int i;
	if(*tree == NULL) {
		create_tree(tree);
		grow_node_array(tree, 1);
		set_node_value(tree, 0, value);
	}
	else {
		for(i = 0; i <= (*tree)->arr_node_index; i++) {
			if(value < (*tree)->arr_node[i]->value) {
				if((*tree)->arr_node[i]->left != NULL) {
					insert(&(*tree)->arr_node[i]->left, value);
					if((*tree)->arr_node[i]->left->arr_node_index == 0) {
						grow_node_array(tree, 1);
						shift_right(tree, i);
						(*tree)->arr_node[i] = (*tree)->arr_node[i+1]->left->arr_node[0];
						(*tree)->arr_node[i+1]->left->arr_node[0] = NULL;
						free((*tree)->arr_node[i+1]->left->arr_node);
						free((*tree)->arr_node[i+1]->left);
						(*tree)->arr_node[i+1]->left = NULL;
					}
				}
				else {
					insert_node(tree, value, i);
					if((*tree)->arr_node_index == degree-1) {
						left_split_leaf_node(tree);
					}
				}
				break;
			}
			else if(i == (*tree)->arr_node_index) {
				if((*tree)->arr_node[i]->right != NULL) {
					insert(&(*tree)->arr_node[i]->right, value);
					if((*tree)->arr_node[i]->right->arr_node_index == 0) {
						grow_node_array(tree, 1);
						(*tree)->arr_node[i+1] = (*tree)->arr_node[i]->right->arr_node[0];
						(*tree)->arr_node[i]->right->arr_node[0] = NULL;
						free((*tree)->arr_node[i]->right->arr_node);
						free((*tree)->arr_node[i]->right);
						(*tree)->arr_node[i]->right = NULL;
						(*tree)->arr_node[i]->left->arr_node[(*tree)->arr_node[i]->left->arr_node_index]->right = (*tree)->arr_node[i+1]->left;
					}
				}
				else {
					insert_node(tree, value, i);
                	if((*tree)->arr_node_index == degree-1) {
						right_split_leaf_node(tree);
					}
				}
				break;
			}
		}

		if((*tree)->arr_node_index == degree-1) {
			split_node(tree);
		}
	}
}

void print_ascending_order(bp_tree* tree) {
	int i;
	while(tree->arr_node[0]->left != NULL)
		tree = tree->arr_node[0]->left;

	do{
		for(i = 0; i <= tree->arr_node_index; i++)
			printf("%d ", tree->arr_node[i]->value);
    	tree = tree->arr_node[i-1]->right;
	}while(tree);
}

int main() {
	int value[] = {5, 10, 15, 25, 35, 40, 45, 50, 60};
	bp_tree* tree = NULL;
	int i, size = sizeof(value) / sizeof(value[0]);
	degree = 3;
	for(i = 0; i < size; i++)
		insert(&tree, value[size-1-i]);

	print_ascending_order(tree);

	getch();
	return 0;
}
