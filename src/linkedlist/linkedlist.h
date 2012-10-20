#ifndef __D_ARRAY__H__
#define __D_ARRAY__H__

#include <stdlib.h>
#include <string.h>

typedef struct linked_list_element  LinkedListElement;
typedef struct linked_list          LinkedList;

struct linked_list_element {
    LinkedListElement * next;
    LinkedListElement * prev;
    size_t datasize;
    char data[];
};

struct linked_list {
    LinkedListElement * first;
    LinkedListElement * last;
    unsigned int length;
};

LinkedList * linkedlist( void *data, size_t datasize );
LinkedList * empty_linkedlist(void);


/*
 * Get info about the LinkedList
 */
unsigned int ll_len( LinkedList *list, int force_recalc);
void * ll_last( LinkedList *list );
void * ll_first( LinkedList *list );

/*
 * Get datasizes
 */
size_t ll_datasize_last( LinkedList *list );
size_t ll_datasize_first( LinkedList *list );
size_t ll_datasize_by_index( LinkedList *list, unsigned int index);
size_t ll_datasize_list( LinkedList *list );


/*
 * Work with elements of the LinkedList 
 */
void * ll_element( LinkedList *list, unsigned int i);
void * ll_pop( LinkedList *list );
void ll_push( LinkedList *list, void *data, size_t datasize );  

/*
 * Remove elements or the LinkedList from memory
 */
void * ll_destroy_by_element( LinkedList *list, LinkedListElement *listelement );
void * ll_destroy_by_index( LinkedList *list, unsigned int i );
void ll_destroy( LinkedList *list );

/*
 * Other functionality 
 */
int ll_element_in_list( LinkedList *list, void *data, size_t datasize );
LinkedList * ll_dump( LinkedList *list );

/*
 * Get stuff as LinkedList from a LinkedList by condition
 */
LinkedList * ll_get_by_cond( LinkedList *list, int(*cnd)(void*, size_t) );

void ll_for_each_element_do( LinkedList *list, int (*func)(void*, size_t) );
void ll_for_each_element_by_condition_do( LinkedList *list, 
        int (*cond)(void*, size_t), int (*func)(void*, size_t) );

LinkedList * ll_join( LinkedList *list1, LinkedList *list2 );

/*
 * Print functionality. Only if debug flag is set!
 */
#ifdef LL_PRINTABLE
void ll_print(LinkedList *list, void (*print_element)(void*, size_t));
void ll_print_binary(LinkedList *list);
#endif //LL_PRINTABLE
#endif
