// program create a b-tree (balancing tree)
//Date 19-Dec-2022
//23-January-2023 (insertion complited with my logic)

#include <stdio.h>
#include <stdlib.h>

int treeDegree = -1, maxValue = -1;

typedef struct node node;
typedef struct btNode btNode;

struct node {
	int data;
	int middleLR;
	btNode* left;
	btNode* right;
	int lHeight;
	int rHeight;
};

struct btNode {
	node** vArray;
	int vIndex;
	int split;
};

btNode* insertBtNode(btNode*, int);

node* createNode(int data) {
	node* temp = (node*)malloc(sizeof(node));
	if(!temp)
		printf("Node memory not allocated.\n");
	else {
		temp->data = data;
		temp->left = NULL;
		temp->right = NULL;
		temp->middleLR = 0;
		temp->lHeight = 0;
		temp->rHeight = 0;
	}
	return temp;
}

btNode* createBtNode(void) {
	btNode* root = NULL;
	root = (btNode*)malloc(sizeof(btNode));
	if(!root) {
		printf("btNode memory not allocated.\n");
	}
	else {
		root->vArray = NULL;
		root->vIndex = -1;
		root->split = 0;
	}
	return root;
}

node** growNodeArray(node** vArray, int size) {
	vArray = (node**)realloc(vArray,(size+1) * sizeof(node*));
	if(!vArray) {
		printf("Node pointer memory not allocated.\n");
	}
	else {
		*(vArray + size) = NULL;
	}
	return vArray;
}

void splitNodes(btNode** root) {
	int sIndex, i, iCount = 0;
	btNode* btTemp1 = NULL;

	if(*root == NULL) {
		return;
	}
	else {
		sIndex = (*root)->vIndex /2;

    //left
		btTemp1 = createBtNode();
		btTemp1->vIndex = sIndex-1;
		btTemp1->vArray = growNodeArray(btTemp1->vArray, btTemp1->vIndex);
		for(i = 0; i <= btTemp1->vIndex; i++) {
			*(btTemp1->vArray + i) = *(((*root)->vArray) + i);
			*(((*root)->vArray + i)) = NULL;
		}
		*((*root)->vArray + 0) = *((*root)->vArray + (sIndex));
		*((*root)->vArray + (sIndex)) = NULL;


		if((*((*root)->vArray + 0))->left == NULL) {
			(*((*root)->vArray + 0))->lHeight++;
			(*((*root)->vArray + 0))->left = btTemp1;
		}
		else {
			if( (*(btTemp1->vArray + btTemp1->vIndex))->middleLR == 2 ) {
			  (*(btTemp1->vArray + btTemp1->vIndex))->middleLR = 0;
			}

			if( (*((*root)->vArray + 0))->middleLR == 1 || (*((*root)->vArray + 0))->middleLR == 3 ) {
				(*((*root)->vArray + 0))->middleLR-=1;
			}

			(*((*root)->vArray + 0))->lHeight = (*(btTemp1->vArray + 0))->lHeight + 1;
			(*((*root)->vArray + 0))->left = btTemp1;

		}

		//right
		btTemp1 = createBtNode();
		btTemp1->vIndex = (*root)->vIndex - (sIndex+1);
  	btTemp1->vArray = growNodeArray(btTemp1->vArray, btTemp1->vIndex);
		for(i = 0; i <= btTemp1->vIndex; i++) {
			*(btTemp1->vArray + i) = *((*root)->vArray + (sIndex+(i+1)));
			*((*root)->vArray + (sIndex+(i+1))) = NULL;
		}

		if( (*((*root)->vArray + 0))->right == NULL ) {
      (*((*root)->vArray + 0))->rHeight++;
      (*((*root)->vArray + 0))->right  = btTemp1;
		}
		else {
			if( (*((*root)->vArray + 0))->middleLR == 2 ) {
				(*((*root)->vArray + 0))->middleLR = 0;
			}
			if( (*(btTemp1->vArray + 0))->middleLR == 1 || (*(btTemp1->vArray + 0))->middleLR == 3) {
        (*(btTemp1->vArray + 0))->middleLR-=1;
			}

			(*((*root)->vArray + (sIndex-1)))->rHeight = (*(btTemp1->vArray + 0))->rHeight + 1;
			(*((*root)->vArray + (sIndex-1)))->right  = btTemp1;
		}

		for(i = 0; i <= (*root)->vIndex; i++) {
			if( *((*root)->vArray + i) == NULL)
        iCount++;
		}

		(*root)->vIndex-=iCount;
		(*root)->vArray = (node**)realloc( (*root)->vArray, (sizeof(node*) * ((*root)->vIndex + 1)) );
		(*root)->split = 1;
	}
}

 btNode* insertBtNode(btNode* root, int data) {
	 int i, j;
   btNode* temp = NULL;
	 if(root == NULL) {
		 root = createBtNode();
		 root->vArray = growNodeArray(root->vArray, ++root->vIndex);
		 *(root->vArray + root->vIndex) = createNode(data);
	 }
	 else {
		 for(i = 0; i <= root->vIndex; i++) {
			 if(data < (*(root->vArray + i))->data)
         break;
		 }

		 if(i <= root->vIndex) {
			 if( (*(root->vArray + i))->left == NULL ) {
				 root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				 for(j = root->vIndex; j > i; j--) {
					 *(root->vArray + j) = *(root->vArray + (j-1));
				 }
				 *(root->vArray + i) = createNode(data);
			 }
			 else {
				 temp = insertBtNode((*(root->vArray + i))->left, data);
				 if(temp->split == 1) {
					 root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					 for(j = root->vIndex; j > i; j--) {
						 *(root->vArray + j) = *(root->vArray + (j-1));
					 }
					 *(root->vArray + i) = *(temp->vArray + temp->vIndex);
					 free(temp->vArray);
					 free(temp);
					 (*(root->vArray + (i+1)))->left = (*(root->vArray + i))->right;
					 (*(root->vArray + (i+1)))->middleLR = 1;
					 for(j = i-1; j >= 0; j--) {
						 if( (*(root->vArray + j))->middleLR > 0 ) {
							 if( (*(root->vArray + j))->middleLR == 3 ) {
								 (*(root->vArray + j))->left = (*(root->vArray + (j-1)))->right;
								 (*(root->vArray + j))->right = (*(root->vArray + (j+1)))->left;
							 }
							 else if( (*(root->vArray + j))->middleLR == 2 ) {
                 (*(root->vArray + j))->right = (*(root->vArray + (j+1)))->left;
							 }
							 else if( (*(root->vArray + j))->middleLR == 1 ) {
                 (*(root->vArray + j))->right = (*(root->vArray + (j+1)))->left;
							 }
						 }
					 }
				 }
				 else {
					 (*(root->vArray + i))->left = temp;
				 }
			 }
		 }
		 else {
			 if( (*(root->vArray + (i-1)))->right == NULL ) {
				 root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				 *(root->vArray + root->vIndex) = createNode(data);
			 }
			 else {
				 temp = insertBtNode((*(root->vArray + (i-1)))->right, data);
				 if(temp->split == 1) {
					 root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					 *(root->vArray + root->vIndex) = *(temp->vArray + temp->vIndex);
					 free(temp->vArray);
					 free(temp);
					 (*(root->vArray + (root->vIndex-1)))->right = (*(root->vArray + root->vIndex))->left;
					 if( (*(root->vArray + (root->vIndex-1)))->middleLR == 0 ) {
             (*(root->vArray + (root->vIndex-1)))->middleLR = 2;
					 }
					 else if( (*(root->vArray + (root->vIndex-1)))->middleLR == 1 ) {
						 (*(root->vArray + (root->vIndex-1)))->middleLR = 3;
					 }
				 }
				 else {
					 (*(root->vArray + (i-1)))->right = temp;
				 }
			 }
		 }
		 if(root->vIndex == maxValue) {
			 splitNodes(&root);
		 }
	 }
	 return root;
 }

