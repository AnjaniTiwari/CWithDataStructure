/* B+ Tree implementation my own logic. */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "b_plus_tree.h"


void create_bp_tree(bp_tree** tree) {
	*tree = (bp_tree*)calloc(1, sizeof(bp_tree));
	if(*tree == NULL) {
		printf("create_bp_tree memory allocation failed.\n");
		exit(1);
	}
	(*tree)->bp_tree_index = -1;
}

void create_node(bp_tree** tree) {
	(*tree)->arr_node = (node*)calloc(1, sizeof(node));
	if((*tree)->arr_node == NULL) {
		printf("create_node memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node->data_index = -1;
}

void grow_data_array(bp_tree** tree, int size) {
	(*tree)->arr_node->data_index+=size;
	(*tree)->arr_node->data_arr = (data**)realloc((*tree)->arr_node->data_arr, sizeof(data*)*((*tree)->arr_node->data_index+1));
	if((*tree)->arr_node == NULL) {
		printf("grow_data_array memory allocation failed.\n");
		exit(1);
	}
}

void grow_data_array2(data*** data_arr, int* data_index, int size) {
	(*data_index)+=size;
	*data_arr = (data**)realloc(*data_arr, ((*data_index)+1) * sizeof(data*));
	if(!(*data_arr)) {
		printf("grow_data_array2 memory allocation failed.\n");
		exit(1);
	}
}

void create_data(bp_tree** tree, int i, int id) {
	(*tree)->arr_node->data_arr[i] = (data*)calloc(1, sizeof(data));
	if((*tree)->arr_node->data_arr[i] == NULL) {
		printf("create_data memory allocation failed.\n");
		exit(1);
	}
	(*tree)->arr_node->data_arr[i]->id = id;
}

void shift_left_data_arr(bp_tree** tree, int start) {
	int i;
	for(i = start; i < (*tree)->arr_node->data_index; i++)
		(*tree)->arr_node->data_arr[i] = (*tree)->arr_node->data_arr[i+1];
}

void shift_left_data_arr2(data*** data_arr, int start_index, int end_index) {
	int i;
	for(i = start_index; i < end_index; i++)
		(*data_arr)[i] = (*data_arr)[i+1];
}

void shift_right_data_arr(bp_tree** tree, int end) {
	int i;
	for(i = (*tree)->arr_node->data_index; i > end; i--)
		(*tree)->arr_node->data_arr[i] = (*tree)->arr_node->data_arr[i-1];
}

void shift_right_data_arr2(data*** temp, int begin_index, int end_index) {
	while(begin_index > end_index) {
		(*temp)[begin_index] = (*temp)[begin_index-1];
		--begin_index;
	}
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
	if((*tree)->arr_node->data_arr == NULL) {
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

void shift_left_bp_tree_arr(bp_tree** tree, int start, int cp_index) {
	int i;
	for(i = start; cp_index+i <= (*tree)->bp_tree_index; i++)
		(*tree)->bp_tree_arr[i] = (*tree)->bp_tree_arr[cp_index+i];
}

void copy_bp_tree_array(bp_tree** c_tree, int start, int end, bp_tree** m_tree) {
	int i;
	for(i = 0; i < end; i++) {
		(*c_tree)->bp_tree_arr[start+i] = (*m_tree)->bp_tree_arr[i];
		(*m_tree)->bp_tree_arr[i] = NULL;
	}
}

void copy_data_arr(bp_tree** c_tree, int start, int end, bp_tree** m_tree) {
	int i;
	for(i = 0 ; i < end; i++) {
		(*c_tree)->arr_node->data_arr[start+i] = (*m_tree)->arr_node->data_arr[i];
		(*m_tree)->arr_node->data_arr[i] = NULL;
	}
}

void copy_data_arr2(data*** dst_data_arr, int start_index, int end_index, data*** src_data_arr) {
	int i;
	for(i = 0 ; i < end_index; i++) {
		(*dst_data_arr)[start_index+i] = (*src_data_arr)[i];
		(*src_data_arr)[i] = NULL;
	}
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
		(*root)->bp_tree_height = (*tree)->bp_tree_height + 1;
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
	//function changed
	copy_data_arr(&left, 0, mid, tree);
    left->bp_tree_height = (*tree)->bp_tree_height;

	if((*tree)->bp_tree_arr != NULL) {
		grow_bp_tree_array(&left, mid+1);
		copy_bp_tree_array(&left, 0, mid+1, tree);
        //function changed please check
		shift_left_bp_tree_arr(tree, 0, mid+1);
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

int height(bp_tree* root, int i) {
	int h_count = 0;
	while(1) {
		if(root->bp_tree_arr == NULL) {
			h_count++;
			break;
		}
		root = root->bp_tree_arr[i];
		h_count++;
	}
	return h_count;
}

void free_bp_tree(bp_tree** root) {
	int i;
	if((*root)->bp_tree_arr != NULL) {
		free((*root)->bp_tree_arr);
	}
	if((*root)->arr_node->data_arr != NULL) {
		for(i = 0; i <= (*root)->arr_node->data_index; i++) {
			if((*root)->arr_node->data_arr[i] != NULL)
				free((*root)->arr_node->data_arr[i]);
		}
		free((*root)->arr_node->data_arr);
		free((*root)->arr_node);
	}
	free(*root);
	*root = NULL;
}

void copy_arr_node(bp_tree** root, int start) {
	int i;
	for(i = start; i < (*root)->bp_tree_index; i++)
		(*root)->bp_tree_arr[i]->arr_node = (*root)->bp_tree_arr[i+1]->arr_node;
    (*root)->bp_tree_arr[i]->arr_node = NULL;
}

void height_shrink(bp_tree** root, int i, short LR) {
	bp_tree* temp = NULL;
	int x, y;
	if(LR == 1) {
		temp = (*root)->bp_tree_arr[i];
		x = temp->arr_node->data_index;
		grow_data_array(&temp, (*root)->arr_node->data_index+1);
		copy_data_arr(&temp, x+1, (*root)->arr_node->data_index+1, root);
		x = temp->bp_tree_index;
		grow_bp_tree_array(&temp, (*root)->bp_tree_index);
		for(y = 1; y <= (*root)->bp_tree_index; y++) {
			temp->bp_tree_arr[x+y] = (*root)->bp_tree_arr[i+y];
			(*root)->bp_tree_arr[i+y] = NULL;
		}
		free_bp_tree(root);
        *root = temp;
	}
	else {

	}
	temp = NULL;
	if((*root)->arr_node->data_index >= degree-1) {
		split_bp_tree(&temp, root);
	}
}

bp_tree* get_smallest_node(bp_tree* root) {
	if(root != NULL) {
		while(root->bp_tree_arr != NULL) {
			root = root->bp_tree_arr[0];
        }
	}
	return root;
}

bp_tree* get_greatest_node(bp_tree* root) {
	if(root != NULL) {
		while(root->bp_tree_arr != NULL) {
			root = root->bp_tree_arr[root->bp_tree_index];
		}
	}
	return root;
}

void check_height(bp_tree** root) {
    bp_tree* temp = NULL;
	int i;
	for(i = 0; i < (*root)->bp_tree_index; i++) {
		if((*root)->bp_tree_arr[i]->bp_tree_height < (*root)->bp_tree_arr[i+1]->bp_tree_height) {
			if((*root)->arr_node->data_index == 0) {
					temp = (*root)->bp_tree_arr[i+1];
					(*root)->bp_tree_arr[i+1] = NULL;
					(*root)->bp_tree_height = temp->bp_tree_height;
					grow_data_array(root, temp->arr_node->data_index+1);
					copy_data_arr(root, (*root)->arr_node->data_index - temp->arr_node->data_index, (*root)->arr_node->data_index, &temp);
					grow_bp_tree_array(root, temp->bp_tree_index);
					copy_bp_tree_array(root, (*root)->bp_tree_index - temp->bp_tree_index, (*root)->bp_tree_index, &temp);
					free_bp_tree(&temp);
				}
				else {
					if((*root)->bp_tree_arr[i]->arr_node->next->data_index < degree-1) {
						grow_data_array2(&(*root)->bp_tree_arr[i]->arr_node->next->data_arr,
										 &(*root)->bp_tree_arr[i]->arr_node->next->data_index, 1);

						shift_right_data_arr2(&(*root)->bp_tree_arr[i]->arr_node->next->data_arr,
											   (*root)->bp_tree_arr[i]->arr_node->next->data_index, 0);
						(*root)->bp_tree_arr[i]->arr_node->next->data_arr[0] = (*root)->bp_tree_arr[i]->arr_node->data_arr[0];
						(*root)->bp_tree_arr[i]->arr_node->data_arr[0] = NULL;
						free_bp_tree((*root)->bp_tree_arr);
						shift_left_bp_tree_arr(root, 0, 1);
						shrink_bp_tree_arr(root, 1);
						free((*root)->arr_node->data_arr[i]);
						shift_left_data_arr(root, 0);
						shrink_data_array(root, 1);
					}
				}
		}
		else if((*root)->bp_tree_arr[i]->bp_tree_height > (*root)->bp_tree_arr[i+1]->bp_tree_height) {
        	int index = -1;
			if((*root)->arr_node->data_index == 0) {
						index = (*root)->bp_tree_arr[i]->arr_node->data_index;
						grow_data_array2(&(*root)->arr_node->data_arr,
										 &(*root)->arr_node->data_index,
										 index+1);

						shift_right_data_arr2(&(*root)->arr_node->data_arr,
											   (*root)->arr_node->data_index, index);

						copy_data_arr2(&(*root)->arr_node->data_arr, 0,
										(*root)->arr_node->data_index,
										&(*root)->bp_tree_arr[i]->arr_node->data_arr);

						grow_bp_tree_array(root, (*root)->bp_tree_arr[i]->bp_tree_index);
						shift_right_bp_tree_arr(root, i+1);
						(*root)->bp_tree_height = (*root)->bp_tree_arr[i]->bp_tree_height;
						temp = (*root)->bp_tree_arr[i];
						copy_bp_tree_array(root, i, (*root)->bp_tree_index, &temp);
						free_bp_tree(&temp);
			}
			else {
				if(i > 0) {
//                            bp_tree* temp2 = (*root);
					temp = (*root)->bp_tree_arr[i];
					while(temp->bp_tree_height != 0) {
//								temp2 = temp;
						temp = temp->bp_tree_arr[temp->bp_tree_index];
					}

					if( temp->arr_node->data_index < degree-1) {
						index = temp->arr_node->data_index + 1;
						grow_data_array2(&temp->arr_node->data_arr,
										 &temp->arr_node->data_index,
										 (*root)->bp_tree_arr[i+1]->arr_node->data_index+1);
						copy_data_arr2(&temp->arr_node->data_arr, index,
										temp->arr_node->data_index,
										&(*root)->bp_tree_arr[i+1]->arr_node->data_arr);
						temp->arr_node->next = (*root)->bp_tree_arr[i+1]->arr_node->next;     //useless
						free_bp_tree(&(*root)->bp_tree_arr[i+1]);
						if(i+1 < (*root)->bp_tree_index)
							shift_left_bp_tree_arr(root, i+1, i+2);
						shrink_bp_tree_arr(root, 1);
						if((*root)->arr_node->data_index > i) {
							free((*root)->arr_node->data_arr[i]);
							shift_left_data_arr2(&(*root)->arr_node->data_arr, i, (*root)->arr_node->data_index);
						}
						else {
							free((*root)->arr_node->data_arr[(*root)->arr_node->data_index]);
							shrink_data_array(root, 1);
						}

					}
				}
			}
		}
	}
}

void remove_id(bp_tree** root, int id) {
	int i;
	bp_tree* temp = NULL, *temp2 = NULL;
	for(i = 0; i <= (*root)->arr_node->data_index; i++) {
		if(id == (*root)->arr_node->data_arr[i]->id) {
			if((*root)->bp_tree_index == -1) {
				if((*root)->arr_node->data_index > 0) {
					free((*root)->arr_node->data_arr[i]);
					if(i < (*root)->arr_node->data_index) {
						shift_left_data_arr(root, i);
					}
					shrink_data_array(root, 1);
				}
				else if((*root)->arr_node->data_index == 0) {
					free((*root)->arr_node->data_arr[i]);
					free((*root)->arr_node->data_arr);
					free((*root)->arr_node);
					(*root)->arr_node = NULL;
				}
			}
			else {
				remove_id(&(*root)->bp_tree_arr[i+1], id);
				if((*root)->bp_tree_arr[i+1]->arr_node != NULL) {
					if(id == (*root)->arr_node->data_arr[i]->id) {
						if((*root)->arr_node->data_index == 0) {
							temp = get_smallest_node((*root)->bp_tree_arr[i+1]);
							(*root)->arr_node->data_arr[i]->id = temp->arr_node->data_arr[0]->id;
							temp2 = get_greatest_node((*root)->bp_tree_arr[i]);
							temp2->arr_node->next = temp->arr_node;
							check_height(root);
						}
						else {
							temp = get_greatest_node((*root)->bp_tree_arr[i]);
							temp2 = (*root)->bp_tree_arr[i+1];
							if((temp->arr_node->data_index + temp2->arr_node->data_index+1) < degree-1) {
								grow_data_array(&temp, temp2->arr_node->data_index+1);
								copy_data_arr(&temp, temp->arr_node->data_index,
											   temp->arr_node->data_index, &temp2);
								temp->arr_node->next = temp2->arr_node->next;
								temp2->arr_node->next = NULL;
								free_bp_tree(&temp2);
								shift_left_bp_tree_arr(root, i+1, i+1);
								shrink_bp_tree_arr(root, 1);
								free((*root)->arr_node->data_arr[i]);
								shift_left_data_arr(root, i);
								shrink_data_array(root, 1);
							}
						}
					}
				}
				else {
					free((*root)->bp_tree_arr[i+1]);
					if(i+1 < (*root)->bp_tree_index) {
						(*root)->bp_tree_arr[i]->arr_node->next = (*root)->bp_tree_arr[i+2]->arr_node;
						shift_left_bp_tree_arr(root, i+1, i+1);
					}
					else if(i > 0) {
						(*root)->bp_tree_arr[i]->arr_node->next = NULL;
						(*root)->bp_tree_arr[i-1]->arr_node->next = (*root)->bp_tree_arr[i]->arr_node;       //useless line
					}
					else if(i == 0)
						(*root)->bp_tree_arr[i]->arr_node->next = NULL;

					shrink_bp_tree_arr(root, 1);
					if((*root)->arr_node->data_index > 0) {
						free((*root)->arr_node->data_arr[i]);
						if(i < (*root)->arr_node->data_index)
							shift_left_data_arr(root, i);
						shrink_data_array(root, 1);
					}
					else {
						temp = (*root)->bp_tree_arr[i];
						(*root)->bp_tree_arr[i] = NULL;
						free_bp_tree(root);
						*root = temp;
					}
				}
			}
			return;
		}
		else if(id < (*root)->arr_node->data_arr[i]->id && (*root)->bp_tree_arr != NULL) {
			remove_id(&(*root)->bp_tree_arr[i], id);
			if((*root)->bp_tree_arr[i]->arr_node == NULL) {
				free((*root)->bp_tree_arr[i]);
				shift_left_bp_tree_arr(root, i, i+1);
				if(i > 0 && i < (*root)->bp_tree_index)
					(*root)->bp_tree_arr[i-1]->arr_node->next = (*root)->bp_tree_arr[i]->arr_node;
				shrink_bp_tree_arr(root, 1);
				if((*root)->arr_node->data_index > 0) {
					free((*root)->arr_node->data_arr[i]);
					shift_left_data_arr(root, i);
					shrink_data_array(root, 1);
				}
				else if((*root)->arr_node->data_index == 0) {
					temp = (*root)->bp_tree_arr[i];
					free_bp_tree(root);
					*root = temp;
				}
			}
			else if((*root)->bp_tree_arr[i]->bp_tree_height < (*root)->bp_tree_arr[i+1]->bp_tree_height) {
				if((*root)->arr_node->data_index == 0) {
					temp = (*root)->bp_tree_arr[i+1];
					(*root)->bp_tree_arr[i+1] = NULL;
					(*root)->bp_tree_height = temp->bp_tree_height;
					grow_data_array(root, temp->arr_node->data_index+1);
					copy_data_arr(root, (*root)->arr_node->data_index - temp->arr_node->data_index, (*root)->arr_node->data_index, &temp);
					grow_bp_tree_array(root, temp->bp_tree_index);
					copy_bp_tree_array(root, (*root)->bp_tree_index - temp->bp_tree_index, (*root)->bp_tree_index, &temp);
					free_bp_tree(&temp);
				}
				else {
					if((*root)->bp_tree_arr[i]->arr_node->next->data_index < degree-1) {
						grow_data_array2(&(*root)->bp_tree_arr[i]->arr_node->next->data_arr,
										 &(*root)->bp_tree_arr[i]->arr_node->next->data_index, 1);

						shift_right_data_arr2(&(*root)->bp_tree_arr[i]->arr_node->next->data_arr,
											   (*root)->bp_tree_arr[i]->arr_node->next->data_index, 0);
						(*root)->bp_tree_arr[i]->arr_node->next->data_arr[0] = (*root)->bp_tree_arr[i]->arr_node->data_arr[0];
						(*root)->bp_tree_arr[i]->arr_node->data_arr[0] = NULL;
						free_bp_tree((*root)->bp_tree_arr);
						shift_left_bp_tree_arr(root, 0, 1);
						shrink_bp_tree_arr(root, 1);
						free((*root)->arr_node->data_arr[i]);
						shift_left_data_arr(root, 0);
						shrink_data_array(root, 1);
					}
				}
				if((*root)->arr_node->data_index == degree-1) {
					split_bp_tree(&temp, root);
				}
			}
			return;
		}
		else if(i == (*root)->arr_node->data_index) {
			if(i+1 <= (*root)->bp_tree_index) {
				int index = -1;
				remove_id(&(*root)->bp_tree_arr[i+1], id);
				if((*root)->bp_tree_arr[i]->bp_tree_height > (*root)->bp_tree_arr[i+1]->bp_tree_height) {
					if((*root)->arr_node->data_index == 0) {
						index = (*root)->bp_tree_arr[i]->arr_node->data_index;
						grow_data_array2(&(*root)->arr_node->data_arr,
										 &(*root)->arr_node->data_index,
										 index+1);

						shift_right_data_arr2(&(*root)->arr_node->data_arr,
											   (*root)->arr_node->data_index, index);

						copy_data_arr2(&(*root)->arr_node->data_arr, 0,
										(*root)->arr_node->data_index,
										&(*root)->bp_tree_arr[i]->arr_node->data_arr);

						grow_bp_tree_array(root, (*root)->bp_tree_arr[i]->bp_tree_index);
						shift_right_bp_tree_arr(root, i+1);
						(*root)->bp_tree_height = (*root)->bp_tree_arr[i]->bp_tree_height;
						temp = (*root)->bp_tree_arr[i];
						copy_bp_tree_array(root, i, (*root)->bp_tree_index, &temp);
						free_bp_tree(&temp);
					}
					else {
						if(i > 0) {
//                            bp_tree* temp2 = (*root);
							temp = (*root)->bp_tree_arr[i];
							while(temp->bp_tree_height != 0) {
//								temp2 = temp;
								temp = temp->bp_tree_arr[temp->bp_tree_index];
							}

							if( temp->arr_node->data_index < degree-1) {
								index = temp->arr_node->data_index + 1;
								grow_data_array2(&temp->arr_node->data_arr,
												 &temp->arr_node->data_index,
												 (*root)->bp_tree_arr[i+1]->arr_node->data_index+1);
								copy_data_arr2(&temp->arr_node->data_arr, index,
												temp->arr_node->data_index,
												&(*root)->bp_tree_arr[i+1]->arr_node->data_arr);
								temp->arr_node->next = (*root)->bp_tree_arr[i+1]->arr_node->next;     //useless
								free_bp_tree(&(*root)->bp_tree_arr[i+1]);
								if(i+1 < (*root)->bp_tree_index)
									shift_left_bp_tree_arr(root, i+1, i+2);
								shrink_bp_tree_arr(root, 1);
								if((*root)->arr_node->data_index > i) {
									free((*root)->arr_node->data_arr[i]);
									shift_left_data_arr2(&(*root)->arr_node->data_arr, i, (*root)->arr_node->data_index);
								}
								else {
									free((*root)->arr_node->data_arr[(*root)->arr_node->data_index]);
									shrink_data_array(root, 1);
								}

							}
						}
					}
				}
			}
			else {
                printf("\n%d node not exist.\n", id);
            }
			return;
		}
	}
}

void delete_id(bp_tree** root, int id) {
	if(*root != NULL) {
		remove_id(root, id);
		if((*root)->arr_node == NULL) {
			free(*root);
			*root = NULL;
		}
	}
	else
		printf("tree empty.\n");
}

void print_id(bp_tree* temp) {
	node* n_temp = NULL;
	int i;
	if(temp != NULL) {
		while(temp->bp_tree_index != -1)
			temp = temp->bp_tree_arr[0];
		n_temp = temp->arr_node;
		 do {
			for(i = 0; i <= n_temp->data_index; i++)
				printf("%d ", n_temp->data_arr[i]->id);
			n_temp = n_temp->next;
		}while(n_temp != NULL);
	}
	else
		printf("tree empty.\n");
}
/*
					   | 20  |
					 /        \
				   /            \
	  | 10 | 15 |    		        | 27 | 29 |
	 /     \     \                /       \      \
	/       \     \             /          \      \
|4|5|-->|10|12|-->|15|16|-->|20|26|-->|27|28|-->|29|30|

*/

bp_tree* dummy_tree(void) {
	bp_tree* root = NULL, *root2 = NULL, *main = NULL;

	create_bp_tree(&main);
	create_node(&main);
	grow_data_array(&main, 1);
	create_data(&main, 0, 20);
	grow_bp_tree_array(&main, 2);


	create_bp_tree(&root);
	create_node(&root);

	grow_data_array(&root, 2);
	create_data(&root, 0, 10);
	create_data(&root, 1, 15);

	grow_bp_tree_array(&root, 3);

	create_bp_tree(&root->bp_tree_arr[0]);
	create_node(&root->bp_tree_arr[0]);
	grow_data_array(&root->bp_tree_arr[0], 2);
	create_data(&root->bp_tree_arr[0], 0, 4);
	create_data(&root->bp_tree_arr[0], 1, 5);

	create_bp_tree(&root->bp_tree_arr[1]);
	create_node(&root->bp_tree_arr[1]);
	grow_data_array(&root->bp_tree_arr[1], 2);
	create_data(&root->bp_tree_arr[1], 0, 10);
	create_data(&root->bp_tree_arr[1], 1, 12);

	create_bp_tree(&root->bp_tree_arr[2]);
	create_node(&root->bp_tree_arr[2]);
	grow_data_array(&root->bp_tree_arr[2], 2);
	create_data(&root->bp_tree_arr[2], 0, 15);
	create_data(&root->bp_tree_arr[2], 1, 16);

	root->bp_tree_arr[0]->arr_node->next = root->bp_tree_arr[1]->arr_node;
	root->bp_tree_arr[1]->arr_node->next = root->bp_tree_arr[2]->arr_node;

	create_bp_tree(&root2);
	create_node(&root2);
	grow_data_array(&root2, 2);
	create_data(&root2, 0, 27);
	create_data(&root2, 1, 29);

	grow_bp_tree_array(&root2, 3);

	create_bp_tree(&root2->bp_tree_arr[0]);
	create_node(&root2->bp_tree_arr[0]);
	grow_data_array(&root2->bp_tree_arr[0], 2);
	create_data(&root2->bp_tree_arr[0], 0, 20);
	create_data(&root2->bp_tree_arr[0], 1, 26);

	create_bp_tree(&root2->bp_tree_arr[1]);
	create_node(&root2->bp_tree_arr[1]);
	grow_data_array(&root2->bp_tree_arr[1], 2);
	create_data(&root2->bp_tree_arr[1], 0, 27);
	create_data(&root2->bp_tree_arr[1], 1, 28);

	create_bp_tree(&root2->bp_tree_arr[2]);
	create_node(&root2->bp_tree_arr[2]);
	grow_data_array(&root2->bp_tree_arr[2], 2);
	create_data(&root2->bp_tree_arr[2], 0, 29);
	create_data(&root2->bp_tree_arr[2], 1, 30);

	root2->bp_tree_arr[0]->arr_node->next = root2->bp_tree_arr[1]->arr_node;
	root2->bp_tree_arr[1]->arr_node->next = root2->bp_tree_arr[2]->arr_node;

	root->bp_tree_arr[2]->arr_node->next = root2->bp_tree_arr[0]->arr_node;
	main->bp_tree_arr[0] = root;
	main->bp_tree_arr[1] = root2;
	return main;
}

int main() {
	int id[] = {5, 10, 15, 25, 35, 40, 45, 50, 60};
	int id2[] = {35, 15, 5, 50, 29, 30};
	bp_tree* root = NULL;
	int i, size = sizeof(id) / sizeof(*id);
	degree = 3;

	for(i = 0; i < size; i++)
		insert(&root, id[i]);

//	root = dummy_tree();

	for(i = 0; i < size; i++) {
		print_id(root);
		delete_id(&root, id[size-1-i]);
		printf("\n");
	}

	getch();
	return 0;
}