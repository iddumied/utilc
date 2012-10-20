#include "binarytree/binarytree.h"

/*
 * static function prototypes
 */
static BinaryTreeElement * new_binarytreeelement(size_t datasize);


/*
 * Function implementations
 */
BinaryTree * empty_binarytree() {
    BinaryTree * tree = (BinaryTree*) malloc( sizeof(BinaryTree) );
    if(tree) {
        tree->head = NULL;
    }
    return tree;
}

/*
 * static functions implementations
 */
static BinaryTreeElement * new_binarytreeelement(size_t datasize) {
    BinaryTreeElement *elem = (BinaryTreeElement*) malloc(sizeof(BinaryTreeElement)+datasize);
    if(elem) {
        elem->left = NULL;
        elem->right= NULL;
        elem->datasize = datasize;
    }
    return elem;
}

