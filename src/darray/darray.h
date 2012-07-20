#ifndef __D_ARRAY__H__
#define __D_ARRAY__H__

#include <stdlib.h>

typedef struct DArrayElement DArrayElement;
typedef struct DArray DArray;

struct {
    void * e;
    DArray * n;
    DArray * p;
} DArrayElement;

struct {
    DArrayElement * f;
    DArrayElement * l;
    unsigned int len;
} DArray;

DArray * darray(void(*) e);
unsigned long long int da_len( DArray * );
void (*) da_last( DArray * );
void (*) da_first( DArray * );

void da_destroy( DArray * );

void da_element( DArray*, unsigned int );

void (*) da_destroy_by_element( DArray *, DArrayElement * );
void (*) da_destroy_by_index( DArray *, unsigned int );
void da_destroy( DArray * );

void da_push( DArray *, void(*) ); // a->len has to be set += 1 only if it was set
void (*) da_pop( DArray * );  // a->len -= 1 onyl if it was set
DArray * da_dump( DArray * );
DArray * da_new_sorted( DArray * );
DArray * da_sort( DArray * );

#endif
