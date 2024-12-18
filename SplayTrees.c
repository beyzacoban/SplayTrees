/* Beyza Coban 150123069
   Beyza Parmak 150122007
   This program compares the performance of Splay Trees and Mod-Splay by calculating the number of comparisons, rotations, total cost, and their preorder traversal.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;  
    int frequency;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
}node;

/*Function Prototypes*/
void right_rotation(node **root, node *x);
void left_rotation(node **root, node *x);
void splay(node **root, node *n);
void insert(node **root, int key);
void rotateRight(node **root, node *x);
void rotateLeft(node **root, node *x);
void modsplay(node **root, node *n);
void modinsert(node **root, int key);
void preOrderSplay(node *root);
void preOrderMod(node *root);
node* search(node* root, int key);


int splay_comparisons = 0;
int splay_rotations = 0;
int modsplay_comparisons = 0;
int modsplay_rotations = 0;
int splay_total_cost=0;
int modsplay_total_cost=0;

/*Array that stores preorder statements to be written to the output file*/
char preOrderSplayArray[1000];  
char preOrderModArray[1000];    



/*Right rotation for tree*/ 
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
    
}

/*Left rotation for tree*/ 
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
    
}


/*Splay operation to move a node to the root*/ 
void splay(node **root, node *n) {
    while (n->parent != NULL) {
        node *parent = n->parent;
        node *grandparent = parent->parent;

        if (parent == *root) {
            if (n == parent->left) {
                right_rotation(root, parent);
                splay_rotations++;
            } else {
                left_rotation(root, parent);
                splay_rotations++;
            }
        } else {
            if (n == parent->left && parent == grandparent->left) {
                /*zig zig*/ 
                right_rotation(root, grandparent);
                right_rotation(root, parent);
                splay_rotations=splay_rotations+2;
            } else if (n == parent->right && parent == grandparent->right) {
                /*zag zag*/ 
                left_rotation(root, grandparent);
                left_rotation(root, parent);
                splay_rotations=splay_rotations+2;
            } else if (n == parent->left && parent == grandparent->right) {
                /*zig zag*/ 
                right_rotation(root, parent);
                left_rotation(root, grandparent);
                splay_rotations=splay_rotations+2;
            } else {
                /*zag zig*/ 
                left_rotation(root, parent);
                right_rotation(root, grandparent);
                splay_rotations=splay_rotations+2;
            }
        }
    }
}



/*Insert operation for Splay tree*/ 
void insert(node **root, int key) {
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

/*Mod-Splay operation: Move node with higher frequency to root*/ 
void modsplay(node **root, node *n) {
    while (n->parent != NULL) {
        node *parent = n->parent;
        node *grandparent = parent->parent;
        if (n->frequency > (*root)->frequency) {
            if (parent == *root) {
                if (n == parent->left) {
                    right_rotation(root, parent);
                    modsplay_rotations++;
                } else {
                    left_rotation(root, parent);
                    modsplay_rotations++;
                }
            } else {
                if (n == parent->left && parent == grandparent->left) {
                    right_rotation(root, grandparent);
                    right_rotation(root, parent);
                    modsplay_rotations = modsplay_rotations+2;
                } else if (n == parent->right && parent == grandparent->right) {
                    left_rotation(root, grandparent);
                    left_rotation(root, parent);
                    modsplay_rotations = modsplay_rotations+2;
                } else if (n == parent->left && parent == grandparent->right) {
                    right_rotation(root, parent);
                    left_rotation(root, grandparent);
                    modsplay_rotations = modsplay_rotations+2;
                } else {
                    left_rotation(root, parent);
                    right_rotation(root, grandparent);
                    modsplay_rotations = modsplay_rotations+2;
                }
            }
        } else {
            return; /*If frequency is not greater, do nothing*/ 
        }
    }
}
/*Insert operation for Mod-Splay tree*/ 
void modinsert(node **root, int key) {
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

/*Preorder traversal for Splay tree*/ 
void preOrderSplay(node *root) {
    if (root != NULL) {
        /*the array over which key values are written recursively */
        sprintf(preOrderSplayArray+ strlen(preOrderSplayArray),"%d, ",root->key );
        preOrderSplay(root->left);
        preOrderSplay(root->right);
    }
}

/*Preorder traversal for Mod-Splay tree*/ 
void preOrderMod(node *root) {
    if (root != NULL) {
        
        sprintf(preOrderModArray+ strlen(preOrderModArray),"(%d,%d), ", root->key, root->frequency);
        preOrderMod(root->left);
        preOrderMod(root->right);
    }
}

node* search(node* root, int key) {
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

    /*Read input from file*/ 
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        perror("Input file not opened");
        return 1;

    }

    int array[1000]; 
    char line[256];
    int count = 0;


while (fgets(line, sizeof(line), input_file)) {
    char *token = strtok(line, ",");
    int num = 0;
    while (token != NULL) {
        int i;
        for (i = 0; token[i] != '\0'; i++) {
        if (token[i] >= 48 && token[i] <= 57) {
            num = num * 10 + (token[i] - 48);
        }
        else {
            break;
        }
    }

        array[count++] = num; 
        token = strtok(NULL, ",");        
		num=0;

    }
}
    fclose(input_file);
    
    int i;
    /* Insert characters into Splay and Mod-Splay trees*/ 
    for (i = 0; i < count; i++) {
    	 if (array[i] == ',') {
        continue; 
    }
        insert(&splay_root, array[i]);
        modinsert(&modsplay_root, array[i]);
     
   modsplay_node = search(modsplay_root, array[i]);
    
    /*If the node is found, splay it; otherwise, insert it*/ 
    if (modsplay_node != NULL) {
        modsplay(&modsplay_root, modsplay_node);
    } else {
        modinsert(&modsplay_root, array[i]);
    }

    }

    /*Perform pre-order traversal and store result in arrays*/ 
    preOrderSplay(splay_root);
    preOrderMod(modsplay_root);

    /*Write results to the output file*/ 
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {

        printf("Output file could not be opened.\n");
        return 1;
    }

  
    int y;
    for (y = 0; y < count; y++) {
    fprintf(output_file, "%d ", array[y]);
}
    fprintf(output_file, "\n");

    /*Calculate total cost for both trees*/ 
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
