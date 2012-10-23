#include "binarytree/binarytree.h"

/*
 * static function prototypes
 */
static BinaryTreeElement * new_binarytreeelement(size_t datasize);
static void insert(
        BinaryTreeElement *te, 
        void *data, 
        size_t datasize, 
        signed int (*cmp)(
            void *data1, 
            size_t datasize1, 
            void *data2, 
            size_t datasize2
            ) 
        );

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

static void insert(
        BinaryTreeElement *te, 
        void *data, 
        size_t datasize, 
        signed int (*cmp)(
            void *data1, 
            size_t datasize1, 
            void *data2, 
            size_t datasize2
            ) 
        ) {

    /*
     * cmp function should return
     *
     * -1 if first is bigger
     *  0 if equal (no insert)
     *  1 if second is bigger
     */
    signed int compare = cmp(te, te->datasize, data, datasize);

    if ( compare < 0 ) {
        if ( te->left ) {
            insert(te->left, data, datasize, cmp );
        }
        else { // !te->left
            te->left = new_binarytreeelement(datasize);
            memcpy( te->left->data, data, datasize );
        }
    }
    else if ( compare == 0 ) {
        // do exactly nothing
    }
    else { // if compare > 0
        if ( te->right ) {
            insert(te->right, data, datasize, cmp );
        }
        else {
            te->right = new_binarytreeelement(datasize);
            memcpy( te->right->data, data, datasize );
        }
    }


}

/*
 * Function implementations
 */
BinaryTree * empty_binarytree() {
    BinaryTree * tree = (BinaryTree*) malloc( sizeof(BinaryTree) );
    if(tree) {
        tree->root = NULL;
    }
    return tree;
}

void bt_insert( 
        BinaryTree *tree, 
        void *data, 
        size_t datasize, 
        signed int (*cmp)(
            void *data1, 
            size_t datasize1, 
            void *data2, 
            size_t datasize2
            ) 
        ) {

    insert(tree->root, data, datasize, cmp);
}

size_t bt_root_datasize(BinaryTree *bt) {
    return bt->root->datasize;
}

void * bt_get_root_data(BinaryTree *bt) {
    return bt->root->data;
}
