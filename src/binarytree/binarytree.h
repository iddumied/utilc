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
    BinaryTreeElement *root;
} BinaryTree;

BinaryTree * empty_binarytree(void);

#define BT_CMP_FUNC signed int (*cmp)(void *d1, size_t ds1, void *d2, size_t ds2)
void bt_insert(
        BinaryTree *tree, 
        void *data, 
        size_t datasize,
        BT_CMP_FUNC
        );

size_t bt_head_datasize(BinaryTree *bt);
void * bt_get_root_data(BinaryTree *bt);

#endif
