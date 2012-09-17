#ifndef __D_ARRAY__H__
#define __D_ARRAY__H__

#include <stdlib.h>

typedef struct linked_list_element  LinkedListElement;
typedef struct linked_list          LinkedList;

struct linked_list_element {
    void * e;
    LinkedListElement * next;
    LinkedListElement * prev;
};

struct linked_list {
    LinkedListElement * first;
    LinkedListElement * last;
    unsigned int length;
};

LinkedList * linkedlist( void(*) );

/*
 * Get info about the LinkedList
 */
unsigned int ll_len( LinkedList * );
void * ll_last( LinkedList * );
void * ll_first( LinkedList * );

/*
 * Work with elements of the LinkedList 
 */
void * ll_element( LinkedList *, unsigned int );
void * ll_pop( LinkedList* );
void ll_push( LinkedList *, void (*) );  

/*
 * Remove elements or the LinkedList from memory
 */
void * ll_destroy_by_element( LinkedList *, LinkedListElement * );
void * ll_destroy_by_index( LinkedList *, unsigned int );
void ll_destroy( LinkedList * );

/*
 * Other functionality 
 */
LinkedList * ll_dump( LinkedList * );

/*
 * Sort the LinkedList
 */
LinkedList * ll_sort( LinkedList *, signed int (*)(void*,void*) );

/*
 * Get stuff as LinkedList from a LinkedList by condition
 */
LinkedList * get_by_cond( LinkedList * l, int(*cnd)(void*) );

#endif
