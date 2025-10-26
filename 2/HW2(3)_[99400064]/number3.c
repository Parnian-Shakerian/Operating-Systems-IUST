#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define NUM_CHILDREN 3

struct Node {
    int value;
    struct Node* children[NUM_CHILDREN];
};

struct Node* create_node(int value) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->value = value;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

bool search_tree(struct Node* node, int value, int path[], int level) {
    if (node == NULL) {
        return false;
    }
    path[level] = node->value;
    if (node->value == value) {
        return true;
    }
    bool found = false;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (node->children[i] != NULL) {
            if (search_tree(node->children[i], value, path, level + 1)) {
                found = true;
                break;
            }
        }
    }
    if (!found) {
        path[level] = -1;
    }
    return found;
}

int main() {
    struct Node* root = create_node(19);
    root->children[0] = create_node(5);
    root->children[0]->children[0] = create_node(3);
    root->children[0]->children[1] = create_node(4);
    root->children[1] = create_node(6);
    root->children[1]->children[0] = create_node(2);
    root->children[1]->children[1] = create_node(11);

    int path[NUM_CHILDREN];
    int value = 2;
    if (search_tree(root, value, path, 0)) {
        printf("Path to node with value %d:", value);
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (path[i] != -1) {
                printf(" %d->", path[i]);
            } else {
                break;
            }
        }
        printf("found\n");
    } else {
        printf("Value not found\n");
    }

    return 0;
}
