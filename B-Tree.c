// program create a b-tree (balancing tree)

#include <stdio.h>
#include <stdlib.h>

int treeDegree = -1, maxValue = -1;


typedef struct btNode btNode;

typedef struct
{
	int data;
	int middleLR;
	btNode *left;
	btNode *right;
	int lHeight;
	int rHeight;
}node;

struct btNode
{
	node **vArray;
	int vIndex;
	int split;
};

btNode *insertBtNode(btNode *, int);
void rootMoveUp(btNode** root, int i, char LR);

node* createNode(int data)
{
	node *temp = (node *)malloc(sizeof(node));
	if (!temp)
		printf("Node memory not allocated.\n");
	else
	{
		temp->data = data;
		temp->left = NULL;
		temp->right = NULL;
		temp->middleLR = 0;
		temp->lHeight = 0;
		temp->rHeight = 0;
	}
	return temp;
}

btNode* createBtNode(void)
{
	btNode *root = NULL;
	root = (btNode *)malloc(sizeof(btNode));
	if (!root) {
		printf("btNode memory not allocated.\n");
	}
	else {
		root->vArray = NULL;
		root->vIndex = -1;
		root->split = 0;
	}
	return root;
}

void growNodeArray(node*** vArray, int size) {
	*vArray = (node**)realloc((*vArray), (size + 1) * sizeof(node *));
	if (!(*vArray))
		printf("Node pointer memory not allocated.\n");
	else
		(*vArray)[size] = NULL;
}

void shrinkNodeArray(node*** vArray, int size) {
	*vArray = (node**)realloc((*vArray), (size + 1) * sizeof(node *));
	if (!(*vArray))
		printf("Node pointer memory not allocated.\n");
}

void shiftLeft(btNode** root, int index) {
	int i;
	for(i = index; i < (*root)->vIndex; i++)
		(*root)->vArray[i] = (*root)->vArray[i+1];
    (*root)->vArray[i] = NULL;
}

void shiftRight(btNode** root, int index) {
	int i;
	for(i = (*root)->vIndex; i > index; i--)
		(*root)->vArray[i] = (*root)->vArray[i-1];
	(*root)->vArray[i] = NULL;
}

void addMiddleLR(int* middleLR, int value) {
	if(value == 1) {
		if(*middleLR == 0 || *middleLR == 2)
			*middleLR+=1;
    }
	else if(value == 2) {
		if(*middleLR == 0 || *middleLR == 1)
			*middleLR+=2;
    }
	else
		printf("addMiddleLR %d is invalid input.\n", value);
}

void subMiddleLR(int* middleLR, int value) {
	if(value == 1) {
		if(*middleLR == 1 || *middleLR == 3)
			*middleLR-=1;
	}
	else if(value == 2) {
		if(*middleLR == 2 || *middleLR == 3)
			*middleLR-=2;
    }
	else
		printf("subMiddleLR %d is invalid input.\n", value);
}

void heightUpdate(node* temp, int value) {
	if(value == 1)
		temp->lHeight = temp->left->vArray[0]->lHeight + 1;
	else if(value == 2)
		temp->rHeight = temp->right->vArray[0]->rHeight + 1;
	else
		printf("heightUpdate %d is invalid input.\n", value);
}

