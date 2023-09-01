// program create a b-tree (balancing tree)
//Date 19-Dec-2022
//23-January-2023(insertion complited with my logic)

#include <stdio.h>
#include <stdlib.h>

int treeDegree = -1, maxValue = -1;

typedef struct node node;
typedef struct btNode btNode;

struct node
{
	int data;
	int middleLR;
	btNode *left;
	btNode *right;
	int lHeight;
	int rHeight;
};

struct btNode
{
	node **vArray;
	int vIndex;
	int split;
};

btNode *insertBtNode(btNode *, int);

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

void splitNodes(btNode **root)
{
	int sIndex, i, iCount = 0;
	btNode *btTemp1 = NULL;

	if (*root == NULL)
	{
		return;
	}
	else
	{
		sIndex = (*root)->vIndex / 2;

		//left
		btTemp1 = createBtNode();
		btTemp1->vIndex = sIndex - 1;
    	growNodeArray(&btTemp1->vArray, btTemp1->vIndex);
		for (i = 0; i <= btTemp1->vIndex; i++)
		{
			*(btTemp1->vArray + i) = *(((*root)->vArray) + i);
			*(((*root)->vArray + i)) = NULL;
		}
		*((*root)->vArray + 0) = *((*root)->vArray + (sIndex));
		*((*root)->vArray + (sIndex)) = NULL;

		if ((*((*root)->vArray + 0))->left == NULL)
		{
			(*((*root)->vArray + 0))->lHeight++;
			(*((*root)->vArray + 0))->left = btTemp1;
		}
		else
		{
			if ((*(btTemp1->vArray + btTemp1->vIndex))->middleLR == 2)
			{
				(*(btTemp1->vArray + btTemp1->vIndex))->middleLR = 0;
			}

			if ((*((*root)->vArray + 0))->middleLR == 1 || (*((*root)->vArray + 0))->middleLR == 3)
			{
				(*((*root)->vArray + 0))->middleLR -= 1;
			}

			(*((*root)->vArray + 0))->lHeight = (*(btTemp1->vArray + 0))->lHeight + 1;
			(*((*root)->vArray + 0))->left = btTemp1;
		}

		//right
		btTemp1 = createBtNode();
		btTemp1->vIndex = (*root)->vIndex - (sIndex + 1);
		growNodeArray(&btTemp1->vArray, btTemp1->vIndex);
		for (i = 0; i <= btTemp1->vIndex; i++)
		{
			*(btTemp1->vArray + i) = *((*root)->vArray + (sIndex + (i + 1)));
			*((*root)->vArray + (sIndex + (i + 1))) = NULL;
		}

		if ((*((*root)->vArray + 0))->right == NULL)
		{
			(*((*root)->vArray + 0))->rHeight++;
			(*((*root)->vArray + 0))->right = btTemp1;
		}
		else
		{
			if ((*((*root)->vArray + 0))->middleLR == 2)
			{
				(*((*root)->vArray + 0))->middleLR = 0;
			}
			if ((*(btTemp1->vArray + 0))->middleLR == 1 || (*(btTemp1->vArray + 0))->middleLR == 3)
			{
				(*(btTemp1->vArray + 0))->middleLR -= 1;
			}

			(*((*root)->vArray + (sIndex - 1)))->rHeight = (*(btTemp1->vArray + 0))->rHeight + 1;
			(*((*root)->vArray + (sIndex - 1)))->right = btTemp1;
		}

		for (i = 0; i <= (*root)->vIndex; i++)
		{
			if (*((*root)->vArray + i) == NULL)
				iCount++;
		}

		(*root)->vIndex -= iCount;
		(*root)->vArray = (node **)realloc((*root)->vArray, (sizeof(node *) * ((*root)->vIndex + 1)));
		(*root)->split = 1;
	}
}

