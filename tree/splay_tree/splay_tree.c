#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct splay_node splay_node;

struct splay_node {
	int data;
	splay_node* left;
	splay_node* right;
};

void allocate_splay_node(splay_node** node, int data) {
	(*node) = (splay_node*)malloc(sizeof(splay_node));
	if(!node) {
		printf("memory allocation failed in allocate_splay_node function.\n");
	}
	else {
		(*node)->data = data;
		(*node)->left = NULL;
		(*node)->right = NULL;
	}
}

void zig_rotation(splay_node** root) {
	splay_node* temp = (*root)->left;
	(*root)->left = temp->right;
	temp->right = *root;
	*root = temp;
}

void zag_rotation(splay_node** root) {
	splay_node* temp = (*root)->right;
	(*root)->right = temp->left;
	temp->left = *root;
    *root = temp;
}

void insert_splay_node(splay_node** root, int data) {
	if(!(*root)) {
		allocate_splay_node(root, data);
	}
	else {
		if(data < (*root)->data) {
			insert_splay_node(&(*root)->left, data);
            zig_rotation(root);
		}
		else if(data > (*root)->data) {
			insert_splay_node(&(*root)->right, data);
			zag_rotation(root);
		}

	}
}

int main() {
	int arr[] = {10, 7, 15, 1, 9, 13, 20, -1, 8, 11, 14, 30};
	unsigned int arr_count = sizeof(arr)/sizeof(arr[0]);
	splay_node* root = NULL;
	int i;
	for(i = 0; i < arr_count; i++)
		insert_splay_node(&root, arr[i]);

	getch();
	return 0;
}