void balanceLeft(btNode** root, int i) {
	btNode* temp = NULL;
	if((*root)->vIndex == maxValue-1) {
		if((*root)->vArray[i]->left == NULL) {
			if(i == (*root)->vIndex) {
				if((*root)->vArray[i]->right->vIndex < maxValue-1) {
					temp = (*root)->vArray[i]->right;
					(*root)->vArray[i]->right = NULL;
					(*root)->vArray[i]->rHeight = 0;
					growNodeArray(&temp->vArray, ++temp->vIndex);
					shiftRight(&temp, 0);
					temp->vArray[0] = (*root)->vArray[i];
					(*root)->vArray[i] = NULL;
					shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
					(*root)->vArray[(*root)->vIndex]->right = temp;
					heightUpdate((*root)->vArray[(*root)->vIndex], 2);
					subMiddleLR(&(*root)->vArray[(*root)->vIndex]->middleLR, 2);
					temp = NULL;
				}
				else {
					temp = createBtNode();                                             //improvement possible
					growNodeArray(&temp->vArray, ++temp->vIndex);
					temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
					(*root)->vArray[i]->data = (*root)->vArray[i]->right->vArray[0]->data;
					free((*root)->vArray[i]->right->vArray[0]);
					shiftLeft(&(*root)->vArray[i]->right, 0);
					shrinkNodeArray(&(*root)->vArray[i]->right->vArray, --(*root)->vArray[i]->right->vIndex);
					(*root)->vArray[i]->left = temp;
					heightUpdate((*root)->vArray[i], 1);
					subMiddleLR(&(*root)->vArray[i]->middleLR, 1);
					(*root)->vArray[i-1]->right = temp;
					heightUpdate((*root)->vArray[i-1], 2);
					addMiddleLR(&(*root)->vArray[i-1]->middleLR, 2);
				}
			}
			else {
				if((*root)->vArray[i]->right->vIndex == maxValue-1) {
					btNode* temp = createBtNode();
					growNodeArray(&temp->vArray, ++temp->vIndex);
					temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
					(*root)->vArray[i]->data = (*root)->vArray[i]->right->vArray[0]->data;
					free((*root)->vArray[i]->right->vArray[0]);
					shiftLeft(&(*root)->vArray[i]->right, 0);
					shrinkNodeArray(&(*root)->vArray[i]->right->vArray, --(*root)->vArray[i]->right->vIndex);
					(*root)->vArray[i]->left = temp;
					heightUpdate((*root)->vArray[i], 1);
					subMiddleLR(&(*root)->vArray[i]->middleLR, 1);
				}
				else {
					(*root)->vArray[i]->right = NULL;
					(*root)->vArray[i]->rHeight = 0;
					(*root)->vArray[i]->middleLR = 0;
					growNodeArray(&(*root)->vArray[i+1]->left->vArray, ++(*root)->vArray[i+1]->left->vIndex);
					shiftRight(&(*root)->vArray[i+1]->left, 0);
					(*root)->vArray[i+1]->left->vArray[0] = (*root)->vArray[i];
					shiftLeft(root, i);
					shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
					subMiddleLR(&(*root)->vArray[i]->middleLR, 1);
				}
			}
		}
		else {
			if((*root)->vArray[i]->right->vIndex < maxValue-1) {
				temp = (*root)->vArray[i]->right;
				(*root)->vArray[i]->right = NULL;
				(*root)->vArray[i]->rHeight = 0;
				growNodeArray(&temp->vArray, ++temp->vIndex);
				shiftRight(&temp, 0);
				temp->vArray[0] = (*root)->vArray[i];
				temp->vArray[0]->right = temp->vArray[1]->left;
				heightUpdate(temp->vArray[0], 2);
				addMiddleLR(&temp->vArray[0]->middleLR, 2);
				(*root)->vArray[i-1]->right = temp;
				heightUpdate((*root)->vArray[i-1], 2);
				subMiddleLR(&(*root)->vArray[i-1]->middleLR, 2);
                shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
			}
		}
	}
	else {
		if((*root)->vArray[i]->left == NULL) {
			if((*root)->vArray[i]->right->vIndex > 0) {
				temp = createBtNode();
				growNodeArray(&temp->vArray, ++temp->vIndex);
				temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
				(*root)->vArray[i]->data = (*root)->vArray[i]->right->vArray[0]->data;
				free((*root)->vArray[i]->right->vArray[0]);
				shiftLeft(&(*root)->vArray[i]->right, 0);
				shrinkNodeArray(&(*root)->vArray[i]->right->vArray, --(*root)->vArray[i]->right->vIndex);
				(*root)->vArray[i]->left = temp;
				heightUpdate((*root)->vArray[i], 1);
			}
			else {
				temp = (*root)->vArray[i]->right;
				(*root)->vArray[i]->right = NULL;
				(*root)->vArray[i]->rHeight = 0;
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				(*root)->vArray[(*root)->vIndex] = temp->vArray[0];
				free(temp->vArray);
				free(temp);
				temp = NULL;
			}
		}
		else {
			if((*root)->vArray[i]->right->vIndex < maxValue-1) {
				temp = (*root)->vArray[i]->right;
				(*root)->vArray[i]->right = NULL;
				(*root)->vArray[i]->rHeight = 0;
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				(*root)->vArray[(*root)->vIndex] = temp->vArray[0];
				(*root)->vArray[i]->right = temp->vArray[0]->left;
				(*root)->vArray[i]->rHeight = temp->vArray[0]->lHeight;
				addMiddleLR(&(*root)->vArray[i]->middleLR, 2);
				free(temp->vArray);
				free(temp);
				temp = NULL;
			}
		}
	}
}