node** shrinkNodeArray(node** vArray, int size) {
	vArray = (node**)realloc(vArray, (size+1) * sizeof(node**));
	if(!vArray) {
    printf("vArray shrink null.\n");
	}
	return vArray;
}

 btNode* maxNode(btNode* root, int* data) {
	 if( (*(root->vArray + root->vIndex))->right == NULL ) {
		 *data = (*(root->vArray + root->vIndex))->data;
		 free((root->vArray + root->vIndex));
		 if(root->vIndex > 0) {
			 root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
		 }
		 else {
			 free(root);
			 root = NULL;
		 }
		 return root;
	 }
	 else {
     (*(root->vArray + root->vIndex))->right = maxNode((*(root->vArray + root->vIndex))->right, data);
	 }
	 return root;
 }

btNode* rotateRight(btNode* root, int i) {
  btNode* btTemp = NULL;
	if(i == 0) {
	  btTemp = (*(root->vArray + 0))->left;
		(*(root->vArray + 0))->left = NULL;
		(*(root->vArray + 0))->lHeight = 0;
		btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
		*(btTemp->vArray + btTemp->vIndex) = *(root->vArray + 0);
			if((*(btTemp->vArray + (btTemp->vIndex-1)))->right != NULL) {
				(*(btTemp->vArray + btTemp->vIndex))->left = (*(btTemp->vArray + (btTemp->vIndex-1)))->right;
				(*(btTemp->vArray + btTemp->vIndex))->lHeight = (*(btTemp->vArray + (btTemp->vIndex-1)))->rHeight;
				(*(btTemp->vArray + btTemp->vIndex))->middleLR = 1;
			}
		free(root->vArray);
		free(root);
	}
	else {
		if((*(root->vArray + i))->right == NULL) {
			if((*(root->vArray + i))->middleLR == 1)
				(*(root->vArray + i))->middleLR = 0;
			else
				(*(root->vArray + (i-1)))->middleLR = 2;
			btTemp = (*(root->vArray + i))->left;
			btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
			*(btTemp->vArray + btTemp->vIndex) = *(root->vArray + i);
			(*(btTemp->vArray + btTemp->vIndex))->left = NULL;
			(*(btTemp->vArray + btTemp->vIndex))->lHeight = (*(btTemp->vArray + (btTemp->vIndex-1)))->lHeight;
			root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
			return root;
		}

	}
	 return btTemp;
 }

