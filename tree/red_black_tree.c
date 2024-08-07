/*******************************************************************************
0 - Black
1 - Red
2 - Red Red Conflict
3 - Reverse recolor
4 - double black

RB for Red Black Node
db for Double Black Node
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define BLACK 0
#define RED 1
#define LEFT 0
#define RIGHT 1
#define FALSE 0
#define TRUE 1

typedef struct RB_Node RB_Node;

struct RB_Node {
	int data;
	unsigned short color:2;
	unsigned short double_black;
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
		printf("%d data is already exist.\n", data);
	}
}

void insert_RB_Node(RB_Node** root, int data) {
	insert(root, data);
	if((*root)->color != BLACK)
		(*root)->color = BLACK;
}

void remove_db_node(RB_Node** root, unsigned short db_dir) {
	unsigned short color;
	if(db_dir == LEFT) {
		if((*root)->right != NULL) {
			if((*root)->right->color == RED) {
				//case 4
				color = (*root)->color;
				(*root)->color = (*root)->right->color;
				(*root)->right->color = color;
				left_rotation(root);
				remove_db_node(&(*root)->left, LEFT);
			}
			else {
				if((*root)->right->right != NULL &&
					(*root)->right->right->color == RED) {
					//case 6
					if((*root)->color != (*root)->right->color) {
						color = (*root)->color;
						(*root)->color = (*root)->right->color;
						(*root)->right->color = color;
					}
					right_rotation(root);
					free((*root)->left->left);
					(*root)->left->left = NULL;
					(*root)->right->color = BLACK;
				}
				else if((*root)->right->left != NULL &&
						(*root)->right->left->color == RED) {
						//case 5
						(*root)->right->color = RED;
						(*root)->right->left->color = BLACK;
						right_rotation(&(*root)->right);
						remove_db_node(root, LEFT);

				}
				else {
					//case 3
					if((*root)->color == RED)
						(*root)->color = BLACK;
					else
						(*root)->double_black = TRUE;
					(*root)->right->color = RED;

					if((*root)->left->left == NULL &&
						(*root)->left->right == NULL) {
						free((*root)->left);
						(*root)->left = NULL;
					}
					else
                    	(*root)->left->double_black = FALSE;
				}
			}
		}
	}
	else {
		if((*root)->left != NULL) {
			if((*root)->left->color == RED) {
				//case 4
				color = (*root)->color;
				(*root)->color = (*root)->left->color;
				(*root)->left->color = color;
				right_rotation(root);
				remove_db_node(root, RIGHT);
			}
			else {
				if((*root)->left->left != NULL &&
					(*root)->left->left->color == RED) {
					//case 6
					color = (*root)->color;
					(*root)->color = (*root)->left->color;
					(*root)->left->color = color;
					right_rotation(root);
					(*root)->left->color = BLACK;
					free((*root)->right->right);
					(*root)->right->right = NULL;
				}
				else if((*root)->left->right != NULL &&
						(*root)->left->right->color == RED) {
						//case 5
						(*root)->left->color = RED;
						(*root)->left->right->color = BLACK;
						left_rotation(&(*root)->left);
						remove_db_node(root, RIGHT);
				}
				else {
					//case 3
					if((*root)->color == RED)
						(*root)->color = BLACK;
					else
						(*root)->double_black = TRUE;
					(*root)->left->color = RED;

					if((*root)->right->left == NULL &&
						(*root)->right->right == NULL) {
							free((*root)->right);
							(*root)->right = NULL;
					}
					else
						(*root)->right->double_black = FALSE;
				}
			}
		}
	}
}

//db for double black node
void in_order_predecessor(RB_Node** root, int* data) {
	if((*root)->right != NULL) {
		if((*root)->right->right != NULL)
			in_order_predecessor(&(*root)->right, data);
		else {
			*data = (*root)->right->data;
			if((*root)->right->color == RED) {
				free((*root)->right);
				(*root)->right = NULL;
			}
			else {
				(*root)->right->double_black = TRUE;
				remove_db_node(root, RIGHT);
            }
        }
	}
	else if((*root)->left != NULL) {
		*data = (*root)->data;
		in_order_predecessor(&(*root)->left, &(*root)->data);
		if((*root)->left != NULL && (*root)->left->double_black == TRUE) {
			remove_db_node(root, LEFT);
        }
	}
	else {
		*data = (*root)->data;
		if((*root)->color == RED) {
			free(*root);
			*root = NULL;
		}
		else
			(*root)->double_black = TRUE;
    }
}

void in_order_successor(RB_Node** root, int* data) {
	if((*root)->left != NULL) {
		in_order_successor(&(*root)->left, data);
		if((*root)->left != NULL && (*root)->left->double_black == TRUE)
        	remove_db_node(root, LEFT);
	}
	else {
		*data = (*root)->data;
		if((*root)->color == RED) {
			free(*root);
			*root = NULL;
		}
		else
        	(*root)->double_black = TRUE;
	}
}

void delection(RB_Node** root, int data) {
	if(*root != NULL) {
		if(data == (*root)->data) {
			if((*root)->left != NULL) {
				//in-order predecessor
				in_order_predecessor(&(*root)->left, &(*root)->data);
				if((*root)->left != NULL && (*root)->left->double_black == TRUE)
                	remove_db_node(root, LEFT);
			}
			else if((*root)->right != NULL) {
				//in-order successor
				in_order_successor(&(*root)->right, &(*root)->data);
				if((*root)->right != NULL && (*root)->right->double_black == TRUE)
                    remove_db_node(root, RIGHT);
			}
			else {
				if((*root)->color == RED) {
					//case 1
					free(*root);
					*root = NULL;
				}
				else
					(*root)->double_black = TRUE;
				return;
			}
		}
		else if(data < (*root)->data) {
			delection(&(*root)->left, data);
			if((*root)->left != NULL && (*root)->left->double_black == TRUE)
				remove_db_node(root, LEFT);
		}
		else if(data > (*root)->data) {
			delection(&(*root)->right, data);
			if((*root)->right != NULL && (*root)->right->double_black == TRUE)
				remove_db_node(root, RIGHT);
		}
	}
	else
		printf("%d node is not present in tree", data);
}

void delection_RB_Node(RB_Node** root, int data) {
	delection(root, data);
	if((*root) != NULL) {
		if((*root)->color == RED)
        	(*root)->color = BLACK;
		else {
			if((*root)->double_black == TRUE) {
				if((*root)->left != NULL || (*root)->right != NULL) {
                	//case 2
					(*root)->double_black = FALSE;
				}
				else {
					free(*root);
					*root = NULL;
				}
			}
		}
    }
}

void dummy_rb_tree(RB_Node** root) {
	allocate_RB_Node(root);
	(*root)->data = 50;

    //left sub tree
	allocate_RB_Node(&(*root)->left);
	(*root)->left->data = 30;
	allocate_RB_Node(&(*root)->left->left);
	(*root)->left->left->data = 15;
	allocate_RB_Node(&(*root)->left->right);
	(*root)->left->right->data = 35;

	//right sub tree
	allocate_RB_Node(&(*root)->right);
	(*root)->right->data = 65;
	allocate_RB_Node(&(*root)->right->left);
	(*root)->right->left->data = 55;
	allocate_RB_Node(&(*root)->right->right);
	(*root)->right->right->data = 70;
	(*root)->right->right->color = RED;

	//right sub tree sub tree
	allocate_RB_Node(&(*root)->right->right->left);
	(*root)->right->right->left->data = 68;
	allocate_RB_Node(&(*root)->right->right->right);
	(*root)->right->right->right->data = 80;

	allocate_RB_Node(&(*root)->right->right->right->right);
	(*root)->right->right->right->right->data = 90;
	(*root)->right->right->right->right->color = RED;



}
int main() {
	int arr[] = {8, 18, 5, 17, 15, 25, 40, 80};
//	int arr[] = {55, 30, 90, 80, 50, 35, 15, 65, 70, 68};
	unsigned short i, arr_size;
	RB_Node* root = NULL;

	arr_size = sizeof(arr)/sizeof(arr[0]);

//    dummy_rb_tree(&root);

	for(i = 0; i < arr_size; i++) {
		insert_RB_Node(&root, arr[i]);
	}

	for(i = 0; i < arr_size; i++) {
		delection_RB_Node(&root, arr[i]);
	}


	getch();
	return 0;
}

