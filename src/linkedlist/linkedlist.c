#include "linkedlist/linkedlist.h"

static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );
static LinkedList * quicksort( LinkedList*, signed int (*)(void(*), void(*)) );

LinkedList * linkedlist( void *e ) {
    LinkedListElement *el = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    LinkedList *l = (LinkedList*) malloc( sizeof( LinkedList ) );
    el->e = e;
    el->n = el->p = NULL;

    l->f = l->l = el;
    return l;
}

static LinkedListElement * linkedlistelement_at( LinkedList * l, unsigned int i ) {
    LinkedListElement * c = l->f;
    for( ; i != 0 ; i-- )
        if ( c->n )
            c = c->n;
        else {
            i = 0;
            c = NULL;
        }

    return c; 
}

static void savdeclen( LinkedList * l ) {
    if( l->len ) l->len--;
}

static void savinclen( LinkedList * l ) {
    if( l->len ) l->len++;
}

/*
 * Get info about the LinkedList
 */

/*
 * Saves the calculated value in the LinkedList struct.
 * So when called first time, it calculates the len int
 * O(n)
 *
 * afterwards it will take O(1)
 *
 */
unsigned int ll_len( LinkedList * list ) {
    if( list->len ) 
        return list->len;

    unsigned int i = 0;
    LinkedListElement * c = list->f;
    while( c = c->n ) i++;
    list->len = i;
    return i;
}

void * ll_last( LinkedList * list ) {
    return list->l->e;
}

void * ll_first( LinkedList * list ) {
    return list->f->e;
}

/*
 * Work with elements in the List
 */

void * ll_element( LinkedList * l , unsigned int i ) {
    LinkedListElement * listelement = linkedlistelement_at( l, i );
    if( listelement ) 
        return listelement->e;
    else
        return NULL; // e == NULL
}

void * ll_pop( LinkedList * list ) {
    return ll_destroy_by_element( list, list->f );
} 

void ll_push( LinkedList * list, void * e ) {
    LinkedListElement * element = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    element->e = e;
    if ( list->l ) {
        element->p = list->l;
        list->l->n = element;
    }
    else {
        list->f = element;
    }
    list->l = element;
    if( list->len ) list->len++;
}

/*
 * Remove elements or the LinkedList from memory
 */

void * ll_destroy_by_element( LinkedList * list, LinkedListElement * e ) {
    void *el;

    if ( !e )
        return NULL;

    if( e->p ) { // fix pointers of next and previous
        if ( e->n ) 
            e->p->n = e->n;
        else 
            e->p->n = NULL;
    }
    else { // fix first if destroyed was first
        list->f = e->n;
    }

    if( e->n ) {
        if( e->p )
            e->n->p = e->p; // if statements before this loc are redundant (#83,84)
        else
            e->n->p = NULL;
    }
    else { // fix last if destroyed was last
        list->l = e->p;
    }

    el = e->e; // save element value
    free( e );

    savdeclen(list);

    return el;
}

void * ll_destroy_by_index( LinkedList * list, unsigned int i ){
    LinkedListElement * e = linkedlistelement_at( list, i );
    return ll_destroy_by_element( list, e );
}  

void ll_destroy( LinkedList * list ) {
    if( !list->len ) ll_len( list ); // just ensure the l->len value exists. 
    while ( list->len )
        ll_destroy_by_element( list, list->l );

    free( list );
} 

/*
 * Other functionality
 */

LinkedList * ll_dump( LinkedList *list ) {
    LinkedList * new = linkedlist( list->f->e ); 
    LinkedListElement * c = list->f;
    while( c = c->n ) // O(n)
        ll_push( new, c->e );

    return new;
}

LinkedList * ll_sort( LinkedList * list, signed int (*cmpfunc)( void *a, void *b ) ) {
    LinkedList * sorted;
    if( !list->len ) ll_len(list);
    if( list->len > 10 )
        sorted = quicksort(list, cmpfunc );

    return sorted;
}

static LinkedList * quicksort( LinkedList * l, signed int (*cmpfunc)( void* a, void* b ) ) {
    if( l->f == l->l ) return l;

    LinkedListElement * pivot = l->f;
    LinkedListElement * curr = l->l;

    signed int cmp;
    void *stash;

    while( curr != pivot ) {
        cmp = (*cmpfunc)( pivot->e, curr->e );
        curr = curr->n;
        if( cmp > 0 ) { //  curr->previous is > than pivot, bring it on right side
            stash = ll_destroy_by_element( l, curr->p );
            ll_push( l, stash ); 
        }
    }

    if( pivot->p ) {
        // generate a new LinkedList for the left side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * left = (LinkedList*) malloc( sizeof( LinkedList ) );
        left->l = l->l;
        left->f = pivot->p;
        left->f->n = NULL;
        
        left = quicksort( left, cmpfunc );

        left->f->n = pivot;
        pivot->p = left->f;

    }

    if( pivot->n ) {
        // generate a new LinkedList for the right side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * right = (LinkedList*) malloc( sizeof( LinkedList ) );
        right->l = pivot->n;
        right->l->p = NULL;
        right->f = l->f;

        right = quicksort( right, cmpfunc );

        right->l->p = pivot;
        pivot->n = right->l;
        
    }

    return l;
}

LinkedList * get_by_cond( LinkedList * l, int(*cnd)(void*) ) {
    LinkedList *new;
    LinkedListElement *c = l->f;

    if( (cnd)(l->f->e) )
        new = linkedlist(l->f->e);
    else
        new = NULL;

    while( c = c->n ) {
        if( (cnd)(c->e) ) {
            if( new == NULL ) 
                new = linkedlist(c->e);
            else
                ll_push(new, c->e);
        }
    }
    return new;
}
