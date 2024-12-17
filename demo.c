#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char key;  
    int frequency;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

int splay_comparisons = 0;
int splay_rotations = 0;
int modsplay_comparisons = 0;
int modsplay_rotations = 0;
int splay_total_cost=0;
int modsplay_total_cost=0;

char preOrderSplayArray[1000] = "";  
char preOrderModArray[1000] = "";    

// Function to create a new node
struct Node* createNode(int key) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->frequency = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void rotateRight(struct Node **root, struct Node *x) {
    struct Node *y = x->left;
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

// Left rotation operation
void rotateLeft(struct Node **root, struct Node *x) {
    struct Node *y = x->right;
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

// Right rotation operation
void right_rotate(struct Node **root, struct Node *x) {
    struct Node *y = x->left;
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

// Left rotation operation
void left_rotate(struct Node **root, struct Node *x) {
    struct Node *y = x->right;
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

// Splay operation
void splay(struct Node **root, struct Node *n) {
    while (n->parent != NULL) {
        struct Node *parent = n->parent;
        struct Node *grandparent = parent->parent;

        if (parent == *root) {
            if (n == parent->left) {
                right_rotate(root, parent);
            } else {
                left_rotate(root, parent);
            }
        } else {
            if (n == parent->left && parent == grandparent->left) {
                right_rotate(root, grandparent);
                right_rotate(root, parent);
            } else if (n == parent->right && parent == grandparent->right) {
                left_rotate(root, grandparent);
                left_rotate(root, parent);
            } else if (n == parent->left && parent == grandparent->right) {
                right_rotate(root, parent);
                left_rotate(root, grandparent);
            } else {
                left_rotate(root, parent);
                right_rotate(root, grandparent);
            }
        }
    }
}

// Insert operation for Splay tree
void insert(struct Node **root, char key) {
    struct Node *temp = *root;
    struct Node *y = NULL;
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    n->key = key;
    n->frequency = 1;
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

// Mod-Splay operation: Move the node with greater frequency to root
void modsplay(struct Node **root, struct Node *n) {
    while (n->parent != NULL) {
        struct Node *parent = n->parent;
        struct Node *grandparent = parent->parent;
        // modsplaydaki splay operationlarý if (node->frequency >root->frequency ) nin içine alýp else durumunda geri dönmek
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

void modinsert(struct Node **root, char key) {
    struct Node *temp = *root;
    struct Node *y = NULL;
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
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

// Preorder traversal for printing
void preOrderSplay(struct Node *root) {
    if (root != NULL) {
        sprintf(preOrderSplayArray+ strlen(preOrderSplayArray),"%c, ", root->key);
        preOrderSplay(root->left);
        preOrderSplay(root->right);
    }
}
//mod splay icin ayri bir preOrder methodu acildi cunku dosyay yazdrima islemi icin kolaylik sagliyor.
void preOrderMod(struct Node *root) {
    if (root != NULL) {
        sprintf(preOrderModArray+ strlen(preOrderModArray),"%c(%d) ", root->key, root->frequency);
        preOrderMod(root->left);
        preOrderMod(root->right);
    }
}
struct Node* search(struct Node* root, char key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}



int main() {
    struct Node *splay_root = NULL;
    struct Node *modsplay_root = NULL;
    struct Node *modsplay_node = NULL; 

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
        continue; // Virgülü atla
    }
        insert(&splay_root, array[i]);
    }

    // Perform pre-order traversal and store result in arrays
    preOrderSplay(splay_root);

 // Mod-Splay Tree adjustments
    for (i = 0; i < strlen(array) - 1; i++) {
    	 if (array[i] == ',') {
        continue; // Virgülü atla
    }
          modinsert(&modsplay_root, array[i]);
     
    modsplay_node = search(modsplay_root, array[i]);
    
    // If the node is found, splay it; otherwise, insert it
    if (modsplay_node != NULL) {
        modsplay(&modsplay_root, modsplay_node);
    } else {
        modinsert(&modsplay_root, array[i]);
    }
}
     preOrderMod(modsplay_root);

    // Write results to the output file
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Output file could not be opened.\n");
        return 1;
    }

    // Write input content to output file
    fprintf(output_file, "Input:\n%s\n\n", array);

    // Calculate total cost for both trees
    int splay_total_cost = splay_comparisons + splay_rotations;
    int modsplay_total_cost = modsplay_comparisons + modsplay_rotations;
    fprintf(output_file, "Splay Tree Preorder:\n%s\n", preOrderSplayArray);
    fprintf(output_file, "Mod-Splay Tree Preorder:\n%s\n", preOrderModArray);
    fprintf(output_file, "\nPerformance Comparison:\n");
    fprintf(output_file, " %-20s %-15s %-15s %-15s\n", "Tree Type", "Comparisons", "Rotations", "Cost");
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Splay", splay_comparisons, splay_rotations, splay_total_cost);
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Mod-Splay", modsplay_comparisons, modsplay_rotations, modsplay_total_cost);

    fclose(output_file);
    return 0;
}