void balanceRight(btNode** root, int i) {
    btNode* temp = NULL;
	if((*root)->vIndex == maxValue-1) {
		if((*root)->vArray[i]->right == NULL) {
			if(i < (*root)->vIndex) {
				if((*root)->vArray[i]->left->vIndex < maxValue-1) {
					temp = (*root)->vArray[i]->left;
					(*root)->vArray[i]->left = NULL;
					(*root)->vArray[i]->lHeight = NULL;
					growNodeArray(&temp->vArray, ++temp->vIndex);
					temp->vArray[temp->vIndex] = (*root)->vArray[i];
					shiftLeft(root, i);
					shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
					(*root)->vArray[i]->left = temp;
					heightUpdate((*root)->vArray[i], 1);
					subMiddleLR(&(*root)->vArray[i]->middleLR, 1);
				}
				else {
					temp = createBtNode();
					growNodeArray(&temp->vArray, ++temp->vIndex);
					temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
					(*root)->vArray[i]->data = (*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]->data;
					free((*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]);
					shrinkNodeArray(&(*root)->vArray[i]->left->vArray, --(*root)->vArray[i]->left->vIndex);
					(*root)->vArray[i]->right = temp;
					heightUpdate((*root)->vArray[i], 2);
					subMiddleLR(&(*root)->vArray[i]->middleLR, 2);
					(*root)->vArray[i+1]->left = temp;
					heightUpdate((*root)->vArray[i+1], 1);
					addMiddleLR(&(*root)->vArray[i]->middleLR, 2);
				}
			}
			else {
				if((*root)->vArray[i]->left->vIndex == maxValue-1) {
					temp = createBtNode();
					growNodeArray(&temp->vArray, ++temp->vIndex);
					temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
					(*root)->vArray[i]->data = (*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]->data;
					free((*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]);
					shrinkNodeArray(&(*root)->vArray[i]->left->vArray, --(*root)->vArray[i]->left->vIndex);
					(*root)->vArray[i]->right = temp;
					heightUpdate((*root)->vArray[i], 2);
				}
				else {
					(*root)->vArray[i]->left = NULL;
					(*root)->vArray[i]->lHeight = 0;
					(*root)->vArray[i]->middleLR = 0;
					growNodeArray(&(*root)->vArray[i-1]->right->vArray, ++(*root)->vArray[i-1]->right->vIndex);
					(*root)->vArray[i-1]->right->vArray[(*root)->vArray[i-1]->right->vIndex] = (*root)->vArray[i];
					shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
					subMiddleLR(&(*root)->vArray[i-1]->middleLR, 2);
				}
			}
		}
		else {
			if((*root)->vArray[i]->left->vIndex < maxValue-1) {
				temp = (*root)->vArray[i]->left;
				(*root)->vArray[i]->left = NULL;
				(*root)->vArray[i]->lHeight = 0;
				growNodeArray(&temp->vArray, ++temp->vIndex);
				temp->vArray[temp->vIndex] = (*root)->vArray[i];
				temp->vArray[temp->vIndex]->left = temp->vArray[temp->vIndex-1]->right;
				heightUpdate(temp->vArray[temp->vIndex], 1);
				addMiddleLR(&temp->vArray[temp->vIndex]->middleLR, 1);
				subMiddleLR(&temp->vArray[temp->vIndex]->middleLR, 2);
				shiftLeft(root, i);
                shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
				(*root)->vArray[i]->left = temp;
				heightUpdate((*root)->vArray[i], 1);
				subMiddleLR(&(*root)->vArray[i]->middleLR, 1);
				temp = NULL;
			}
		}
	}
	else {
		if((*root)->vArray[i]->right == NULL) {
			if((*root)->vArray[i]->left->vIndex > 0) {
				temp = createBtNode();
				growNodeArray(&temp->vArray, ++temp->vIndex);
				temp->vArray[temp->vIndex] = createNode((*root)->vArray[i]->data);
				(*root)->vArray[i]->data = (*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]->data;
				free((*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex]);
				shrinkNodeArray(&(*root)->vArray[i]->left->vArray, --(*root)->vArray[i]->left->vIndex);
				(*root)->vArray[i]->right = temp;
				heightUpdate((*root)->vArray[i], 2);
			}
			else {
				temp = (*root)->vArray[i]->left;
				(*root)->vArray[i]->left = NULL;
				(*root)->vArray[i]->lHeight = 0;
                growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				shiftRight(root, 0);
				(*root)->vArray[0] = temp->vArray[0];
				free(temp->vArray);
				free(temp);
				temp = NULL;
			}
		}
		else {
			if((*root)->vArray[i]->left->vIndex < maxValue-1) {
				temp = (*root)->vArray[i]->left;
				(*root)->vArray[i]->left = NULL;
				(*root)->vArray[i]->lHeight = 0;
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				shiftRight(root, i);
				(*root)->vArray[i] = temp->vArray[0];
				(*root)->vArray[i+1]->left = (*root)->vArray[i]->right;
				(*root)->vArray[i+1]->lHeight = (*root)->vArray[i]->rHeight;
				addMiddleLR(&(*root)->vArray[i+1]->middleLR, 1);
				free(temp->vArray);
				free(temp);
				temp = NULL;
			}
		}
	}
}

