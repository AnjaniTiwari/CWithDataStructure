/*
0 - Black
1 - Red
2 - Red Red Conflict
3 - Reverse recolor
*/

#include <stdio.h>
#include <stdlib.h>


typedef struct RB_Node RB_Node;

struct RB_Node {
	int data;
	unsigned short color:2;
	RB_Node *left, *right;
};




void allocate_RB_Node(RB_Node** root) {
	*root = (RB_Node*)calloc(1, sizeof(RB_Node));
	if(*root == NULL) {
		printf("allocate_RB_Node memory allocation failed.\n");
		exit(1);
	}
}

void left_rotation(RB_Node** root) {
	RB_Node* temp = NULL;
	temp = *root;
	*root = (*root)->right;
	temp->right = (*root)->left;
	(*root)->left = temp;
}

void right_rotation(RB_Node** root) {
	RB_Node* temp = NULL;
	temp = *root;
	*root = (*root)->left;
	temp->left = (*root)->right;
    (*root)->right = temp;
}


void rotation(RB_Node** g_root, unsigned short rotation_direction) {
	//rotation_direction 0 - left , 1 - right
	unsigned short color;
	if(rotation_direction == 0) {
		if((*g_root)->right->right == NULL && (*g_root)->right->left != NULL) {
			//right-left rotation
			right_rotation(&(*g_root)->right);
			left_rotation(&(*g_root));
		}
		else {
			left_rotation(g_root);
			color = (*g_root)->color;
			(*g_root)->color = (*g_root)->left->color;
			(*g_root)->left->color = color;
		}

	}
	else {
		if((*g_root)->left->left == NULL && (*g_root)->left->right != NULL) {
			//left-right rotation
			left_rotation(&(*g_root)->left);
			right_rotation(&(*g_root));
			color = (*g_root)->color;
			(*g_root)->color = (*g_root)->right->color;
			(*g_root)->right->color = color;
		}
		else {
			right_rotation(g_root);
			color = (*g_root)->right->color;
			(*g_root)->right->color = (*g_root)->color;
			(*g_root)->color = color;
		}
	}
}


void insert(RB_Node** root, int data) {
	if((*root) == NULL) {
		allocate_RB_Node(root);
		(*root)->data = data;
		(*root)->color = 1;
		return;
	}

	if(data < (*root)->data) {
		insert(&(*root)->left, data);
		if((*root)->left != NULL) {
			if((*root)->color == 1 && (*root)->left->color == 1) {
				(*root)->color = 2;
			}
			else if((*root)->left->color == 2) {
				if((*root)->right == NULL || (*root)->right->color == 0) {
					(*root)->left->color = 1;
					rotation(root, 1);
				}
				else if((*root)->right->color == 1) {
					(*root)->left->color = 0;
					(*root)->right->color = 0;
					(*root)->color = 1;
				}
			}
		}
	}
	else if(data > (*root)->data){
		insert(&(*root)->right, data);
		if((*root)->right != NULL) {
			if((*root)->color == 1 && (*root)->right->color == 1) {
				(*root)->color = 2;
			}
			else if((*root)->right->color == 2) {
				if((*root)->left == NULL) {
                	(*root)->right->color = 1;
                    rotation(root, 0);
				}
				else if((*root)->left->color == 1) {
					(*root)->left->color = 0;
					(*root)->right->color = 0;
					(*root)->color = 1;
				}
				else {
                	(*root)->right->color = 1;
                    rotation(root, 0);
				}
			}
		}
	}
	else {
		printf("%d data is already exits.\n", data);
	}
}

void insert_RB_Node(RB_Node** root, int data) {
	insert(root, data);
	if((*root)->color != 0)
    	(*root)->color = 0;
}

int main() {
	int arr[] = {8, 18, 5, 17, 15, 25, 40, 80};
	unsigned short i, arr_size;
	RB_Node* root = NULL;

	arr_size = sizeof(arr)/sizeof(arr[0]);

	for(i = 0; i < arr_size; i++) {
		insert_RB_Node(&root, arr[arr_size-1-i]);
	}

	getch();
	return 0;
}

