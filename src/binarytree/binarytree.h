#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <stdlib.h>

struct bintree_elem{
    struct bintree_elem * left;
    struct bintree_elem * right;
    size_t datasize;
    char data[];
};

typedef struct bintree_elem BinaryTreeElement;

typedef struct {
    BinaryTreeElement *head;
} BinaryTree;

BinaryTree * empty_binary_tree(void);

#endif