btNode* rotateLeft(btNode* root, int i) {
	btNode* btTemp = NULL;
	btNode* btTemp2 = NULL;
	int j;
	if(root->vIndex == 0) {
		btTemp = (*(root->vArray + 0))->right;
		(*(root->vArray + 0))->right = NULL;
		(*(root->vArray + 0))->rHeight = 0;
		root->vArray = growNodeArray(root->vArray, ++root->vIndex);
		*(root->vArray + root->vIndex) = *(btTemp->vArray + 0);
		if((*(root->vArray + root->vIndex))->left != NULL) {
			(*(root->vArray + (root->vIndex-1)))->right = (*(root->vArray + root->vIndex))->left;
			(*(root->vArray + (root->vIndex-1)))->rHeight = (*(root->vArray + root->vIndex))->lHeight;
			(*(root->vArray + (root->vIndex-1)))->middleLR = 2;
		}
		free(btTemp->vArray);
		free(btTemp);
	}
	else {
		if((*(root->vArray + (i-1)))->left == NULL) {

		}
		else {
			if((*(root->vArray + (i-1)))->left->vIndex > 0) {
				btTemp = createBtNode();
				btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
				*(btTemp->vArray + btTemp->vIndex) = createNode((*(root->vArray + (i-1)))->data);
				btTemp2 = (*(root->vArray + (i-1)))->left;
				(*(root->vArray + (i-1)))->data = (*(btTemp2->vArray + btTemp2->vIndex))->data;
				if((*(btTemp2->vArray + (btTemp2->vIndex-1)))->middleLR == 2 || (*(btTemp2->vArray + (btTemp2->vIndex-1)))->middleLR == 3) {
					(*(btTemp2->vArray + (btTemp2->vIndex-1)))->middleLR-=2;
				}
				(*(btTemp->vArray + btTemp->vIndex))->left = (*(btTemp2->vArray + btTemp2->vIndex))->right;
				(*(btTemp2->vArray + btTemp2->vIndex))->right = NULL;
				btTemp2->vArray  = shrinkNodeArray(btTemp2->vArray, --btTemp2->vIndex);
				(*(btTemp->vArray + btTemp->vIndex))->lHeight = (*((*(btTemp->vArray + btTemp->vIndex))->left->vArray + 0))->lHeight + 1;
				(*(btTemp->vArray + btTemp->vIndex))->right = (*(root->vArray + (i-1)))->right;
				(*(btTemp->vArray + btTemp->vIndex))->rHeight = (*((*(btTemp->vArray + btTemp->vIndex))->right->vArray + 0))->rHeight + 1;

				(*(root->vArray + (i-1)))->right = btTemp;
				if((*(root->vArray + (i-1)))->middleLR > 1)
					(*(root->vArray + (i-1)))->middleLR-=2;

				(*(root->vArray + i))->left = btTemp;
				(*(root->vArray + i))->lHeight = (*(root->vArray + (i-1)))->rHeight;

				if((*(root->vArray + i))->middleLR == 0 || (*(root->vArray + i))->middleLR == 2)
					(*(root->vArray + i))->middleLR+=1;
				btTemp = NULL;
				btTemp2 = NULL;
			}
			else if((*(root->vArray + i))->right->vIndex > 0) {
				btTemp = createBtNode();
				btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
				btTemp2 = (*(root->vArray + i))->right;
				(*(btTemp->vArray + btTemp->vIndex)) = createNode((*(root->vArray + i))->data);
				(*(root->vArray + i))->data = (*(btTemp2->vArray + 0))->data;
				(*(btTemp->vArray + btTemp->vIndex))->right = (*(btTemp2->vArray + 0))->left;
				(*(btTemp->vArray + btTemp->vIndex))->rHeight = (*((*(btTemp->vArray + btTemp->vIndex))->right->vArray + 0))->rHeight + 1;
				(*(btTemp2->vArray + 0))->left = NULL;
				if((*(btTemp2->vArray + 1))->middleLR == 1 || (*(btTemp2->vArray + 1))->middleLR == 3) {
					(*(btTemp2->vArray + 1))->middleLR-=1;
				}
				for(j = 0; j < btTemp2->vIndex; j++) {
					*(btTemp2->vArray + j) = *(btTemp2->vArray + (j+1));
				}
				*(btTemp2->vArray + j) = NULL;
				btTemp2->vArray = shrinkNodeArray(btTemp2->vArray, --btTemp2->vIndex);

				(*(btTemp->vArray + btTemp->vIndex))->left = (*(root->vArray + i))->left;
				(*(btTemp->vArray + btTemp->vIndex))->lHeight = (*((*(btTemp->vArray + btTemp->vIndex))->left->vArray + 0))->lHeight + 1;
				(*(root->vArray + i))->left = btTemp;
				(*(root->vArray + i))->lHeight = (*(btTemp->vArray + btTemp->vIndex))->lHeight + 1;
				(*(root->vArray + (i-1)))->right = btTemp;
				(*(root->vArray + (i-1)))->rHeight = (*(btTemp->vArray + btTemp->vIndex))->lHeight + 1;
				if((*(root->vArray + (i-1)))->middleLR == 0 || (*(root->vArray + (i-1)))->middleLR == 1)
					(*(root->vArray + (i-1)))->middleLR+=2;
				btTemp = NULL;
				btTemp2 = NULL;
			}
			else  {
				btTemp = (*(root->vArray + (i-1)))->left;
				btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
				*(btTemp->vArray + btTemp->vIndex) = *(root->vArray + (i-1));
				if((*(btTemp->vArray + btTemp->vIndex))->middleLR == 2)
					(*(btTemp->vArray + btTemp->vIndex))->middleLR = 0;
				(*(btTemp->vArray + btTemp->vIndex))->left = (*(btTemp->vArray + (btTemp->vIndex-1)))->right;
				(*(btTemp->vArray + btTemp->vIndex))->middleLR = 1;
				(*(btTemp->vArray + btTemp->vIndex))->lHeight = (*(btTemp->vArray + (btTemp->vIndex-1)))->rHeight;
				(*(btTemp->vArray + btTemp->vIndex))->rHeight = (*((*(btTemp->vArray + btTemp->vIndex))->right->vArray + 0))->rHeight;
				for(j = i-1; j < root->vIndex; j++) {
					*(root->vArray + j) = *(root->vArray + (j+1));
				}
				*(root->vArray + j) = NULL;
				root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
				(*(root->vArray + root->vIndex))->left = btTemp;
				(*(root->vArray + root->vIndex))->lHeight =  (*((*(root->vArray + root->vIndex))->left->vArray + 0))->lHeight + 1;
				(*(root->vArray + root->vIndex))->rHeight =  (*((*(root->vArray + root->vIndex))->right->vArray + 0))->rHeight + 1;
			}
		}
	}
	return root;
}