void findMaxNode(btNode** root, int* data) {
	if((*root)->vArray[(*root)->vIndex]->right == NULL) {
		*data = (*root)->vArray[(*root)->vIndex]->data;
		free((*root)->vArray[(*root)->vIndex]);
		if((*root)->vIndex > 0) {
			shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
		}
		else {
			free((*root)->vArray);
			free(*root);
			*root = NULL;
		}
	}
	else {
		findMaxNode(&(*root)->vArray[(*root)->vIndex]->right, data);
		if((*root)->vArray[(*root)->vIndex]->right == NULL) {
			(*root)->vArray[(*root)->vIndex]->rHeight = 0;
			balanceRight(root, (*root)->vIndex);
		}
    }
}

void findMinNode(btNode** root, int* data) {
	if((*root)->vArray[0]->left == NULL) {
		*data = (*root)->vArray[0]->data;
		free((*root)->vArray[0]);
		if((*root)->vIndex > 0) {
			shiftLeft(root, 0);
			shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
		}
		else {
			free((*root)->vArray);
			free(*root);
			*root = NULL;
		}

	}
	else {
		findMinNode(&(*root)->vArray[0]->left, data);
		if((*root)->vArray[0]->left == NULL) {
			(*root)->vArray[0]->lHeight = 0;
			balanceLeft(root, 0);
		}

	}
}

void checkLRHeight(btNode** root, int i) {
	if((*root)->vArray[i]->lHeight < (*root)->vArray[i]->rHeight) {
		balanceLeft(root, i);
	}
	else if((*root)->vArray[i]->lHeight > (*root)->vArray[i]->rHeight) {
    	balanceRight(root, i);
	}
}

