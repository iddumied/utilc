#include "linkedlist/linkedlist.h"

#ifdef DEBUG
#include <stdio.h>
#endif

static LinkedListElement * new_linkedlistelement();
static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );
static LinkedListElement * next(LinkedListElement*);
static LinkedListElement * previous(LinkedListElement*);

/*static LinkedList * quicksort( LinkedList*, signed int (*)(void(*), void(*)) );*/

LinkedList * linkedlist( void *e ) {
#ifdef DEBUG
    printf("::ll : linkedlist created\n");
#endif
    LinkedListElement *el = new_linkedlistelement(); 
    LinkedList *l = (LinkedList*) malloc( sizeof( LinkedList ) );

    if(l) { // if malloc failed, don't do something
        el->e = e;
        el->next = el->prev = NULL;
        
        l->first = l->last = el;
        l->length = 1;
    }
    return l;
}

LinkedList * empty_linkedlist() {
#ifdef DEBUG
    printf("::ll : linkedlist created (empty)\n");
#endif
    LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
    if (list) {
        list->first = list->last = NULL;
        list->length = 0;
    }
    return list;
}

static LinkedListElement* new_linkedlistelement() {
#ifdef DEBUG
    printf("::ll : linkedlist element created\n");
#endif
    LinkedListElement *ll_element = (LinkedListElement*) malloc(sizeof(LinkedListElement));
    if (ll_element) {
        ll_element->e = NULL;
        ll_element->next = NULL;
        ll_element->prev = NULL;
    }
    return ll_element;
}

/*
 * This function is NOT redundant with the ll_element function!
 * This one returns a LinkedListElement and the ll_element function returns the
 * void* to the real element!
 */
static LinkedListElement * linkedlistelement_at( LinkedList * list, unsigned int i ) {
#ifdef DEBUG
    printf("::ll : linkedlist_element_at %i\n", i);
#endif
    LinkedListElement *current = list->first;
    int j;
    for(j = 0; j != i && current && (current = next(current)); j++) {

    }
    if ( j != i ) return NULL;
    return current;
}

static void savdeclen( LinkedList * list ) {
    if( list->length ) list->length--;
}

static void savinclen( LinkedList * list ) {
    if( list->length ) list->length++;
}

static LinkedListElement * next(LinkedListElement *curr) {
    return curr->next;
}

