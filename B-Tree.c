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

node** growNodeArray(node **vArray, int size) {
	vArray = (node**)realloc(vArray, (size + 1) * sizeof(node *));
	if (!vArray)
		printf("Node pointer memory not allocated.\n");
	else
		vArray[size] = NULL;
	return vArray;
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
		btTemp1->vArray = growNodeArray(btTemp1->vArray, btTemp1->vIndex);
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
		btTemp1->vArray = growNodeArray(btTemp1->vArray, btTemp1->vIndex);
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
		root->vArray = growNodeArray(root->vArray, ++root->vIndex);
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
				root->vArray = growNodeArray(root->vArray, ++root->vIndex);
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
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
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
				root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				*(root->vArray + root->vIndex) = createNode(data);
			}
			else
			{
				temp = insertBtNode((*(root->vArray + (i - 1)))->right, data);
				if (temp->split == 1)
				{
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
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

node** shrinkNodeArray(node **vArray, int size) {
	if(size < 0) {
		free(vArray);
		vArray = NULL;
	}
	else {
		vArray = (node**)realloc(vArray, (size+1) * sizeof(node**));
		if (!vArray)
			printf("vArray shrink null.\n");
    }
	return vArray;
}

void shiftLeft(btNode** root, int i) {
	while(i < (*root)->vIndex) {
		(*root)->vArray[i] = (*root)->vArray[i+1];
		i++;
	}
}

void shiftRight(btNode** root, int i) {
	while(i) {
		(*root)->vArray[i] = (*root)->vArray[i-1];
		i--;
	}
}

void deleteShiftLeft(btNode** root, int delIndex) {
	int i;
	free((*root)->vArray[delIndex]);
	(*root)->vArray[delIndex] = NULL;
    shiftLeft(root, delIndex);
	(*root)->vArray = shrinkNodeArray((*root)->vArray, --(*root)->vIndex);
}

void deleteNode(btNode** root, int delIndex) {
	free((*root)->vArray[delIndex]);
	(*root)->vArray = shrinkNodeArray((*root)->vArray, --(*root)->vIndex);
}

void vArrayNodeMove(btNode** src, int src_index, btNode** dst, int dst_index) {
	(*dst)->vArray[dst_index] = (*src)->vArray[src_index];
	shiftLeft(src, src_index);
	(*src)->vArray = shrinkNodeArray((*src)->vArray, --(*src)->vIndex);
}

void balanceLeft(btNode** root, int i) {
	if((*root)->vArray[i]->right->vIndex > 0) {
		(*root)->vArray[i]->left->vArray = growNodeArray((*root)->vArray[i]->left->vArray, ++(*root)->vArray[i]->left->vIndex);
		(*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex] = createNode((*root)->vArray[i]->data);
		(*root)->vArray[i]->data = (*root)->vArray[i]->right->vArray[0]->data;
		free((*root)->vArray[i]->right->vArray[0]);
		(*root)->vArray[i]->right->vArray[0] = NULL;
		shiftLeft(&(*root)->vArray[i]->right, 0);
		(*root)->vArray[i]->right->vArray = shrinkNodeArray((*root)->vArray[i]->right->vArray, --(*root)->vArray[i]->right->vIndex);
	}
	else {
		if(i < (*root)->vIndex) {
			(*root)->vArray[i]->right->vArray = growNodeArray((*root)->vArray[i]->right->vArray, ++(*root)->vArray[i]->right->vIndex);
			shiftRight(&(*root)->vArray[i]->right, (*root)->vArray[i]->right->vIndex);
			(*root)->vArray[i]->right->vArray[0] = createNode((*root)->vArray[i]->data);
			free((*root)->vArray[i]);
			if((*root)->vArray[i+1]->middleLR == 1 || (*root)->vArray[i+1]->middleLR == 3)
				(*root)->vArray[i+1]->middleLR-=1;
			shiftLeft(root, i);
			(*root)->vArray = shrinkNodeArray((*root)->vArray, --(*root)->vIndex);
		}
		else {
			(*root)->vArray = growNodeArray((*root)->vArray, ++(*root)->vIndex);
			(*root)->vArray[(*root)->vIndex] = (*root)->vArray[i]->right->vArray[0];
			(*root)->vArray[i]->right->vArray[0] = NULL;
			(*root)->vArray[i]->rHeight = 0;
			free((*root)->vArray[i]->right);
			(*root)->vArray[i]->right = NULL;
			free((*root)->vArray[i]->left);
			(*root)->vArray[i]->left = NULL;
			(*root)->vArray[i]->lHeight = 0;
		}
	}
}

void balanceLeftRight(btNode** root, int i) {
	if((*root)->vArray[i]->right->vIndex > 0) {
		(*root)->vArray[i]->left->vArray = growNodeArray((*root)->vArray[i]->left->vArray, ++(*root)->vArray[i]->left->vIndex);
		(*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex] = createNode((*root)->vArray[i]->data);
		(*root)->vArray[i]->data = (*root)->vArray[i]->right->vArray[0]->data;
		shiftLeft(&(*root)->vArray[i]->right, 0);
        (*root)->vArray[i]->right->vArray = shrinkNodeArray((*root)->vArray[i]->right->vArray, --(*root)->vArray[i]->right->vIndex);
	}
	else if((*root)->vArray[i-1]->left->vIndex > 0) {
		(*root)->vArray[i]->left->vArray = growNodeArray((*root)->vArray[i]->left->vArray, ++(*root)->vArray[i]->left->vIndex);
		(*root)->vArray[i]->left->vArray[(*root)->vArray[i]->left->vIndex] = createNode((*root)->vArray[i-1]->data);
		(*root)->vArray[i-1]->data = (*root)->vArray[i-1]->left->vArray[(*root)->vArray[i-1]->left->vIndex]->data;
		(*root)->vArray[i-1]->left->vArray = shrinkNodeArray((*root)->vArray[i-1]->left->vArray, --(*root)->vArray[i-1]->left->vIndex);
	}
	else {

	}
}

void balance(btNode** root, int i) {
	if(i == 0) {
		balanceLeft(root, i);
	}
	else {
		balanceLeftRight(root, i);
	}
}

void findMaxNode(btNode** root, int* data) {
	if((*root)->vArray[(*root)->vIndex]->right == NULL) {
		*data = (*root)->vArray[(*root)->vIndex]->data;
		free((*root)->vArray[(*root)->vIndex]);
        (*root)->vArray = shrinkNodeArray((*root)->vArray, --(*root)->vIndex);
	}
	else findMaxNode(root, data);
}

void findMinNode()

void deleteBtNode(btNode** root, int data) {
	int i;
	for(i = 0; i <= (*root)->vIndex; i++) {
		if(data < (*root)->vArray[i]->data) {
			deleteBtNode(&(*root)->vArray[i]->left, data);
			if((*root)->vArray[i]->left->vArray == NULL) {
				balance(root, i);
			}
			break;
		}
		else if(data > (*root)->vArray[i]->data && i == (*root)->vIndex) {
			deleteBtNode(&(*root)->vArray[i]->right, data);
			break;
		}
		else if(data == (*root)->vArray[i]->data){
			if((*root)->vArray[i]->left == NULL && (*root)->vArray[i]->right == NULL) {
				if(i == (*root)->vIndex) {
                	deleteNode(root, i);
				}
				else {
					deleteShiftLeft(root, i);
				}
			}
			else if((*root)->vArray[i]->left != NULL && (*root)->vArray[i]->left->vIndex > 0) {
				findMaxNode(&(*root)->vArray[i]->left, &(*root)->vArray[i]->data);
			}
			else if((*root)->vArray[i]->right != NULL && (*root)->vArray[i]->right->vIndex > 0) {
            	findMaxNode(&(*root)->vArray[i]->right, &(*root)->vArray[i]->data);
			}
            break;
		}
	}
}

void deleteBT(btNode** root, int data) {
	if(*root != NULL) {
        deleteBtNode(root, data);
		if((*root)->vArray == NULL) {
			free(*root);
			*root = NULL;
        }
	}

}
int main()
{
	btNode* root = createBtNode();
	root->vArray = growNodeArray(root->vArray, root->vIndex+=2);
	root->vArray[0] = createNode(3);
	root->vArray[0]->left = createBtNode();
	root->vArray[0]->left->vArray = growNodeArray(root->vArray[0]->left->vArray, root->vArray[0]->left->vIndex+=2);
	root->vArray[0]->left->vArray[0] = createNode(1);
	root->vArray[0]->left->vArray[1] = createNode(2);
	root->vArray[0]->lHeight = root->vArray[0]->left->vArray[0]->lHeight + 1;
	root->vArray[0]->right = createBtNode();
	root->vArray[0]->right->vArray = growNodeArray(root->vArray[0]->right->vArray, root->vArray[0]->right->vIndex+=2);
	root->vArray[0]->right->vArray[0] = createNode(4);
	root->vArray[0]->right->vArray[1] = createNode(5);
	root->vArray[0]->rHeight = root->vArray[0]->right->vArray[0]->rHeight + 1;
	root->vArray[1] = createNode(6);
	root->vArray[1]->left = root->vArray[0]->right;
	root->vArray[1]->lHeight = root->vArray[1]->left->vArray[0]->lHeight + 1;
	root->vArray[1]->middleLR = 1;
	root->vArray[1]->right = createBtNode();
	root->vArray[1]->right->vArray = growNodeArray(root->vArray[1]->right->vArray, root->vArray[1]->right->vIndex+=2);
	root->vArray[1]->right->vArray[0] = createNode(7);
	root->vArray[1]->right->vArray[1] = createNode(8);
	root->vArray[1]->rHeight = root->vArray[1]->right->vArray[0]->rHeight + 1;

	deleteBT(&root, 3);
	deleteBT(&root, 2);
	return 0;
}