void updateLeftHeight(btNode** root, int i) {
	if((*root)->vArray[i]->left == NULL)
		(*root)->vArray[i]->lHeight = 0;
	else
		(*root)->vArray[i]->lHeight = (*root)->vArray[i]->left->vArray[0]->lHeight + 1;
	checkLRHeight(root, i);
}

void updateRightHeight(btNode** root, int i) {
	if((*root)->vArray[i]->right == NULL)
		(*root)->vArray[i]->rHeight = 0;
	else
		(*root)->vArray[i]->rHeight = (*root)->vArray[i]->right->vArray[0]->rHeight + 1;
	checkLRHeight(root, i);
}

void deleteNode(btNode** root, int data) {
	int i;
	if(*root != NULL) {
		for(i = 0; i <= (*root)->vIndex; i++) {
			if(data == (*root)->vArray[i]->data) {
				if((*root)->vArray[i]->right != NULL) {
					if((*root)->vArray[i]->lHeight == 1 && (*root)->vArray[i]->left->vIndex > 0){
						findMaxNode(&(*root)->vArray[i]->left, &(*root)->vArray[i]->data);
						updateLeftHeight(root, i);
					}
					else{
						findMinNode(&(*root)->vArray[i]->right, &(*root)->vArray[i]->data);
						updateRightHeight(root, i);
					}
				}
				else {
					free((*root)->vArray[i]);
					if((*root)->vIndex == 0){
						free((*root)->vArray);
						*root = NULL;
					}
					else {
						if(i < (*root)->vIndex)
							shiftLeft(root, i);
						shrinkNodeArray(&(*root)->vArray, --(*root)->vIndex);
					}
				}
				break;
			}
			else if(data < (*root)->vArray[i]->data) {
				deleteNode(&(*root)->vArray[i]->left, data);
				updateLeftHeight(root, i);
				break;
			}
			else if(data > (*root)->vArray[i]->data && (i == (*root)->vIndex)){
				deleteNode(&(*root)->vArray[i]->right, data);
				updateRightHeight(root, i);
				break;
			}
		}
	}
	else
		printf("%d node is not exist.\n", data);
}

void inorder(btNode* root) {
	if(root != NULL) {
		int i;
		for(i = 0; i <= root->vIndex; i++) {
			if(root->vArray[i]->left != NULL)
				inorder(root->vArray[i]->left);

			printf("%2d ", root->vArray[i]->data);

			if(root->vArray[i]->right != NULL)
            	inorder(root->vArray[i]->right);
		}
	}
}

void updateHeight(btNode** root, int i) {
	if((*root)->vArray[i]->left == NULL)
		(*root)->vArray[i]->lHeight = 0;
	else
		(*root)->vArray[i]->lHeight = (*root)->vArray[i]->left->vArray[0]->lHeight + 1;

	if((*root)->vArray[i]->right == NULL)
		(*root)->vArray[i]->rHeight = 0;
	else
		(*root)->vArray[i]->rHeight = (*root)->vArray[i]->right->vArray[0]->rHeight + 1;

	if((*root)->vArray[i]->lHeight != (*root)->vArray[i]->rHeight) {
		if((*root)->vArray[i]->lHeight < (*root)->vArray[i]->rHeight) {
			rootMoveUp(root, i, 'R');
		}
		else {
			rootMoveUp(root, i, 'L');
		}
	}
}

void resetMiddleLR(int *middleLR) {
	*middleLR = 0;
}