btNode *insertBtNode(btNode *root, int data)
{
	int i, j;
	btNode *temp = NULL;
	if (root == NULL)
	{
		root = createBtNode();
		growNodeArray(&root->vArray, ++root->vIndex);
		*(root->vArray + root->vIndex) = createNode(data);
	}
	else
	{
		for (i = 0; i <= root->vIndex; i++)
		{
			if (data < (*(root->vArray + i))->data)
				break;
		}

		if (i <= root->vIndex)
		{
			if ((*(root->vArray + i))->left == NULL)
			{
				growNodeArray(&root->vArray, ++root->vIndex);
				for (j = root->vIndex; j > i; j--)
				{
					*(root->vArray + j) = *(root->vArray + (j - 1));
				}
				*(root->vArray + i) = createNode(data);
			}
			else
			{
				temp = insertBtNode((*(root->vArray + i))->left, data);
				if (temp->split == 1)
				{
					growNodeArray(&root->vArray, ++root->vIndex);
					for (j = root->vIndex; j > i; j--)
					{
						*(root->vArray + j) = *(root->vArray + (j - 1));
					}
					*(root->vArray + i) = *(temp->vArray + temp->vIndex);
					free(temp->vArray);
					free(temp);
					(*(root->vArray + (i + 1)))->left = (*(root->vArray + i))->right;
					(*(root->vArray + (i + 1)))->middleLR = 1;
					for (j = i - 1; j >= 0; j--)
					{
						if ((*(root->vArray + j))->middleLR > 0)
						{
							if ((*(root->vArray + j))->middleLR == 3)
							{
								(*(root->vArray + j))->left = (*(root->vArray + (j - 1)))->right;
								(*(root->vArray + j))->right = (*(root->vArray + (j + 1)))->left;
							}
							else if ((*(root->vArray + j))->middleLR == 2)
							{
								(*(root->vArray + j))->right = (*(root->vArray + (j + 1)))->left;
							}
							else if ((*(root->vArray + j))->middleLR == 1)
							{
								(*(root->vArray + j))->right = (*(root->vArray + (j + 1)))->left;
							}
						}
					}
				}
				else
				{
					(*(root->vArray + i))->left = temp;
				}
			}
		}
		else
		{
			if ((*(root->vArray + (i - 1)))->right == NULL)
			{
            	growNodeArray(&root->vArray, ++root->vIndex);
				*(root->vArray + root->vIndex) = createNode(data);
			}
			else
			{
				temp = insertBtNode((*(root->vArray + (i - 1)))->right, data);
				if (temp->split == 1)
				{
                	growNodeArray(&root->vArray, ++root->vIndex);
					*(root->vArray + root->vIndex) = *(temp->vArray + temp->vIndex);
					free(temp->vArray);
					free(temp);
					(*(root->vArray + (root->vIndex - 1)))->right = (*(root->vArray + root->vIndex))->left;
					if ((*(root->vArray + (root->vIndex - 1)))->middleLR == 0)
					{
						(*(root->vArray + (root->vIndex - 1)))->middleLR = 2;
					}
					else if ((*(root->vArray + (root->vIndex - 1)))->middleLR == 1)
					{
						(*(root->vArray + (root->vIndex - 1)))->middleLR = 3;
					}
				}
				else
				{
					(*(root->vArray + (i - 1)))->right = temp;
				}
			}
		}
		if (root->vIndex == maxValue)
		{
			splitNodes(&root);
		}
	}
	return root;
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

void balanceRight(btNode** root, int i) {
	if((*root)->vIndex == 0) {
		if((*root)->vArray[i]->left != NULL) {
			if((*root)->vArray[i]->left->vIndex == 0 && (*root)->vArray[i]->right == NULL) {
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				shiftRight(root, i);
				(*root)->vArray[i] = (*root)->vArray[i+1]->left->vArray[0];
				free((*root)->vArray[i+1]->left->vArray);
				free((*root)->vArray[i+1]->left);
				(*root)->vArray[i+1]->left = NULL;
				(*root)->vArray[i+1]->lHeight = 0;
			}
			else if((*root)->vArray[i]->left->vIndex < maxValue-1) {
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				shiftRight(root, i);
				(*root)->vArray[i] = (*root)->vArray[i+1]->left->vArray[0];
				(*root)->vArray[i+1]->left = (*root)->vArray[i]->right;
				if((*root)->vArray[i+1]->middleLR == 0 || (*root)->vArray[i+1]->middleLR == 2)
					(*root)->vArray[i+1]->middleLR+=1;
				(*root)->vArray[i+1]->lHeight = (*root)->vArray[i+1]->left->vArray[0]->lHeight + 1;
			}
		}
	}
	else {
		if((*root)->vArray[i+1]->right != NULL) {
			if((*root)->vArray[i+1]->right->vIndex < maxValue-1) {
				growNodeArray(&(*root)->vArray[i+1]->right->vArray, ++(*root)->vArray[i+1]->right->vIndex);
				shiftRight(&(*root)->vArray[i+1]->right, (*root)->vArray[i+1]->right->vIndex-1);
			}
		}
    }
}

void balanceLeft(btNode** root, int i) {
	if((*root)->vIndex == 0) {
		if((*root)->vArray[i]->right != NULL) {
			if((*root)->vArray[i]->left == NULL && (*root)->vArray[i]->right->vIndex == 0) {
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				(*root)->vArray[(*root)->vIndex] = (*root)->vArray[i]->right->vArray[0];
				free((*root)->vArray[i]->right->vArray);
				free((*root)->vArray[i]->right);
				(*root)->vArray[i]->right = NULL;
				(*root)->vArray[i]->rHeight = 0;
			}
			else if((*root)->vArray[i]->left != NULL && (*root)->vIndex < maxValue-1 && (*root)->vArray[i]->right->vIndex < maxValue-1) {
				growNodeArray(&(*root)->vArray, ++(*root)->vIndex);
				(*root)->vArray[(*root)->vIndex] = (*root)->vArray[i]->right->vArray[0];
				(*root)->vArray[i]->right = (*root)->vArray[i+1]->left;
				if((*root)->vArray[i]->middleLR == 0 || (*root)->vArray[i]->middleLR == 1)
					(*root)->vArray[i]->middleLR+=2;
				(*root)->vArray[i]->rHeight = (*root)->vArray[i]->right->vArray[0]->rHeight + 1;
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
	for(i = 0; i <= (*root)->vIndex; i++) {
		if(data == (*root)->vArray[i]->data) {
			if((*root)->vArray[i]->left != NULL) {
				findMaxNode(&(*root)->vArray[i]->left, &(*root)->vArray[i]->data);
				updateLeftHeight(root, i);
			}
			break;
		}
		else if(data < (*root)->vArray[i]->data) {
			deleteNode(&(*root)->vArray[i]->left, data);
			updateLeftHeight(root, i);
			break;
		}
		else if(data > (*root)->vArray[i]->data){
        	deleteNode(&(*root)->vArray[i]->right, data);
			updateRightHeight(root, i);
			break;
		}
	}
}

int main()
{
	btNode* root = NULL;
	treeDegree =  3;
	maxValue = treeDegree - 1;

	root = insertBtNode(NULL, 20);
	root = insertBtNode(root, 40);
	root = insertBtNode(root, 10);
	root = insertBtNode(root, 30);
	root = insertBtNode(root, 33);
	root = insertBtNode(root, 50);
	root = insertBtNode(root, 60);
	root = insertBtNode(root, 5);
	root = insertBtNode(root, 15);
	root = insertBtNode(root, 25);
	root = insertBtNode(root, 28);
	root = insertBtNode(root, 31);
	root = insertBtNode(root, 35);
	root = insertBtNode(root, 45);
	root = insertBtNode(root, 55);
	root = insertBtNode(root, 65);

	deleteNode(&root, 33);
	deleteNode(&root, 20);
	deleteNode(&root, 40);

	return 0;
}
