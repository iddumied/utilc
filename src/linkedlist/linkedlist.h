#ifndef __D_ARRAY__H__
#define __D_ARRAY__H__

#include <stdlib.h>

typedef struct LinkedListElement LinkedListElement;
typedef struct LinkedList LinkedList;

struct {
    void * e;
    LinkedList * n;
    LinkedList * p;
} LinkedListElement;

struct {
    LinkedListElement * f;
    LinkedListElement * l;
    unsigned int len;
} LinkedList;

LinkedList * linkedlist(void(*) e);

/*
 * Get info about the LinkedList
 */
unsigned int ll_len( LinkedList * );
void (*) ll_last( LinkedList * );
void (*) ll_first( LinkedList * );

/*
 * Work with elements of the LinkedList 
 */
void ll_element( LinkedList *, unsigned int );
void (*) ll_pop( LinkedList* );
void ll_push( LinkedList * );  

/*
 * Remove elements or the LinkedList from memory
 */
void (*) ll_destroy_by_element( LinkedList *, LinkedListElement * );
void (*) ll_destroy_by_index( LinkedList *, unsigned int );
void ll_destroy( LinkedList * );

/*
 * Other functionality 
 */
LinkedList * ll_dump( LinkedList * );

/*
 * Sort the LinkedList
 */
LinkedList * ll_new_sorted( LinkedList * );
LinkedList * ll_sort( LinkedList * );

#endif
