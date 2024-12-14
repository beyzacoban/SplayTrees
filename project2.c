#include <stdio.h>
#include <stdlib.h>

 struct node {
	char key;
	int frequency;
	struct node *left;
	struct node *right;
};



// Function Prototypes
struct node* newNode(char key);
struct node* rightRotate(struct node* x);
struct node* leftRotate(struct node* x);
struct node* splay(struct node* root, char key);
struct node* search(struct node* root, char key);
struct node* modSplay(struct node* root, char key);
void preOrder(struct node* root, FILE *outputFile);

int comparison_count=0;
int rotation_count=0;

int main() {
	
    FILE *file;
    char fileName[] = "input.txt";
    file = fopen(fileName, "r");
    
    if (file == NULL) {
        puts("File could not be opened");
        return 1;
    }
    

    FILE *output_file;
    // Open the output file
    output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        puts("Output file could not be opened");
        return 1;
    }

    struct node* splayTree = NULL;
    struct node* modSplayTree = NULL;
    char ch;

    // Process each character in the input file
    while ((ch = fgetc(file)) != EOF) {
        splayTree = splay(splayTree, ch);
        modSplayTree = modSplay(modSplayTree, ch);
    }

    fclose(file);

    // Write results to the output file
    fprintf(output_file, "Splay Aðacý (Pre-Order):\n");
    preOrder(splayTree, output_file);
    fprintf(output_file, "\nToplam Karþýlaþtýrma: %d, Toplam Rotasyon: %d\n", comparison_count, rotation_count);

    comparison_count = 0;
    rotation_count = 0;

    fprintf(output_file, "\nMod-Splay Aðacý (Pre-Order):\n");
    preOrder(modSplayTree, output_file);
    fprintf(output_file, "\nToplam Karþýlaþtýrma: %d, Toplam Rotasyon: %d\n", comparison_count, rotation_count);

    // Close the output file
    fclose(output_file);

    return 0;
}
// YENÝ NODE EKLEME
struct node* newNode(char key) {
	struct node* node=(struct node*)malloc(sizeof(struct node));
	node->key=key;
	node->frequency=1;
	node->left=node->right=NULL;
	return (node);
}




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

struct node* modSplay(struct node* root, char key) {
    root = search(root, key);
    if (root->key == key) {
        root->frequency++;
    } else {
        struct node* new_node = newNode(key);
        if (root == NULL) {
            return new_node;
        } else if (root->key > key) {
            new_node->right = root;
            new_node->left = root->left;
            root->left = NULL;
        } else {
            new_node->left = root;
            new_node->right = root->right;
            root->right = NULL;
        }
        root = new_node;
    }

    if (root->left != NULL && root->left->frequency > root->frequency) {
        root = rightRotate(root);
    } else if (root->right != NULL && root->right->frequency > root->frequency) {
        root = leftRotate(root);
    }

    return root;
}
 

void preOrder(struct node* root, FILE *outputFile) {
    if (root != NULL) {
        fprintf(outputFile, "%c (%d) ", root->key, root->frequency);
        preOrder(root->left, outputFile);
        preOrder(root->right, outputFile);
    }
}


