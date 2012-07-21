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
unsigned long long int ll_len( LinkedList * );
void (*) ll_last( LinkedList * );
void (*) ll_first( LinkedList * );

void ll_destroy( LinkedList * );

void ll_element( LinkedList*, unsigned int );

void (*) ll_destroy_by_element( LinkedList *, LinkedListElement * );
void (*) ll_destroy_by_index( LinkedList *, unsigned int );
void ll_destroy( LinkedList * );

void ll_push( LinkedList *, void(*) ); // l->len has to be set += 1 only if it was set
void (*) ll_pop( LinkedList * );  // l->len -= 1 onyl if it was set
LinkedList * ll_dump( LinkedList * );
LinkedList * ll_new_sorted( LinkedList * );
LinkedList * ll_sort( LinkedList * );

#endif
