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

// Function to create a new node
struct Node* createNode(int key) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->frequency = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to perform a right rotation
struct Node* rotateRight(struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;
    x->right = y;
    modsplay_rotations += 1; // Zig rotation cost
    return x;
}

// Function to perform a left rotation
struct Node* rotateLeft(struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    y->left = x;
    modsplay_rotations += 1; // Zig rotation cost
    return y;
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

// Splay function to move the most frequent node to the root
struct Node* modsplay(struct Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    // Key is in the left subtree
    if (key < root->key) {
        modsplay_comparisons++;
        if (root->left == NULL)
            return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            modsplay_comparisons++;
            root->left->left = modsplay(root->left->left, key);
            root = rotateRight(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            modsplay_comparisons++;
            root->left->right = modsplay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = rotateLeft(root->left);
        }

        return (root->left == NULL) ? root : rotateRight(root);
    }
    // Key is in the right subtree
    else {
        modsplay_comparisons++;
        if (root->right == NULL)
            return root;

        // Zig-Zig (Right Right)
        if (key > root->right->key) {
            modsplay_comparisons++;
            root->right->right = modsplay(root->right->right, key);
            root = rotateLeft(root);
        }
        // Zig-Zag (Right Left)
        else if (key < root->right->key) {
            modsplay_comparisons++;
            root->right->left = modsplay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rotateRight(root->right);
        }

        return (root->right == NULL) ? root : rotateLeft(root);
    }
}

// Insert a key into the BST
struct Node* modinsert(struct Node* root, int key) {
    if (root == NULL)
        return createNode(key);

    modsplay_comparisons++;
    if (key < root->key)
        root->left = modinsert(root->left, key);
    else if (key > root->key)
        root->right = modinsert(root->right, key);

    return root;
}



// Find the node with the maximum frequency
struct Node* findMaxFrequency(struct Node* root, struct Node** maxNode) {
    if (root == NULL)
        return NULL;

    if (*maxNode == NULL || root->frequency > (*maxNode)->frequency)
        *maxNode = root;

    findMaxFrequency(root->left, maxNode);
    findMaxFrequency(root->right, maxNode);

    return *maxNode;
}

/*
// Search for a key in the BST and update frequency
struct Node* search(struct Node* root, char key) {
    if (root == NULL || root->key == key) {
        if (root != NULL) root->frequency++;
        return root;
    }

    modsplay_comparisons++;
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}
*/

// Find the node with the maximum frequency


// Pre-order traversal for Mod-Splay Tree
char preOrderModArray[1000] = ""; // Initialize as empty string
void preOrderMod(struct Node *root) {
    if (root != NULL) {
        char buffer[20];
        sprintf(buffer, "%c(%d) ", root->key, root->frequency);
        strcat(preOrderModArray, buffer);
        preOrderMod(root->left);
        preOrderMod(root->right);
    }
}
//mod splay icin ayri bir preOrder methodu acildi cunku dosyay yazdrima islemi icin kolaylik sagliyor.

// Pre-order traversal for Splay Tree
char preOrderSplayArray[1000] = ""; // Initialize as empty string
void preOrderSplay(struct Node *root) {
    if (root != NULL) {
        char buffer[20];
        sprintf(buffer, "%c(%d) ", root->key, root->frequency);
        strcat(preOrderSplayArray, buffer);
        preOrderSplay(root->left);
        preOrderSplay(root->right);
    }
}

int main() {
    struct Node *splay_root = NULL;
    struct Node *modsplay_root = NULL;

    // Open the input file
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Input file could not be opened.\n");
        return 1;
    }

    // Read data from the input file (ignoring commas)
    char array[1000];
    int i = 0;
    char ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch != ',') {  // Ignore commas
            array[i++] = ch;
        }
    }
    array[i] = '\0';  // Null-terminate the string

    fclose(input_file);

    // Insert elements into both trees
    for (i = 0; i < strlen(array); i++) {
        insert(&splay_root, array[i]);
        modsplay_root = modinsert(modsplay_root, array[i]);
    }

    // Perform preorder traversal and store result in the arrays
    preOrderSplay(splay_root);
    preOrderMod(modsplay_root);

    int splay_total_cost = splay_comparisons + splay_rotations;
    int modsplay_total_cost = modsplay_comparisons + modsplay_rotations;

    // Output results to file
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Output file could not be opened.\n");
        return 1;
    }

    // Output Splay and Mod-Splay Trees and performance comparison
    fprintf(output_file, "Splay Tree Preorder:\n%s\n", preOrderSplayArray);
    fprintf(output_file, "Mod-Splay Tree Preorder:\n%s\n", preOrderModArray);
    fprintf(output_file, "\nPerformance Comparison:\n");
    fprintf(output_file, " %-20s %-15s %-15s %-15s\n", "Tree Type", "Comparisons", "Rotations", "Cost");
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Splay", splay_comparisons, splay_rotations, splay_total_cost);
    fprintf(output_file, " %-20s %-15d %-15d %-15d\n", "Mod-Splay", modsplay_comparisons, modsplay_rotations, modsplay_total_cost);

    fclose(output_file);
    return 0;
}