btNode* deleteBtNode(btNode* root, int value) {
	int i, j;
	if(root == NULL)
		printf("%d is not found in B-Tree.\n", value);
	else {
		for(i = 0; i <= root->vIndex; i++) {
			if( value == (*(root->vArray + i))->data ) {
				if( (*(root->vArray + i))->left != NULL ) {
					(*(root->vArray + i))->left = maxNode((*(root->vArray + i))->left, &(*(root->vArray + i))->data);
					if( (*(root->vArray + i))->left == NULL ) {
						(*(root->vArray + i))->lHeight = 0;
						return rotateLeft(root, i);
				 }
				 else {
					 (*(root->vArray + i))->lHeight = (*(((*(root->vArray + i))->left)->vArray + 0))->lHeight + 1;
					 if( (*(root->vArray + i))->lHeight < (*(root->vArray + i))->rHeight ) {
						 if(i > 0) {

						 }
						 else {
							 return rotateLeft(root, i);
             }
					 }
				 }
				}
				else {
					free(*(root->vArray + i));
					for(; i < root->vIndex; i++) {
						*(root->vArray + i) = *(root->vArray + (i+1));
					}
					if(i == 0) {
						free(root);
						return NULL;
					}
					root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
				}
				break;
			}
			else {
			 if( value > (*(root->vArray + i))->data  && (i == root->vIndex) ) {
				 //right
				 (*(root->vArray + i))->right = deleteBtNode((*(root->vArray + i))->right, value);
				 if( (*(root->vArray + i))->right == NULL ) {
           (*(root->vArray + i))->rHeight = 0;
           return rotateRight(root, i);
				 }
				 else {
					 (*(root->vArray + i))->rHeight = (*(((*(root->vArray + i))->right)->vArray + 0))->rHeight + 1;
					 if( (*(root->vArray + i))->rHeight < (*(root->vArray + i))->lHeight ) {
						 if(i > 0) {

						 }
						 else {
               return rotateRight(root, i);
						 }
					 }
					 break;
				 }
			 }
			 else if( value < (*(root->vArray + i))->data ) {
				 //left
				 (*(root->vArray + i))->left = deleteBtNode((*(root->vArray + i))->left, value);
				 if( (*(root->vArray + i))->left == NULL ) {
           (*(root->vArray + i))->lHeight = 0;
					 return rotateLeft(root, i);
				 }
				 else {
					 (*(root->vArray + i))->lHeight = (*(((*(root->vArray + i))->left)->vArray + 0))->lHeight + 1;
					 if( (*(root->vArray + i))->lHeight < (*(root->vArray + i))->rHeight ) {
						 if(i > 0) {
							 root = rotateLeft(root, i);
						 }
						 else
               return rotateLeft(root, i);
					 }
					 break;
				 }
			 }
			}
		}
	}
	return root;
}


