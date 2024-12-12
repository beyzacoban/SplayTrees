#include <stdio.h>
#include <stdlib.h>

struct node {
	char key;
	struct node *left, *right;
};

struct node* newNode(char key) {
	struct node* node=(struct node*)malloc(sizeof(struct node));
	node->key=key;
	node->left=node->right=NULL;
	return (node);
}

int comparison_count=0;
int rotation_count=0;



struct node *rightRotate(struct node* x) {
	struct node* y=x->left;
	x->left=y->right;
	y->right=x;
	rotation_count++;
	return y;
}


struct node *leftRotate(struct node *x) {
	struct node *y =x->right;
	x->right=y->left;
	y->left=x;
	rotation_count++;
	return y;
}

struct node *splay(struct node *root,char key) {
	if(root==NULL|| root->key==key) {
	comparison_count++;
	return root;
	}
	if(root->key>key) {
		comparison_count++;
		if(root->left==NULL) {
			return root;
		}
	
	//zig-zig 
	if(root->left->key>key) {
		comparison_count++;

		root->left->left=splay(root->left->left,key);
		
		root=rightRotate(root);
	//zig-zag	
	} else if(root->left->key<key) {
		comparison_count++;
		root->left->right=splay(root->left->right,key);
		
		if(root->left->right!=NULL){
			root->left=leftRotate(root->left);
		
	}
}
	return (root->left==NULL)?root:rightRotate(root);
	} else { 
		comparison_count++;
	   if(root->right==NULL) {
		return root;
	}
	//zag-zig
	if(root->right->key>key) {
		comparison_count++;
		root->right->left=splay(root->right->left,key);
		
		if(root->right->left!=NULL) {
			root->right=rightRotate(root->right);
		}
			//zag-zag
		} else if(root->right->key<key) {
			comparison_count++;
			root->right->right=splay(root->right->right,key);
			root=leftRotate(root);
		}
		return root->right==NULL ?root:leftRotate(root);
	
	

}}


	struct node *search(struct node *root, char key) {
    return splay(root, key);
}
 

void preOrder(struct node *root) {
    if (root != NULL) {
        printf("%c ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main() {
    struct node* root = newNode('W');
    root->left = newNode('O');
    root->right = newNode('R');
    root->left->left = newNode('L');
    root->right->right = newNode('D');
 
    printf("Pre-order traversal before search:\n");
    preOrder(root);
    printf("\n");

    root = search(root, 'L');

    printf("Pre-order traversal after search:\n");
    preOrder(root);
    printf("\n");

    int total_cost = comparison_count + rotation_count;
    printf("Total comparisons: %d\n", comparison_count);
    printf("Total rotations: %d\n", rotation_count);
    printf("Total cost: %d\n", total_cost);
    return 0;
}