static LinkedListElement * previous(LinkedListElement *curr) {
    return curr->prev;
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
unsigned int ll_len( LinkedList * list, int force_recalc ) {
#ifdef DEBUG
    printf("::ll : ll_len");
#endif
    unsigned int i;
    LinkedListElement *c;

    if( list->length && !force_recalc ) 
        return list->length;

    i = 0;
    c = list->first;

    if(c) i++;
    while(c && (c = next(c)) ) i++;
    list->length = i;

#ifdef DEBUG
    printf(" %i\n",i);
#endif

    return i;
}

void * ll_last( LinkedList * list ) {
#ifdef DEBUG
    printf("::ll : ll_last: %p\n",list->last->e);
#endif
    return list->last->e;
}

void * ll_first( LinkedList * list ) {
#ifdef DEBUG
    printf("::ll : ll_first: %p\n",list->first->e);
#endif
    return list->first->e;
}

/*
 * Work with elements in the List
 */

void * ll_element( LinkedList * l , unsigned int i ) {
#ifdef DEBUG
    printf("::ll : ll_element\n");
#endif
    LinkedListElement * listelement = linkedlistelement_at( l, i );
    if( listelement ) 
        return listelement->e;
    else
        return NULL; // e == NULL
}

void * ll_pop( LinkedList * list ) {
#ifdef DEBUG
    printf("::ll : ll_pop\n");
#endif
    return ll_destroy_by_element( list, list->first );
} 

void ll_push( LinkedList *list, void *e ) {
#ifdef DEBUG
    printf("::ll : ll_push\n");
#endif
    LinkedListElement * element = new_linkedlistelement(); 
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
#ifdef DEBUG
    printf("::ll : ll_destroy_by_element\n");
#endif
    void *el;

    if ( !listelement )
        return NULL;

    if ( !( list->first == listelement && list->last == listelement ) ) {
        // listelement is NOT the only element in the list
        if ( list->first == listelement ) {
            // listelement is first
            list->first->next->prev = NULL; // before 2nd there will be nothing
            list->first = next(listelement);
        }
        else if ( list->last == listelement ) {
            // listelement is last
            list->last->prev->next = NULL; // after 2nd-last there will be nothing
            list->last = previous(listelement);
        }
        else {
            // listelement is inside the list 
            listelement->prev->next = next(listelement);
            listelement->next->prev = previous(listelement);
        }
    }
    // else {}
    el = listelement->e; // save element value
    free( listelement );
    savdeclen(list);
    return el;
}

void * ll_destroy_by_index( LinkedList * list, unsigned int i ){
#ifdef DEBUG
    printf("::ll : ll_destroy_by_index\n");
#endif
    LinkedListElement * e = linkedlistelement_at( list, i );
    return ll_destroy_by_element( list, e );
}  

void ll_destroy( LinkedList * list ) {
#ifdef DEBUG
    printf("::ll : ll_destroy\n");
#endif
    LinkedListElement * curr = list->first;

    while( curr->next ) {
        curr = next(curr);
        ll_destroy_by_element( list, previous(curr) );
    }
    ll_destroy_by_element( list, list->last );
    free( list );
} 

/*
 * Other functionality
 */

int ll_element_in_list( LinkedList *list, void *el ) {
#ifdef DEBUG
    printf("::ll : ll_element_in_list\n");
#endif
    LinkedListElement * curr = list->first;
    int found = 0;

    while( !found && curr->next ) {
        found = curr->e == el;
        curr = next(curr);
    }
    return found;
}

LinkedList * ll_dump( LinkedList *list ) {
#ifdef DEBUG
    printf("::ll : ll_dump\n");
#endif
    LinkedList * newlist = linkedlist( list->first->e ); 
    LinkedListElement * c = list->first;
    while( c = next(c) ) // O(n)
        ll_push( newlist, c->e );

    return newlist;
}

/*
 * No sorting in Version 1.
 * Sorting will be implemented for version 2 or so.
 */

/*
LinkedList * ll_sort( LinkedList * list, signed int (*cmpfunc)( void *a, void *b ) ) {
#ifdef DEBUG
    printf("::ll : ll_sort\n");
#endif
    LinkedList * sorted;
    if( !list->length ) ll_len(list, 0);
    if( list->length > 10 ) // currently, there is no other sorting algo.
        sorted = quicksort(list, cmpfunc );

    return sorted;
}
*/

/*
static LinkedList * quicksort( LinkedList * list, signed int (*cmpfunc)( void* a, void* b ) ) {
#ifdef DEBUG
    printf("::ll : quicksort\n");
#endif
    if( list->first == list->last ) return list;

    LinkedListElement * pivot = list->first;
    LinkedListElement * curr = list->last;

    signed int cmp;
    void *stash;

    while( curr != pivot ) {
        cmp = (*cmpfunc)( pivot->e, curr->e );
        curr = curr->next;
        if( cmp > 0 ) { //  curr->previous is > than pivot, bring it on right side
            stash = ll_destroy_by_element( list, previous(curr) );
            ll_push( list, stash ); 
        }
    }

    if( previous(pivot) ) {
        // generate a new LinkedList for the left side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * left = (LinkedList*) malloc( sizeof( LinkedList ) );
        left->last = list->last;
        left->first = previous(pivot);
        left->first->next = NULL;
        
        left = quicksort( left, cmpfunc );

        left->first->next = pivot;
        pivot->prev = left->first;

    }

    if( next(pivot) ) {
        // generate a new LinkedList for the right side, sort it with qs and merge
        // it with this LinkedList
        //
        // This is done by breaking the chain into pieces and use the pieces as
        // new LinkedList. Then sort it with quicksort and pack it into the old
        // LinkedList by repairing the pointers.
        //
        LinkedList * right = (LinkedList*) malloc( sizeof( LinkedList ) );
        right->last = next(pivot);
        right->last->prev = NULL;
        right->first = list->first;

        right = quicksort( right, cmpfunc );

        right->last->prev = pivot;
        pivot->next = right->last;
        
    }

    return list;
}
*/

LinkedList * ll_get_by_cond( LinkedList * list, int(*cnd)(void*) ) {
#ifdef DEBUG
    printf("::ll : ll_get_by_cond\n");
#endif
    LinkedList * newlist = empty_linkedlist();
    LinkedListElement *current = list->first;
    
    if( cnd(list->first->e ) )
       ll_push(newlist, list->first->e);

    while( current = current->next ) {
        if( cnd(current->e ) )
            ll_push(newlist, current->e);
    } 

    return newlist;
}

/*
 * Executes for each element in the list the passed function, by passing the
 * element to the function.
 * If the function returns 0, the execution is completely stopped and 
 * there is no function call for any other element from the list.
 */
void ll_for_each_element_do( LinkedList * list, int (*func)(void*) ) {
#ifdef DEBUG
    printf("::ll : ll_for_each_element_do\n");
#endif
    int __continue = func(list->first->e);
    LinkedListElement *current = list->first;
    while( (current = current->next) && __continue ) {
        __continue = func(current->e);
    }

}

/*
 * Executes for each element the cond function. If the return value is 1, 
 * the func function is called with the value. So if the func function is very 
 * complex, the condition-function can control, which elements are processed
 * by the func function.
 * Anyway, if the func function returns 0, the whole process is aborted.
 */
void ll_for_each_element_by_condition_do( LinkedList *list, int (*cond)(void*), int (*func)(void*) ) {
#ifdef DEBUG
    printf("::ll : ll_for_each_element_by_condition_do\n");
#endif
    LinkedListElement *curr = list->first;
    int lastresult = 1;
    if ( cond(curr->e) ) lastresult = func(curr->e); 
    while( lastresult && (curr = curr->next) && curr ) {
        if(cond(curr->e)) lastresult = func(curr->e);
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
#ifdef DEBUG
    printf("::ll : ll_join\n");
#endif
    LinkedList * result = empty_linkedlist();

    LinkedListElement *current = list1->first;

    ll_push( result, current->e );
    while( current && (current = current->next)) {
        ll_push(result, current->e);
    }
    
    /* for second list, do the stuff again */
    current = list2->first;

    ll_push( result, current->e );
    while( current && (current = current->next)) {
        ll_push(result, current->e);
    }

    return result;
}