int main() {
	btNode* root = NULL;
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

  root = deleteBtNode(root, 65);
	root = deleteBtNode(root, 31);
	root = deleteBtNode(root, 15);
//	root = deleteBtNode(root, 40);
	system("PAUSE");
	return 0;
}





/*
odd logic
btNode* rotateLeft(btNode* root) {
	int i;
	btNode* btTemp =  (*(root->vArray + 0))->right;
	btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
	for(i = btTemp->vIndex; i > 0; i--) {
		*(btTemp->vArray + i) = *(btTemp->vArray + (i-1));
	}
	(*(root->vArray + 0))->right = NULL;
	(*(root->vArray + 0))->rHeight = 0;
	*(btTemp->vArray + i) = *(root->vArray + 0);
	if((*(btTemp->vArray + (i+1)))->left != NULL) {
		(*(btTemp->vArray + i))->right = (*(btTemp->vArray + (i+1)))->left;
		(*(btTemp->vArray + i))->rHeight = (*(((*(btTemp->vArray + i))->right)->vArray + 0))->rHeight + 1;
		(*(btTemp->vArray + i))->middleLR = 2;
	}
	free(root->vArray);
	free(root);
	return btTemp;
}

*/

/* old delection logic

btNode* deleteBtNode(btNode* root, int value) {
	int i, j;
	if(root == NULL) {
		printf("%d node is not present in b-tree.\n", value);
		return NULL;
	}

	for(i = 0; i <= root->vIndex; i++) {
		if( value < (*(root->vArray + i))->data ) {
			(*(root->vArray + i))->left = deleteBtNode((*(root->vArray + i))->left, value);
			if( (*(root->vArray + i))->left == NULL ) {
				(*(root->vArray + i))->lHeight--;
			}
			else {
				(*(root->vArray + i))->lHeight = (*((*(root->vArray + i))->left)->vArray + 0)->lHeight + 1;
			}
			if( (*(root->vArray + 0))->lHeight != (*(root->vArray + 0))->rHeight) {
				root = rotateLeft(root);
			}
			return root;
		}
	}
	--i;
	if( value == (*(root->vArray + i))->data ) {
		if( (*(root->vArray + i))->left != NULL ) {
			(*(root->vArray + i))->left = maxNode((*(root->vArray + i))->left, &(*(root->vArray + i))->data);
			if( (*(root->vArray + i))->left == NULL ) {
				root = rotateLeft(root);
				if(root->vIndex == maxValue) {
					splitNodes(&root);
				}
			}
		}
		else {
			free((root->vArray + i));
			if(root->vIndex == 0) {
				free(root);
				return NULL;
			}
			for(; i > root->vIndex; i++) {
				*(root->vArray + i) = *(root->vArray + (i+1));
			}
			root->vArray = shrinkNodeArray(root->vArray, --root->vIndex);
		}
	}
	else {
		(*(root->vArray + i))->right = deleteBtNode( (*(root->vArray + i))->right, value );
		if( (*(root->vArray + i))->right == NULL) {
			(*(root->vArray + i))->rHeight--;
		}
		else {
			(*(root->vArray + 0))->rHeight = (*(((*(root->vArray + i))->right)->vArray + 0))->rHeight + 1;
		}
		if( (*(root->vArray + 0))->lHeight != (*(root->vArray + 0))->rHeight) {
			root = rotateRight(root);
		}
	}
	return root;
 }









*/


 /*
 int balanceBTree(btNode** root) {
	 int lCount = 0, rCount = 0;

	 if( (*((*(root))->vArray + 0))->left != NULL ) {
		 lCount++;
		 lCount+=balanceBTree( &(*((*(root))->vArray + 0))->left );
	 }

	 if( (*((*(root))->vArray + 0))->right != NULL ) {
		 rCount++;
		 rCount+=balanceBTree( &(*((*(root))->vArray + 0))->right );
	 }

	 if(lCount > rCount) {
		 *root = rotateRight(*root);
		 lCount = rCount;
	 }
	 else if(lCount < rCount) {
		 *root = rotateLeft(*root);
	 }
	 return lCount;
 }

 */

//3rd try

