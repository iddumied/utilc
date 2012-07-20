#include "darray/darray.h"

static DArrayElement * darrayelement_at( DArray *, unsigned int );

DArray * darray( void(*) e ) {
    DArrayElement el = (DArrayElement*) malloc( sizeof( DArrayElement ) );
    DArray a = (DArray*) malloc( sizeof( DArray ) );
    el->e = e;
    el->n = el->p = NULL;

    a->f = a->p = el;
    return a;
}

static DArrayElement * darrayelement_at( DArray * a, unsigned int i ) {
    DArrayElement * c = a->f;
    for( ; i != 0 ; i-- )
        if ( c->n )
            c = c->n;
        else
            return NULL;

    return c; 
}

/*
 * Saves the calculated value in the DArray struct.
 * So when called first time, it calculates the len int
 * O(n)
 *
 * afterwards it will take O(1)
 *
 */
unsigned int da_len( DArray * a ) {
    if( a->len ) 
        return a->len;

    unsigned int i = 0;
    DArrayElement * c = a->f;
    while( c = c->n ) i++;
    a->len = i;
    return i;
}

void (*) da_element( DArray * a , unsigned int i ) {
    DArrayElement * e = darrayelement_at( a, i );
    if( e ) 
        return e->e;
    else
        return e; // e == NULL
}

void (*) da_destroy_by_element( DArray * a, DArrayElement * e ) {
    void (*) el;

    if ( !e )
        return NULL;

    if( e->p && e->n ) { // fix pointers of next and previous
        e->n->p = e->p;
        e->p->n = e->n;
    }

    if( !e->n ) // fix last if destroyed was last
        a->l = e->p;

    if( !e->p ) // fix first if destroyed was first
        a->f = e->n;

    el = e->e; // save element value
    free( e );

    if( a->len ) a->len--;

    return el;
}

void (*) da_destroy_by_index( DArray * a, unsigned int i ){
    DArrayElement * e = darrayelement_at( a, i );
    return da_destroy_by_element( a, e );
}  

void (*) da_destroy( DArray * a ) {
    da_len( a ); // just ensure the a->len value exists. 
    while ( a->len )
        da_destroy_by_element( a, a->l );

    free( a );
} 

