#include "linkedlist/linkedlist.h"

static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );
static LinkedList * quicksort( LinkedList*, signed int (*)(void(*), void(*)) );

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
unsigned int ll_len( LinkedList * l ) {
    if( l->len ) 
        return l->len;

    unsigned int i = 0;
    LinkedListElement * c = l->f;
    while( c = c->n ) i++;
    l->len = i;
    return i;
}

void (*) ll_last( LinkedList * l ) {
    return l->l->e;
}

void (*) ll_first( LinkedList * l ) {
    return l->f->e;
}

/*
 * Work with elements in the List
 */

void (*) ll_element( LinkedList * l , unsigned int i ) {
    LinkedListElement * e = linkedlistelement_at( l, i );
    if( e ) 
        return e->e;
    else
        return e; // e == NULL
}

void (*) ll_pop( LinkedList * l ) {
    return ll_destroy_by_element( l, l->f );
} 

void ll_push( LinkedList * l, void (*) e ) {
    LinkedListElement * element = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    if ( l->l ) {
        element->e = e;
        element->p = l->l;
        l->l->n = element;
        l->l = element;
    }
    else {
        l->f = element;
        l->l = element;
    }
    l->len++;
}

/*
 * Remove elements or the LinkedList from memory
 */

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

/*
 * Other functionality
 */

LinkedList * ll_dump( LinkedList * l ) {
    LinkedList * new = linkedlist( l->f->e ); 
    LinkedListElement * c = l->f;
    while( c = c->n ) // O(n)
        ll_push( new, c->e );
    return new;
}

LinkedList * ll_sort( LinkedList * l, signed int (*cmpfunc)( void* a, void* b ) ) {
    LinkedList * sorted;
    if( !l->len ) ll_len(l);
    if( l->len > 10 )
        sorted = quicksort(l, cmpfunc );
}

static LinkedList * quicksort( LinkedList * l, signed int (*cmpfunc)( void* a, void* b ) ) {
    if( l->r == l->l ) return l;

    LinkedListElement * pivot = l->r;
    LinkedListElement * curr = l->l;

    signed int cmp;
    void (*) stash;

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
        LinkedList * left;
        left->l = l->l;
        left->r = pivot->p;
        left->r->n = NULL;
        
        left = quicksort( left, cmpfunc );

        left->r->n = pivot;

    }

    if( pivot->n ) {
        // generate a new LinkedList for the right side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * right;
        right->l = pivot->n;
        right->l->p = NULL;
        right->r = l->r;

        right = quicksort( right, cmpfunc );

        right->r->p = pivot;
        
    }

    return l;
}

LinkedList * get_by_cond( LinkedList * l, int(*cnd)(void*) ) {
    LinkedList new;
    LinkedListElement * c = l->f;

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