/*
void split(btNode** root) {
	btNode* btTemp = NULL;
	btNode* btTemp2 = NULL;
	btNode* btTemp3 = NULL;
	int sIndex = 0, i, j, iCount = 0;

	sIndex = (*root)->vIndex/2;

  //this code is for left tree
	btTemp = createBtNode();
	btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
	*(btTemp->vArray + btTemp->vIndex) = *((*root)->vArray + (sIndex-1));
	*((*root)->vArray + (sIndex-1)) = *((*root)->vArray + (sIndex));
	*((*root)->vArray + (sIndex)) = NULL;
	if((*((*root)->vArray + (sIndex-1)))->left != NULL) {
		if((*(btTemp->vArray + btTemp->vIndex))->right == NULL) {
			(*(btTemp->vArray + btTemp->vIndex))->right = (*((*root)->vArray + (sIndex-1)))->left;
			(*((*root)->vArray + (sIndex-1)))->left = NULL;
		}
		else {
			btTemp2 = (*((*root)->vArray + (sIndex-1)))->left;
			btTemp3 = (*(btTemp->vArray + btTemp->vIndex))->right;
			for(i = 0; i <= btTemp3->vIndex; i++) {
				if((*(btTemp2->vArray + btTemp2->vIndex))->data < (*(btTemp3->vArray + i))->data)
					break;
			}
			btTemp3->vArray = growNodeArray(btTemp3->vArray, ++btTemp3->vIndex);
			if(i < btTemp3->vIndex) {
				for(j = btTemp3->vIndex; j > i; j--) {
					*(btTemp3->vArray + j) = *(btTemp3->vArray + (j-1));
				}
			}
			*(btTemp3->vArray + i) = *(btTemp2->vArray + btTemp2->vIndex);
			*(btTemp2->vArray + btTemp2->vIndex) = NULL;
			free( (btTemp2->vArray + btTemp2->vIndex) );
			free( btTemp2->vArray );
			btTemp2 = NULL;

			(*(btTemp->vArray + btTemp->vIndex))->right = btTemp3;
			btTemp3 = NULL;
		}
	}
	(*((*root)->vArray + (sIndex-1)))->left = btTemp;
	btTemp = NULL;

	//this code is for right tree
	btTemp = createBtNode();
	btTemp->vArray = growNodeArray(btTemp->vArray, ++btTemp->vIndex);
	*(btTemp->vArray + btTemp->vIndex) = *((*root)->vArray + (sIndex+1));
	*((*root)->vArray + (sIndex+1)) = NULL;

	if((*((*root)->vArray + (sIndex-1)))->right != NULL) {
		if((*(btTemp->vArray + btTemp->vIndex))->left == NULL) {
			(*(btTemp->vArray + btTemp->vIndex))->left = (*((*root)->vArray + (sIndex-1)))->right;
			(*((*root)->vArray + (sIndex-1)))->right = NULL;
		}
		else {
			btTemp2 = (*((*root)->vArray + (sIndex-1)))->right;
			btTemp3 = (*(btTemp->vArray + btTemp->vIndex))->left;
			for(i = 0; i <= btTemp3->vIndex; i++) {
				if((*(btTemp2->vArray + btTemp2->vIndex))->data < (*(btTemp3->vArray + i))->data)
					break;
			}
			btTemp3->vArray = growNodeArray(btTemp3->vArray, ++btTemp3->vIndex);
			if(i < btTemp3->vIndex) {
				for(j = btTemp3->vIndex; j > i; j--) {
					*(btTemp3->vArray + j) = *(btTemp3->vArray + (j-1));
				}
			}
			*(btTemp3->vArray + i) = *(btTemp2->vArray + btTemp2->vIndex);
			*(btTemp2->vArray + btTemp2->vIndex) = NULL;
			free( (btTemp2->vArray + btTemp2->vIndex) );
			free( btTemp2->vArray );
			btTemp2 = NULL;

			(*(btTemp->vArray + btTemp->vIndex))->left = btTemp3;
			btTemp3 = NULL;
		}
	}
	(*((*root)->vArray + (sIndex-1)))->right = btTemp;
	btTemp = NULL;

	for(i = 0; i <= (*root)->vIndex; i++) {
		if(*((*root)->vArray + i) == NULL)
			iCount++;
	}
	(*root)->vIndex-=iCount;
	(*root)->vArray = (node**)realloc((*root)->vArray, sizeof(node*) * (1+(*root)->vIndex));
	(*root)->split = 1;
}


btNode* insertBtNode(btNode* root, int data) {
	int i, j;
	btNode* temp = NULL;

	if(root == NULL) {
		root = createBtNode();
		root->vArray = growNodeArray(root->vArray, ++root->vIndex);
		*(root->vArray + root->vIndex) = createNode(data);
	}
	else {
		for(i = 0; i < root->vIndex; i++) {
			if(data < (*(root->vArray + i))->data || (*(root->vArray + i))->right != NULL)
				break;
		}

		if(i >= root->vIndex) {
			if(data < (*(root->vArray + i))->data)
			{
				if((*(root->vArray + i))->left != NULL) {
					temp = insertBtNode((*(root->vArray + i))->left, data);  //1
					if(temp->split == 1) {
						root->vArray = growNodeArray(root->vArray, ++root->vIndex);
						for(j = root->vIndex; j > i; j--) {
							*(root->vArray + j) = *(root->vArray + (j-1));
						}
						*(root->vArray + i) = temp;
						temp = NULL;
						(*(root->vArray + (i+1)))->left = NULL;
					}
					else {
						(*(root->vArray + i))->left = temp;
						temp = NULL;
					}
				}
				else {
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					for(j = root->vIndex; j > i; j--) {
						*(root->vArray + j) = *(root->vArray + (j-1));
					}
					*(root->vArray + i) = createNode(data);
				}
			}
			else {
				if((*(root->vArray + i))->right != NULL) {
					temp = insertBtNode((*(root->vArray + i))->right, data);
					if(temp->split == 1) {
						root->vArray = growNodeArray(root->vArray, ++root->vIndex);
						*(root->vArray + root->vArray) = temp;
						temp = NULL;
					}
					(*(root->vArray + i))->right = temp;
				}
				else {
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					*(root->vArray + root->vIndex) = createNode(data);
				}
			}
		}
		else {
			if((*(root->vArray + i))->left != NULL) {
				insertBtNode((*(root->vArray + i))->left, data);
			}
			else {
				root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				for(j = root->vIndex; j > i; j--) {
					*(root->vArray + j) = *(root->vArray + (j-1));
				}
				*(root->vArray + i) = createNode(data);
			}
		}

		if(root->vIndex == maxValue) {
			split(&root);
		}
	}
	return root;
}

*/


