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
	if (!root)
	{
		printf("btNode memory not allocated.\n");
	}
	else
	{
		root->vArray = NULL;
		root->vIndex = -1;
		root->split = 0;
	}
	return root;
}

node** growNodeArray(node **vArray, int size)
{
	vArray = (node**)realloc(vArray, (size + 1) * sizeof(node *));
	if (!vArray)
	{
		printf("Node pointer memory not allocated.\n");
	}
	else
	{
		vArray[size] = NULL;
	}
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

node** shrinkNodeArray(node **vArray, int size)
{
	vArray = (node**)realloc(vArray, (size+1) * sizeof(node**));
	if (!vArray)
	{
		printf("vArray shrink null.\n");
	}
	return vArray;
}

void shiftRight(btNode** root, int index) {
	int i;
	for(i = (*root)->vIndex; i > index; i--) {
		(*root)->vArray[i] = (*root)->vArray[i-1];
	}
	(*root)->vArray[i] = NULL;
}

void shiftLeft(btNode** root, int index) {
	int i;
	for(i = index; i < (*root)->vIndex; i++) {
		(*root)->vArray[i] = (*root)->vArray[i+1];
	}
	(*root)->vArray[i] = NULL;
}

btNode* addNodeLeft(btNode* root, btNode* temp) {
  root->vArray[root->vIndex]->right = NULL;
  root->vArray[root->vIndex]->rHeight = 0;
  root->vArray = growNodeArray(root->vArray, ++root->vIndex);
  root->vArray[root->vIndex] = temp->vArray[temp->vIndex];
  if(root->vIndex > 0 && root->vArray[root->vIndex]->left != NULL) {
    root->vArray[root->vIndex-1]->right = root->vArray[root->vIndex]->left;
	root->vArray[root->vIndex-1]->rHeight = root->vArray[root->vIndex]->lHeight;
	if(root->vArray[root->vIndex-1]->middleLR == 0 || root->vArray[root->vIndex-1]->middleLR == 1)
	  root->vArray[root->vIndex-1]->middleLR+=2;
  }
  temp->vArray[temp->vIndex] = NULL;
  free(temp->vArray[temp->vIndex]);
  free(temp->vArray);
  free(temp);
  return root;
}

btNode* addNodeRight(btNode* root, btNode* temp) {
  root->vArray = growNodeArray(root->vArray, ++root->vIndex);
  temp->vArray[temp->vIndex]->left = NULL;
  temp->vArray[temp->vIndex]->lHeight = 0;
  root->vArray[root->vIndex] = temp->vArray[temp->vIndex];
  if(root->vIndex > 0 && root->vArray[root->vIndex-1]->right != NULL) {
    root->vArray[root->vIndex]->left =  root->vArray[root->vIndex-1]->right;
	root->vArray[root->vIndex]->lHeight = root->vArray[root->vIndex-1]->lHeight;
	if(root->vArray[root->vIndex]->middleLR == 0 || root->vArray[root->vIndex]->middleLR == 2)
	  root->vArray[root->vIndex]->middleLR+=1;
  }
  free(temp->vArray);
  free(temp);
  return root;
}

btNode* rotateLeft(btNode* root, int i) {
	if((i < root->vIndex) && (root->vIndex > 0)) {
		btNode* temp = root->vArray[i+1]->left;
		if(temp == NULL) {

		}
		else {
			temp->vArray = growNodeArray(temp->vArray, ++temp->vIndex);
			shiftRight(&temp, 0);
			root->vArray[i]->right = NULL;
			root->vArray[i]->rHeight = NULL;
			if(root->vArray[i]->middleLR == 2 || root->vArray[i]->middleLR == 3)
			   root->vArray[i]->middleLR-=2;
			temp->vArray[i] = root->vArray[i];
			root->vArray[i+1]->lHeight = temp->vArray[i]->lHeight + 1;
			shiftLeft(&root, i);
            root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
		}

	}
	else if(root->vArray[i]->right->vIndex > 0) {
		btNode* temp = root->vArray[i]->right;
		root->vArray[i]->right = NULL;
		root->vArray[i]->rHeight = 0;

		//right
		temp->vArray[0]->right = createBtNode();
		temp->vArray[0]->right->vArray = growNodeArray(temp->vArray[0]->right->vArray, ++temp->vArray[0]->right->vIndex);
		temp->vArray[0]->right->vArray[0] = temp->vArray[temp->vIndex];
		temp->vArray[temp->vIndex] = NULL;
		temp->vArray = shrinkNodeArray(temp->vArray, --temp->vIndex);
		temp->vArray[temp->vIndex]->rHeight = temp->vArray[temp->vIndex]->right->vArray[0]->rHeight + 1;

		//left
		temp->vArray[0]->left = createBtNode();
		temp->vArray[0]->left->vArray = growNodeArray(temp->vArray[0]->left->vArray, ++temp->vArray[0]->left->vIndex);
		temp->vArray[0]->left->vArray[0] = root->vArray[i];
		temp->vArray[0]->lHeight = temp->vArray[0]->left->vArray[0]->lHeight + 1;

		root->vArray[i] = temp->vArray[temp->vIndex];
		temp->vArray[temp->vIndex] = NULL;
		free(temp->vArray[temp->vIndex]);
		free(temp);
		temp = NULL;
		root->vArray[i-1]->right = root->vArray[i]->left;
		root->vArray[i-1]->rHeight = root->vArray[i]->lHeight;

		if(root->vArray[i-1]->middleLR == 0 || root->vArray[i-1]->middleLR == 1)
			root->vArray[i-1]->middleLR+=2;
	}
	else
		root = addNodeLeft(root, root->vArray[i]->right);
  	return root;
}

btNode* rotateRight(btNode* root, int i) {
	if(root->vArray[i]->left->vIndex > 0) {
		btNode* temp = root;
		root->vArray[i] = NULL;
		root->vArray[i] = temp->vArray[0];
		temp->vArray[0] = NULL;
	}
	else {
		root =  addNodeRight(root->vArray[i]->left,root);

	}
	return root;
}

btNode* findMaxNode(btNode* root, int* data) {
  if(root->vArray[root->vIndex]->right == NULL) {
    *data = root->vArray[root->vIndex]->data;
	if(root->vIndex > 0) {
	  free(root->vArray[root->vIndex]);
	  root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
	  if(root->vArray[root->vIndex]->middleLR == 2 || root->vArray[root->vIndex]->middleLR == 3)
	    root->vArray[root->vIndex]->middleLR-=2;
	}
	else {
	  free(root->vArray[root->vIndex]);
	  free(root->vArray);
	  free(root);
	  root = NULL;
	}
  	return root;
  }
  root->vArray[root->vIndex]->right = findMaxNode(root->vArray[root->vIndex]->right, data);
  if(root->vArray[root->vIndex]->right == NULL) {
	root->vArray[root->vIndex]->rHeight = 0;
    root = rotateRight(root, root->vIndex);
  }
  else {
    root->vArray[root->vIndex]->rHeight = root->vArray[root->vIndex]->right->vArray[0]->rHeight + 1;
	if(root->vArray[root->vIndex]->rHeight < root->vArray[root->vIndex]->lHeight)
	  root = rotateRight(root, root->vIndex);
  }

  return root;
}


btNode* deleteBtNode(btNode* root, int data) {
	int i;
	btNode *temp = NULL;
	for(i = 0; i <= root->vIndex; i++) {
		if(root->vArray[i]->data == data) {
			if(root->vArray[i]->left != NULL) {
				if(root->vArray[i]->left->vIndex > 0) {

				}
				else if(root->vArray[i]->right->vIndex > 0) {

				}
				else {
					root->vArray[i]->left = findMaxNode(root->vArray[i]->left, &root->vArray[i]->data);
					if(root->vArray[i]->left == NULL) {
                    	root->vArray[i]->lHeight = 0;
						if(root->vArray[i]->right != NULL) {
							root->vArray = growNodeArray(root->vArray, ++root->vIndex);
							root->vArray[root->vIndex] = root->vArray[i]->right->vArray[0];
							root->vArray[i]->right->vArray[0] = NULL;
							root->vArray[i]->rHeight = NULL;
							free(root->vArray[i]->right);
							root->vArray[i]->right = NULL;
							break;
						}
					}
					else {
						root->vArray[i]->lHeight = root->vArray[i]->left->vArray[0]->lHeight+1;
						if(root->vArray[i]->lHeight < root->vArray[i]->rHeight) {
							root = rotateLeft(root, i);
						}
						break;
					}
				}
			}
			else if(root->vArray[i]->right != NULL) {

			}
			else {
				free(root->vArray[i]);
				free(root->vArray);
				free(root);
				root = NULL;
				return NULL;
			}
		}
		else if(root->vArray[i]->data > data) {
			root->vArray[i]->left = deleteBtNode(root->vArray[i]->left, data);
			if(root->vArray[i]->left == NULL) {
				root->vArray[i]->lHeight = 0;
				if(root->vArray[i]->middleLR == 1 || root->vArray[i]->middleLR == 3)
					root->vArray[i]->middleLR-=1;
				if(i > 0) {
					root->vArray[i-1]->right = NULL;
					root->vArray[i-1]->rHeight = 0;
					if(root->vArray[i-1]->middleLR == 2 || root->vArray[i-1]->middleLR == 3)
						root->vArray[i-1]->middleLR-=2;
				}
			}
			else {
				root->vArray[i]->lHeight = root->vArray[i]->left->vArray[0]->lHeight + 1;
			}
			if(root->vArray[i]->lHeight < root->vArray[i]->rHeight) {
				root = rotateLeft(root, i);
			}
			break;
		}
		else if(root->vIndex == i && root->vArray[i]->right != NULL) {
			root->vArray[i]->right = deleteBtNode(root->vArray[i]->right, data);
			if(root->vArray[i]->right == NULL) {

			}
			else {
				root->vArray[i]->rHeight = root->vArray[i]->right->vArray[0]->rHeight + 1;
			}
			if(root->vArray[i]->right == NULL && root->vArray[i]->rHeight < root->vArray[i]->lHeight) {
				root = rotateRight(root, i);
			}
			else if(root->vArray[i]->left == NULL && root->vArray[i]->right->vIndex > 0) {

			}
			else {
				root->vArray[i]->left->vArray = growNodeArray(root->vArray[i]->left->vArray, ++root->vArray[i]->left->vIndex);
				if(i > 0) {
					if(root->vArray[i-1]->middleLR == 2 || root->vArray[i-1]->middleLR == 3)
                    	root->vArray[i-1]->middleLR-=2;
				}
				root->vArray[i]->left->vArray[root->vArray[i]->left->vIndex] = root->vArray[i];

				root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
				temp = root->vArray[i-1]->right;
				temp->vArray[temp->vIndex]->left = temp->vArray[temp->vIndex-1]->right;
				temp->vArray[temp->vIndex]->lHeight = temp->vArray[temp->vIndex]->left->vArray[0]->lHeight + 1;
				if(temp->vArray[temp->vIndex]->middleLR == 0 || temp->vArray[temp->vIndex]->middleLR == 2)
                    temp->vArray[temp->vIndex]->middleLR++;
			}
			break;
		}
	}
	return root;
}

int main()
{
	btNode *root = NULL;
	printf("Enter b_tree order/degree.\n");
	scanf("%d", &treeDegree);
	maxValue = treeDegree - 1;

	root = insertBtNode(root, 20);
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

	root = deleteBtNode(root, 33);
	root = deleteBtNode(root, 31);
	root = deleteBtNode(root, 15);
	root = deleteBtNode(root, 5);
	root = deleteBtNode(root, 60);
  	return 0;
}
