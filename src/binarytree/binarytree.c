#include "binarytree/binarytree.h"

/*
 * static function prototypes
 */
static BinaryTreeElement * new_binarytreeelement(void);


/*
 * Function implementations
 */
BinaryTree * empty_binary_tree() {
    BinaryTree * tree = (BinaryTree*) malloc( sizeof(BinaryTree) );
    return tree;
}


