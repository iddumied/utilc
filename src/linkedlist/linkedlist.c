#include "linkedlist/linkedlist.h"

static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );

LinkedList * linkedlist( void(*) e ) {
    LinkedListElement el = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    LinkedList l = (LinkedList*) malloc( sizeof( LinkedList ) );
    el->e = e;
    el->n = el->p = NULL;

    l->f = l->p = el;
    return l;
}

static LinkedListElement * linkedlistelement_at( LinkedList * l, unsigned int i ) {
    LinkedListElement * c = l->f;
    for( ; i != 0 ; i-- )
        if ( c->n )
            c = c->n;
        else
            return NULL;

    return c; 
}

static void savdeclen( LinkedList * l ) {
    if( l->len ) l->len--;
}

static void savinclen( LinkedList * l ) {
    if( l->len ) l->len++;
}

/*
 * Saves the calculated value in the LinkedList struct.
 * So when called first time, it calculates the len int
 * O(n)
 *
 * afterwards it will take O(1)
 *
 */
unsigned int ll_len( LinkedList * l ) {
    if( l->len ) 
        return l->len;

    unsigned int i = 0;
    LinkedListElement * c = l->f;
    while( c = c->n ) i++;
    l->len = i;
    return i;
}

/*
 * You could do this by your own in your code,... but why not?
 */
void (*) ll_last( LinkedList * l ) {
    return l->l->e;
}

/*
 * You could do this by your own in your code,... but why not?
 */
void (*) ll_first( LinkedList * l ) {
    return l->f->e;
}

void (*) ll_element( LinkedList * l , unsigned int i ) {
    LinkedListElement * e = linkedlistelement_at( l, i );
    if( e ) 
        return e->e;
    else
        return e; // e == NULL
}

void (*) ll_destroy_by_element( LinkedList * l, LinkedListElement * e ) {
    void (*) el;

    if ( !e )
        return NULL;

    if( e->p ) { // fix pointers of next and previous
        if ( e->n ) 
            e->p->n = e->n;
        else 
            e->p->n = NULL;
    }
    else { // fix first if destroyed was first
        l->f = e->n;
    }

    if( e->n ) {
        if( e->p )
            e->n->p = e->p; // if statements before this loc are redundant (#83,84)
        else
            e->n->p = NULL;
    }
    else { // fix last if destroyed was last
        l->l = e->p;
    }

    el = e->e; // save element value
    free( e );

    savdeclen(l);

    return el;
}

void (*) ll_destroy_by_index( LinkedList * l, unsigned int i ){
    LinkedListElement * e = linkedlistelement_at( l, i );
    return ll_destroy_by_element( l, e );
}  

void (*) ll_destroy( LinkedList * l ) {
    ll_len( l ); // just ensure the l->len value exists. 
    while ( l->len )
        ll_destroy_by_element( l, l->l );

    free( l );
} 