// 2nd try
/*
btNode* insertBtNode(btNode* root, int data) {
	int i, j;
	btNode* temp = NULL;
	if(root == NULL) {
		root = createBtNode();
		root->vArray = growNodeArray(root->vArray, ++root->vIndex);
		*(root->vArray + root->vIndex) = createNode(data);
	}
	else {
		for(i = 0; i <= root->vIndex; i++) {
			if((data < (*(root->vArray + i))->data) || ((*(root->vArray + i))->right != NULL))
				break;
		}
		if(i <= root->vIndex) {
			if((*(root->vArray + i))->left == NULL) {
				root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				for(j = root->vIndex; j > i; j--) {
					*(root->vArray + j) = *(root->vArray + (j-1));
				}
				*(root->vArray + j) = createNode(data);
			}
			else {
				temp = insertBtNode((*(root->vArray + i))->left, data);
				if(temp->split == 1) {
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					for(j = root->vIndex; j > 0; j--) {
						*(root->vArray + j) = *(root->vArray + (j-1));
					}
					*(root->vArray + j) = *(temp->vArray + temp->vIndex);
					(*(root->vArray + (j+1)))->left = NULL;
				}
			}
		}
		else {
			if((*(root->vArray + (i-1)))->right == NULL) {
				root->vArray = growNodeArray(root->vArray, ++root->vIndex);
				*(root->vArray + root->vIndex) = createNode(data);
			}
			else {
				temp = insertBtNode((*(root->vArray + (i-1)))->right, data);
				if(temp->split == 1) {
					root->vArray = growNodeArray(root->vArray, ++root->vIndex);
					*(root->vArray + root->vIndex) = *(temp->vArray + temp->vIndex);
					(*(root->vArray + root->vIndex-1))->right = NULL;
					root->split = 0;
					free(temp->vArray);
					free(temp);
				}
			}
		}

		if(root->vIndex == maxValue) {
      split(&root);
		}
	}
	return root;
} */
//old code

