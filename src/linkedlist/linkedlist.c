#include "linkedlist/linkedlist.h"

static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );
static LinkedList * quicksort( LinkedList*, signed int (*)(void(*), void(*)) );

LinkedList * linkedlist( void *e ) {
    LinkedListElement *el = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    LinkedList *l = (LinkedList*) malloc( sizeof( LinkedList ) );
    el->e = e;
    el->next = el->prev = NULL;

    l->first = l->last = el;
    return l;
}

/*
 * This function is NOT redundant with the ll_element function!
 * This one returns a LinkedListElement and the ll_element function returns the
 * void* to the real element!
 */
static LinkedListElement * linkedlistelement_at( LinkedList * list, unsigned int i ) {
    LinkedListElement * c = list->first;
    unsigned int j;
    if (!list->length) ll_len(list);
    for( j = 0 ; j < list->length && j != i  ; j++ ) {
        if ( c->next ) c = c->next; 
        else {
            j = i; // abort the iteration the easy way.
            c = NULL; // but ensure there is NULLtype returned
        }
    }

    return c; 
}

static void savdeclen( LinkedList * list ) {
    if( list->length ) list->length--;
}

static void savinclen( LinkedList * list ) {
    if( list->length ) list->length++;
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
    if( list->length ) 
        return list->length;

    unsigned int i = 0;
    LinkedListElement * c = list->first;
    while( c = c->next ) i++;
    list->length = i;
    return i;
}

void * ll_last( LinkedList * list ) {
    return list->last->e;
}

void * ll_first( LinkedList * list ) {
    return list->first->e;
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
    return ll_destroy_by_element( list, list->first );
} 

void ll_push( LinkedList * list, void * e ) {
    LinkedListElement * element = (LinkedListElement*) malloc( sizeof( LinkedListElement ) );
    element->e = e;
    if ( list->last ) {
        element->prev = list->last;
        list->last->next = element;
    }
    else {
        list->first = element;
    }
    list->last = element;
    if( list->length ) list->length++;
}

/*
 * Remove elements or the LinkedList from memory
 */

void * ll_destroy_by_element( LinkedList * list, LinkedListElement * listelement ) {
    void *el;

    if ( !listelement )
        return NULL;

    if( listelement->prev ) { // fix pointers of next and previous
        if ( listelement->next ) 
            listelement->prev->next = listelement->next;
        else 
            listelement->prev->next = NULL;
    }
    else { // fix first if destroyed was first
        list->first = listelement->next;
    }

    if( listelement->next ) {
        if( listelement->prev )
            listelement->next->prev = listelement->prev; // if statements before this loc are redundant (#83,84)
        else
            listelement->next->prev = NULL;
    }
    else { // fix last if destroyed was last
        list->last = listelement->prev;
    }

    el = listelement->e; // save element value
    free( listelement );

    savdeclen(list);

    return el;
}

void * ll_destroy_by_index( LinkedList * list, unsigned int i ){
    LinkedListElement * e = linkedlistelement_at( list, i );
    return ll_destroy_by_element( list, e );
}  

void ll_destroy( LinkedList * list ) {
    if( !list->length ) ll_len( list ); // just ensure the l->len value exists. 
    while ( list->length )
        ll_destroy_by_element( list, list->last );

    free( list );
} 

/*
 * Other functionality
 */

LinkedList * ll_dump( LinkedList *list ) {
    LinkedList * new = linkedlist( list->first->e ); 
    LinkedListElement * c = list->first;
    while( c = c->next ) // O(n)
        ll_push( new, c->e );

    return new;
}

LinkedList * ll_sort( LinkedList * list, signed int (*cmpfunc)( void *a, void *b ) ) {
    LinkedList * sorted;
    if( !list->length ) ll_len(list);
    if( list->length > 10 ) sorted = quicksort(list, cmpfunc );

    return sorted;
}

static LinkedList * quicksort( LinkedList * list, signed int (*cmpfunc)( void* a, void* b ) ) {
    if( list->first == list->last ) return list;

    LinkedListElement * pivot = list->first;
    LinkedListElement * curr = list->last;

    signed int cmp;
    void *stash;

    while( curr != pivot ) {
        cmp = (*cmpfunc)( pivot->e, curr->e );
        curr = curr->next;
        if( cmp > 0 ) { //  curr->previous is > than pivot, bring it on right side
            stash = ll_destroy_by_element( list, curr->prev );
            ll_push( list, stash ); 
        }
    }

    if( pivot->prev ) {
        // generate a new LinkedList for the left side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * left = (LinkedList*) malloc( sizeof( LinkedList ) );
        left->last = list->last;
        left->first = pivot->prev;
        left->first->next = NULL;
        
        left = quicksort( left, cmpfunc );

        left->first->next = pivot;
        pivot->prev = left->first;

    }

    if( pivot->next ) {
        // generate a new LinkedList for the right side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * right = (LinkedList*) malloc( sizeof( LinkedList ) );
        right->last = pivot->next;
        right->last->prev = NULL;
        right->first = list->first;

        right = quicksort( right, cmpfunc );

        right->last->prev = pivot;
        pivot->next = right->last;
        
    }

    return list;
}

LinkedList * get_by_cond( LinkedList * list, int(*cnd)(void*) ) {
    LinkedList *new;
    LinkedListElement *c = list->first;

    if( (cnd)(list->first->e) )
        new = linkedlist(list->first->e);
    else
        new = NULL;

    while( c = c->next ) {
        if( (cnd)(c->e) ) {
            if( new == NULL ) 
                new = linkedlist(c->e);
            else
                ll_push(new, c->e);
        }
    }
    return new;
}
