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

// Mod-Splay operation
void mod_splay(struct Node **root, struct Node *n) {
    struct Node *temp = *root;
    while (temp != NULL) {
        modsplay_comparisons++;
        if (temp->frequency < n->frequency) {
            splay(root, n);
            modsplay_rotations++;
            break;
        }
        temp = temp->left ? temp->left : temp->right;
    }
}
// Insert operation for Mod-Splay tree
void mod_insert(struct Node **root, char key) {
    struct Node *temp = *root;
    struct Node *y = NULL;
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    n->key = key;
    n->frequency = 1;
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
            mod_splay(root, temp);
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

    mod_splay(root, n);
}
char preOrderSplayArray[100];
char preOrderModArray[100];
// Preorder traversal for printing
void preOrderSplay(struct Node *root) {
    if (root != NULL) {
        sprintf(preOrderSplayArray+ strlen(preOrderSplayArray),"%c(%d) ", root->key, root->frequency);
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


int main() {
    struct Node *splay_root = NULL;
    struct Node *modsplay_root = NULL;
    FILE *input_file = fopen("input.txt", "r");
        if(input_file ==NULL){
            perror("the file is not opened");
            return 1;
        }
    
    char array[100]; //input array
    int i;
    while(!feof(input_file)){
    size_t lenght_read = fread(array,sizeof(char),sizeof(array)-1,input_file);
    array[lenght_read]= '\0';
    }
    fclose(input_file);
    for (i = 0; i < strlen(array); i++) {
        insert(&splay_root, array[i]);
        mod_insert(&modsplay_root, array[i]);
    }

    
    //printf("Performance Comparison:\n");
    //printf("%-20s %-15s %-15s %-15s\n", "Tree Type", "Comparisons", "Rotations","Cost");
    //printf("%-20s %-15d %-15d %-15d\n", "Splay", splay_comparisons, splay_rotations, splay_total_cost);
    //printf("%-20s %-15d %-15d %-15d\n", "Mod-Splay", modsplay_comparisons, modsplay_rotations, modsplay_total_cost);
    
    
    FILE *output_file = fopen("output.txt","w");
     if (output_file == NULL) {
        printf("Dosya açılamadı.\n");
        }
    
    preOrderSplay(splay_root);
    preOrderMod(modsplay_root);
    
    int splay_total_cost = splay_comparisons + splay_rotations;
    int modsplay_total_cost = modsplay_comparisons + modsplay_rotations ;
    
    fprintf(output_file,"Splay Tree Preorder: \n" "%s", preOrderSplayArray);
    fprintf(output_file,"\nMod-Splay Tree Preorder: \n" "%s", preOrderModArray);
    fprintf(output_file,
        "\nPerformance Comparison:\n"
        " %-20s %-15s %-15s %-15s\n" " %-20s %-15d %-15d %-15d\n" " %-20s %-15d %-15d %-15d\n",
        "Tree Type", "Comparisons", "Rotations", "Cost",  "Splay", splay_comparisons, splay_rotations, splay_total_cost, "Mod-Splay", modsplay_comparisons, modsplay_rotations, modsplay_total_cost);
    fclose(output_file);
    return 0;
}

