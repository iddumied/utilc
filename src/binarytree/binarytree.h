#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <stdlib.h>
#include <string.h>

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

BinaryTree * empty_binarytree(void);

void binarytree_insert(
        BinaryTree *tree, 
        void *data, 
        size_t datasize,
        signed int (*cmp)(
            void *data1, 
            size_t datasize1, 
            void *data2, 
            size_t datasize2
        ) );
#endif
