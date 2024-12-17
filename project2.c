/* Beyza Coban 150123069
   Beyza Parmak 
   This program compares the performance of Splay Trees and Mod-Splay by calculating the number of comparisons, rotations, total cost, and their preorder traversal.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char key;  
    int frequency;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
}node;

// Function Prototypes
node* createNode(int key);
void right_rotation(node **root, node *x);
void left_rotation(node **root, node *x);
void splay(node **root, node *n);
void insert(node **root, char key);
void rotateRight(node **root, node *x);
void rotateLeft(node **root, node *x);
void modsplay(node **root, node *n);
void modinsert(node **root, char key);
void preOrderSplay(node *root);
void preOrderMod(node *root);
node* search(node* root, char key);


int splay_comparisons = 0;
int splay_rotations = 0;
int modsplay_comparisons = 0;
int modsplay_rotations = 0;
int splay_total_cost=0;
int modsplay_total_cost=0;

char preOrderSplayArray[1000] = "";  
char preOrderModArray[1000] = "";    

// Function to create a new node
node* createNode(int key) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->key = key;
    newNode->frequency = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Right rotation for Mod-Splay tree
void rotateRight(node **root, node *x) {
    node *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x == *root) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    modsplay_rotations++;
}

// Left rotation for Mod-Splay tree
void rotateLeft(node **root, node *x) {
    node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x == *root) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    modsplay_rotations++;
}

// Right rotation for Splay tree
void right_rotation (node **root, node *x) {
    node *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x == *root) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
    splay_rotations++;
}

// Left rotation for Splay tree
void left_rotation(node **root, node *x) {
    node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x == *root) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    splay_rotations++;
}

// Splay operation to move a node to the root
void splay(node **root, node *n) {
    while (n->parent != NULL) {
        node *parent = n->parent;
        node *grandparent = parent->parent;

        if (parent == *root) {
        	//zig
            if (n == parent->left) {
                right_rotation(root, parent);
            } else {
                left_rotation(root, parent);
            }
        } else {
            if (n == parent->left && parent == grandparent->left) {
                right_rotation(root, grandparent);
                right_rotation(root, parent);
            } else if (n == parent->right && parent == grandparent->right) {
                left_rotation(root, grandparent);
                left_rotation(root, parent);
            } else if (n == parent->left && parent == grandparent->right) {
                right_rotation(root, parent);
                left_rotation(root, grandparent);
            } else {
                left_rotation(root, parent);
                right_rotation(root, grandparent);
            }
        }
    }
}

// Insert operation for Splay tree
void insert(node **root, char key) {
    node *temp = *root;
    node *y = NULL;
    node *n = (node *)malloc(sizeof(node));
    n->key = key;
    n->frequency = 0;
    n->left = NULL;
    n->right = NULL;

    while (temp != NULL) {
        y = temp;
        splay_comparisons++;
        if (key < temp->key) {
            temp = temp->left;
        } else if (key > temp->key) {
            temp = temp->right;
        } else {
            temp->frequency++;
            splay(root, temp);
            return;
        }
    }
 
    n->parent = y;

    if (y == NULL) {
        *root = n;
    } else if (key < y->key) {
        y->left = n;
    } else {
        y->right = n;
    }

    splay(root, n);
}

// Mod-Splay operation: Move node with higher frequency to root
void modsplay(node **root, node *n) {
    while (n->parent != NULL) {
        node *parent = n->parent;
        node *grandparent = parent->parent;
        if (n->frequency > (*root)->frequency) {
            if (parent == *root) {
                if (n == parent->left) {
                    rotateRight(root, parent);
                } else {
                    rotateLeft(root, parent);
                }
            } else {
                if (n == parent->left && parent == grandparent->left) {
                    rotateRight(root, grandparent);
                    rotateRight(root, parent);
                } else if (n == parent->right && parent == grandparent->right) {
                    rotateLeft(root, grandparent);
                    rotateLeft(root, parent);
                } else if (n == parent->left && parent == grandparent->right) {
                    rotateRight(root, parent);
                    rotateLeft(root, grandparent);
                } else {
                    rotateLeft(root, parent);
                    rotateRight(root, grandparent);
                }
            }
        } else {
            return; // If frequency is not greater, do nothing
        }
    }
}
// Insert operation for Mod-Splay tree
void modinsert(node **root, char key) {
    node *temp = *root;
    node *y = NULL;
    node *n = (node *)malloc(sizeof(node));
    n->key = key;
    n->frequency = 0;
    n->left = NULL;
    n->right = NULL;

    while (temp != NULL) {
        y = temp;
        modsplay_comparisons++;
        if (key < temp->key) {
            temp = temp->left;
        } else if (key > temp->key) {
            temp = temp->right;
        } else {
            temp->frequency++;
            modsplay(root, temp);
            return;
        }
    }

    n->parent = y;

    if (y == NULL) {
        *root = n;
    } else if (key < y->key) {
        y->left = n;
    } else {
        y->right = n;
    }

    modsplay(root, n);
}

// Preorder traversal for Splay tree
void preOrderSplay(node *root) {
    if (root != NULL) {
        sprintf(preOrderSplayArray+ strlen(preOrderSplayArray),"%c, ", root->key);
        preOrderSplay(root->left);
        preOrderSplay(root->right);
    }
}

// Preorder traversal for Mod-Splay tree
void preOrderMod(node *root) {
    if (root != NULL) {
        sprintf(preOrderModArray+ strlen(preOrderModArray),"(%c,%d), ", root->key, root->frequency);
        preOrderMod(root->left);
        preOrderMod(root->right);
    }
}

node* search(node* root, char key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}



int main() {
    node *splay_root = NULL;
    node *modsplay_root = NULL;
    node *modsplay_node = NULL; 

    // Read input from file
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        perror("Input file not opened");
        return 1;
    }

    char array[1000]; // Input array for characters
    int i = 0;
    while (fscanf(input_file, "%c", &array[i]) != EOF) {
        i++;
    }
    array[i] = '\0'; // Null terminate the string
    fclose(input_file);

    // Insert characters into Splay and Mod-Splay trees
    for (i = 0; i < strlen(array)-1; i++) {
    	 if (array[i] == ',') {
        continue; 
    }
        insert(&splay_root, array[i]);
        modinsert(&modsplay_root, array[i]);
     
   modsplay_node = search(modsplay_root, array[i]);
    
    // If the node is found, splay it; otherwise, insert it
    if (modsplay_node != NULL) {
        modsplay(&modsplay_root, modsplay_node);
    } else {
        modinsert(&modsplay_root, array[i]);
    }
    }

    // Perform pre-order traversal and store result in arrays
    preOrderSplay(splay_root);
    preOrderMod(modsplay_root);

    // Write results to the output file
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Output file could not be opened.\n");
        return 1;
    }

    // Write input content to output file
    fprintf(output_file, "Input:\n%s\n", array);

    // Calculate total cost for both trees
    int splay_total_cost = splay_comparisons + splay_rotations;
    int modsplay_total_cost = modsplay_comparisons + modsplay_rotations;
    fprintf(output_file, "Splay Tree Preorder:\n%s\n\n", preOrderSplayArray);
    fprintf(output_file, "Mod-Splay Tree Preorder:\n%s\n\n", preOrderModArray);
    fprintf(output_file, " %-20s %-15s %-15s %-15s\n", "Tree Type", "Comparisons", "Rotations", "Cost");
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Splay", splay_comparisons, splay_rotations, splay_total_cost);
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Mod-Splay", modsplay_comparisons, modsplay_rotations, modsplay_total_cost);

    fclose(output_file);
    return 0;
}