/*
btNode* splitBt(btNode* root) {
	int sIndex, i, j, dCount = 0;
	btNode* left = NULL;
	btNode* right = NULL;
	btNode* newRoot = NULL;
	btNode* temp = NULL;

	if(root == NULL) {
		return root;
	}
	else {
		sIndex = (root->vIndex + 1)/2;
		left = createBtNode();
		right = createBtNode();
		left->vArray = growNodeArray(left->vArray, sIndex-1);
		left->vIndex = sIndex-1;
		right->vArray = growNodeArray(right->vArray, (root->vIndex - (sIndex+1)));
		right->vIndex = (root->vIndex - (sIndex+1));

		for(i = 0; i <= left->vIndex; i++) {
			*(left->vArray + i) = *(root->vArray + i);
			*(root->vArray + i) = NULL;
			dCount++;
		}
		if((*(root->vArray + sIndex))->left == NULL) {
			(*(root->vArray + sIndex))->left = left;
		}
		else {
			temp = (*(root->vArray + sIndex))->left;
			for(i = 0; i <= temp->vIndex; i++) {
				if((*(left->vArray + left->vIndex))->data < (*(temp->vArray + i))->data)
					break;
			}
			temp->vIndex++;
			temp->vArray = growNodeArray(temp->vArray, temp->vIndex);
			for(j = temp->vIndex; j > i; j--) {
				*(temp->vArray + j) = *(temp->vArray + (j-1));
			}
			*(temp->vArray + j) = *(left->vArray + left->vIndex);
			if(temp->vIndex == maxValue) {
				temp = splitBt(temp);
				*(root->vArray + (sIndex - 1)) = temp;
				(*(root->vArray + sIndex))->left = NULL;
			}
			*(left->vArray + left->vIndex) = NULL;
			free(left->vArray);
			free(left);
			temp = NULL;
		}
		left = NULL;

		for(i = 0; i <= right->vIndex ;i++) {
			*(right->vArray + i) = *(root->vArray + (sIndex + i+1));
			*(root->vArray + (sIndex + i+1)) = NULL;
			dCount++;
		}
		if((*(root->vArray + sIndex))->right == NULL) {
			(*(root->vArray + sIndex))->right = right;
		}
		else {
			temp = (*(root->vArray + sIndex))->right;
			for(i = 0; i <= temp->vIndex; i++) {
				if((*(right->vArray + right->vIndex))->data < (*(temp->vArray + i))->data)
					break;
			}
			temp->vIndex++;
			temp->vArray = growNodeArray(temp->vArray, temp->vIndex);
			for(j = temp->vIndex; j > i; j--) {
				*(temp->vArray + j) = *(temp->vArray + (j-1));
			}
			*(temp->vArray + j) = *(right->vArray + right->vIndex);
			*(right->vArray + right->vIndex) = NULL;
			free(right->vArray);
			free(right);
			temp = NULL;
		}
    right = NULL;

		newRoot = createBtNode();
		newRoot->vIndex = root->vIndex - dCount;
		newRoot->vArray = growNodeArray(newRoot->vArray, newRoot->vIndex);


		for(i = 0, j = 0; i <= root->vIndex; i++) {
			if(*(root->vArray + i) != NULL) {
				*(newRoot->vArray + j) = *(root->vArray + i);
				*(root->vArray + i) = NULL;
				j++;
			}
		}
		free(root->vArray);
		free(root);
	}
	return newRoot;
}

btNode* insertBtNode(btNode* root, int data) {
	int i, j;
	btNode* temp = NULL;
	if(root == NULL) {
		root = createBtNode();
		root->vArray = growNodeArray(root->vArray, root->vIndex);
		*(root->vArray + root->vIndex) = createNode(data);
	}
	else {
		for(i = 0; i <= root->vIndex; i++) {
			if(data < (*(root->vArray + i))->data)
				break;
		}
		if(i <= root->vIndex) {
			if((*(root->vArray + i))->left == NULL) {
				root->vIndex++;
				root->vArray = growNodeArray(root->vArray, root->vIndex);
				for(j = root->vIndex; j > i; j--) {
					*(root->vArray + j) = *(root->vArray + (j-1));
				}
				*(root->vArray + i) = createNode(data);
			}
			else {
				temp = insertBtNode((*(root->vArray + i))->left, data);
				if( temp->vIndex == 0 ) {
					root->vIndex++;
					root->vArray = growNodeArray(root->vArray, root->vIndex);
					for(j = root->vIndex; j > i; j--) {
						*(root->vArray + j) = *(root->vArray + (j-1));
					}
					*(root->vArray + i) = *(temp->vArray + temp->vIndex);
					(*(root->vArray + (i+1)))->left = NULL;
					*(temp->vArray + temp->vIndex) = NULL;
					free(temp->vArray);
					free(temp);
				}
				else {
					(*(root->vArray + i))->left = temp;
					temp = NULL;
				}
			}
		}
		else {
			if((*(root->vArray + (i-1)))->right == NULL) {
				root->vIndex++;
				root->vArray = growNodeArray(root->vArray, root->vIndex);
				*(root->vArray + root->vIndex) = createNode(data);
			}
			else {
				temp = insertBtNode((*(root->vArray + (i-1)))->right, data);
				if( temp->vIndex == 0 ) {
					root->vIndex++;
					root->vArray = growNodeArray(root->vArray, root->vIndex);
					*(root->vArray + root->vIndex) = *(temp->vArray + temp->vIndex);
					(*(root->vArray + (i-1)))->right = NULL;
					*(temp->vArray + temp->vIndex) = NULL;
					free(temp->vArray);
					free(temp);
				}
				else {
					(*(root->vArray + (i-1)))->right = temp;
					temp = NULL;
				}
			}
		}

		if(root->vIndex == maxValue) {
			root = splitBt(root);
		}
	}
	return root;
}
*/

/*

int findIndex(btNode** root, int data) {
	int i, j;
	for(i = 0; i <= (*root)->vIndex; i++) {
		if(data < (*((*root)->vArray + i))->data)
			break;
	}
	(*root)->vArray = growNodeArray((*root)->vArray, ++((*root)->vIndex));
	if(i < (*root)->vIndex) {
		for(j = (*root)->vIndex; j > i; j--) {
			*((*root)->vArray + j) = *((*root)->vArray + (j-1));
		}
	}
	return i;
}


*/