void splitNodes(btNode** root) {
	int split = (treeDegree & 1) ? treeDegree / 2 : (treeDegree / 2) - 1,
	rIndex = treeDegree - (split+1),
	i, deleteIndex = 0;

	btNode* temp = createBtNode(), *rTemp = createBtNode();

	growNodeArray(&temp->vArray, ++temp->vIndex);
	temp->vArray[temp->vIndex] = (*root)->vArray[split];
	resetMiddleLR(&temp->vArray[temp->vIndex]->middleLR);
	deleteIndex++;

	growNodeArray(&rTemp->vArray, rTemp->vIndex+=rIndex);
	for(i = 0; i <= rTemp->vIndex; i++) {
		rTemp->vArray[i] = (*root)->vArray[(split+1)+i];
		deleteIndex++;
	}
	subMiddleLR(&rTemp->vArray[0]->middleLR, 1);
	shrinkNodeArray(&(*root)->vArray, (*root)->vIndex-=deleteIndex);
	subMiddleLR(&(*root)->vArray[(*root)->vIndex]->middleLR, 2);
	temp->vArray[temp->vIndex]->left = *root;
	temp->vArray[temp->vIndex]->right = rTemp;
	updateHeight(&temp, temp->vIndex);

	*root = temp;
}

void rootMoveUp(btNode** root, int i, char LR) {
	btNode* temp = NULL;
	growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
	if(LR == 'L')
		shiftRight(root, i);
	if(LR == 'R') {
		temp = (*root)->vArray[i]->right;
		(*root)->vArray[i]->right = NULL;
		(*root)->vArray[i]->rHeight = 0;
		(*root)->vArray[i+1] = temp->vArray[temp->vIndex];
		(*root)->vArray[i]->right = temp->vArray[temp->vIndex]->left;
		addMiddleLR(&(*root)->vArray[i]->middleLR, 2);
		updateHeight(root, i);
	}
	else {
		temp = (*root)->vArray[i+1]->left;
		(*root)->vArray[i+1]->left = 0;
		(*root)->vArray[i+1]->lHeight = 0;
		(*root)->vArray[i] = temp->vArray[temp->vIndex];
		(*root)->vArray[i+1]->left = temp->vArray[temp->vIndex]->right;
		addMiddleLR(&(*root)->vArray[i+1]->middleLR, 1);
		updateHeight(root, i+1);
	}

	temp->vArray[temp->vIndex] = NULL;
	free(temp->vArray);
	free(temp);

	if((*root)->vIndex == maxValue) {
        splitNodes(root);
	}
}

void addNode(btNode** root, int data) {
	int i;
	growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
	for(i = 0; i < (*root)->vIndex; i++) {
		if(data < (*root)->vArray[i]->data) {
			shiftRight(root, i);
			break;
		}
	}
	(*root)->vArray[i] = createNode(data);
	if((*root)->vIndex+1 == treeDegree)
		splitNodes(root);
}

void insertNode(btNode** root, int data) {
	int i;
	if(*root == NULL) {
		*root = createBtNode();
		addNode(root, data);
		return;
	}
	if((*root)->vArray[0]->lHeight == 0)
		addNode(root, data);
	else {
		for(i = 0; i <= (*root)->vIndex; i++) {
			if(data < (*root)->vArray[i]->data) {
				insertNode(&(*root)->vArray[i]->left, data);
                updateHeight(root, i);
				break;
			}
		}
		if(i > (*root)->vIndex) {
			insertNode(&(*root)->vArray[(*root)->vIndex]->right, data);
			updateHeight(root, (*root)->vIndex);
		}
	}
}

int main()
{
	btNode* root = NULL;
	int insertData[] = {20, 40, 10, 30, 33, 50, 60, 5, 15, 25, 28, 31, 35, 45, 55, 65};
	int deleteData[] = {5, 40, 28, 30, 65, 35, 50, 33, 25, 45, 10, 20, 55, 15, 31, 60};
	int i,
		iSize = sizeof(insertData)/ sizeof(insertData[0]),
		dSize = sizeof(deleteData)/ sizeof(deleteData[0]);

	treeDegree =  4;
	maxValue = treeDegree - 1;

	for(i = 0; i < iSize; i++) {
		printf("Insert %2d\n", insertData[i]);
		insertNode(&root, insertData[i]);
//		inorder(root);
//		printf("\n");
	}

	for(i = 0; i < dSize; i++) {
		printf("Delete %2d\n", deleteData[i]);
		deleteNode(&root, deleteData[i]);
		inorder(root);
    	printf("\n");
	}

	system("PAUSE");
	return 0;
}