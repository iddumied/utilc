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
    if (!list->length) ll_len(list, true);
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
unsigned int ll_len( LinkedList * list, bool force_recalc ) {
    if( list->length && !force_recalc ) 
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
    savinclen(list);
}

/*
 * Remove elements or the LinkedList from memory
 */

void * ll_destroy_by_element( LinkedList * list, LinkedListElement * listelement ) {
    void *el;

    if ( !listelement )
        return NULL;

    if ( !( list->first == listelement && list->last == listelement ) ) {
        // listelement is NOT the only element in the list
        if ( list->first == listelement ) {
            // listelement is first
            list->first->next->prev = NULL; // before 2nd there will be nothing
            list->first = listelement->next;
        }
        else if ( list->last == listelement ) {
            // listelement is last
            list->last->prev->next = NULL; // after 2nd-last there will be nothing
            list->last = listelement->prev;
        }
        else {
            // listelement is inside the list 
            listelement->prev->next = listelement->next;
            listelement->next->prev = listelement->prev;
        }
    }
    // else {}
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
    LinkedListElement * curr = list->first;

    while( curr->next ) {
        curr = curr->next;
        ll_destroy_by_element( list, curr->prev );
    }
    ll_destroy_by_element( list, list->last );
    free( list );
} 

/*
 * Other functionality
 */

bool ll_element_in_list( LinkedList * list, void * el ) {
    LinkedListElement * curr = list->first;
    bool found = false;

    while( !found ) {
        found = curr->e == el;
        curr = curr->next;
    }
    return found;
}

LinkedList * ll_dump( LinkedList *list ) {
    LinkedList * new = linkedlist( list->first->e ); 
    LinkedListElement * c = list->first;
    while( c = c->next ) // O(n)
        ll_push( new, c->e );

    return new;
}

LinkedList * ll_sort( LinkedList * list, signed int (*cmpfunc)( void *a, void *b ) ) {
    LinkedList * sorted;
    if( !list->length ) ll_len(list, false);
    /*if( list->length > 10 )*/ // currently, there is no other sorting algo.
        sorted = quicksort(list, cmpfunc );

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

LinkedList * ll_get_by_cond( LinkedList * list, int(*cnd)(void*) ) {
    LinkedList * new;
    LinkedListElement * c = list->first;
    while(c) {
        if( cnd(c->e) ) {
            if(!new) {
                new = linkedlist(c->e);
            }
            ll_push(new, c->e);
        }
        c = c->next;
    }
    return new;
}

/*
 * Executes for each element in the list the passed function, by passing the
 * element to the function.
 * If the function returns false, the execution is completely stopped and 
 * there is no function call for any other element from the list.
 */
void ll_for_each_element_do( LinkedList * list, bool (*func)(void*) ) {
    LinkedListElement *curr = list->first;
    bool lastresult = true; 
    while(lastresult) {
        lastresult = func(curr->e);
        curr = curr->next; 
    }
}

/*
 * Executes for each element the cond function. If the return value is true, 
 * the func function is called with the value. So if the func function is very 
 * complex, the condition-function can control, which elements are processed
 * by the func function.
 * Anyway, if the func function returns false, the whole process is aborted.
 */
void ll_for_each_element_by_condition_do( LinkedList * list, bool (*cond)(void*), bool (*func)(void*) ) {
    LinkedListElement *curr = list->first;
    bool lastresult = true; 
    while(lastresult) {
        if( cond(curr->e) ) lastresult = func(curr->e);
        curr = curr->next;
    }
}

/*
 * Easy join function. 
 * The ll_join function joins two LinkedLists together by allocating mem for a 
 * new LinkedList and running through every element of the passed lists and 
 * appending them. 
 * So the expense of this function is O(n^2)
 *
 * Notice: The function does not take every value one time. It really just joins
 * two lists.
 */
LinkedList * ll_join( LinkedList *list1, LinkedList *list2 ) {
    LinkedList * result = linkedlist(list1->first->e);

    LinkedListElement *curr = list1->first;
    while( curr = curr->next ) {
        ll_push( result, curr->e );
    }
    curr = list2->first;
    ll_push( result, list2->first->e );
    while( curr = curr->next ) {
        ll_push( result, curr->e );
    }
    return result;
}
