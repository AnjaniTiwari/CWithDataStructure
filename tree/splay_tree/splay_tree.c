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

void inorder_predecessor(splay_node** root, int* data) {
	if(!(*root)->right) {
        splay_node* temp = NULL;
		*data = (*root)->data;
		if((*root)->left != NULL) {
			temp = (*root)->left;
		}
		free(*root);
		*root = temp;
	}
	else
    	inorder_predecessor(&(*root)->right, data);
}

void inorder_successor(splay_node** root, int* data) {
	if(!(*root)->left) {
		splay_node* temp = NULL;
		*data = (*root)->data;
		if((*root)->right != NULL)
			temp = (*root)->right;
		free(*root);
		*root = temp;
	}
	else
		inorder_successor(&(*root)->left, data);
}

void delete_bottom_up_splay_node(splay_node** root, int data) {
	if(data < (*root)->data) {
		if((*root)->left != NULL)
			delete_bottom_up_splay_node(&(*root)->left, data);
		if((*root)->left != NULL)
			zig_rotation(root);
	}
	else if(data > (*root)->data) {
		if((*root)->right != NULL)
			delete_bottom_up_splay_node(&(*root)->right, data);
		if((*root)->right != NULL)
			zag_rotation(root);
	}
	else {
		if((*root)->left != NULL)
			inorder_predecessor(&(*root)->left, &(*root)->data);
		else if((*root)->right != NULL)
			inorder_successor(&(*root)->right, &(*root)->data);
		else {
			free(*root);
			*root = NULL;
		}
    }
}

void delete_bottom_up(splay_node** root, int data) {
	if((*root) != NULL)
		delete_bottom_up_splay_node(root, data);
	else
        printf("Empty tree.\n");
}

int main() {
	int arr[] = {10, 7, 15, 1, 9, 13, 20, -1, 8, 11, 14, 30};
	int arr2[] = {14, -1, 1, 11, 13, 10, 7, 9, 30, 20, 15, 8};
	unsigned int arr_count = sizeof(arr)/sizeof(arr[0]);
	splay_node* root = NULL;
	int i;

	for(i = 0; i < arr_count; i++)
		insert_splay_node(&root, arr[i]);

	for(i = 0; i < arr_count; i++)
		delete_bottom_up(&root, arr2[i]);

	getch();
	return 0;
}
