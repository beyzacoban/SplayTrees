#include <stdio.h>
#include <stdlib.h>

// Node structure for the Modified Splay Tree
typedef struct Node {
    int key;
    int frequency;
    struct Node *left, *right;
} Node;

// Global variable to track the total cost
int total_cost = 0;

// Utility function to create a new node
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->frequency = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Right rotation
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// Left rotation
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splay operation
Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key) {
        if (root->left == NULL) return root;

        if (key < root->left->key) { // Zig-Zig (Left Left)
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
            total_cost += 2; // Zig-Zig rotation cost
        } else if (key > root->left->key) { // Zig-Zag (Left Right)
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
            total_cost += 2; // Zig-Zag rotation cost
        }

        return (root->left == NULL) ? root : rightRotate(root);
    } else {
        if (root->right == NULL) return root;

        if (key > root->right->key) { // Zig-Zig (Right Right)
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
            total_cost += 2; // Zig-Zig rotation cost
        } else if (key < root->right->key) { // Zig-Zag (Right Left)
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
            total_cost += 2; // Zig-Zag rotation cost
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

// Insert operation
Node* insert(Node* root, int key) {
    if (root == NULL) {
        total_cost += 0; // No comparisons for the first insertion
        return createNode(key);
    }

    root = splay(root, key);

    if (root->key == key) {
        root->frequency++;
        return root;
    }

    Node* newNode = createNode(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    total_cost += 1; // Comparison cost
    return newNode;
}

// Search operation
Node* search(Node* root, int key) {
    if (root == NULL)
        return NULL;

    root = splay(root, key);

    if (root->key == key) {
        root->frequency++;
        total_cost += 1; // Successful comparison
        return root;
    }

    total_cost += 1; // Unsuccessful comparison
    return root;
}

// Inorder traversal
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("Key: %d, Frequency: %d\n", root->key, root->frequency);
    inorder(root->right);
}

// Main function
int main() {
    Node* root = NULL;

    // Example operations
    root = insert(root, 7);
    root = insert(root, 8);
    root = insert(root, 4);
   root = insert(root, 6);
    //root = insert(root, 2);
    //root = insert(root, 5);
    //root = insert(root, 3);
    //root = insert(root, 9);

    //root = search(root, 3);
   // root = search(root, 5);
    root = search(root, 4);

    printf("Inorder Traversal:\n");
    inorder(root);

    printf("\nTotal cost: %d\n", total_cost);

    return 0;
}
