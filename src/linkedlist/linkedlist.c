#include "linkedlist/linkedlist.h"

#ifdef DEBUG
#define LIBRARYNAME "ll"
#include "debug.h"
#endif


static LinkedListElement * new_linkedlistelement(size_t datasize);
static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
static void savdeclen( LinkedList * );
static void savinclen( LinkedList * );
static LinkedListElement * next(LinkedListElement*);
static LinkedListElement * previous(LinkedListElement*);

#ifdef LL_PRINTABLE
static void print_binary(LinkedListElement *lle);
#endif //LL_PRINTABLE

/*static LinkedList * quicksort( LinkedList*, signed int (*)(void(*), void(*)) );*/

LinkedList * linkedlist( void *data, size_t datasize ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    LinkedListElement *dataelement = new_linkedlistelement(datasize); 
    LinkedList *l = (LinkedList*) malloc( sizeof( LinkedList ) );

    if(l) { // if malloc failed, don't do something
        memcpy( dataelement->data, data, datasize);
        dataelement->datasize = datasize;
        dataelement->next = dataelement->prev = NULL;
        
        l->first = l->last = dataelement;
        l->length = 1;
    }
    return l;
}

LinkedList * empty_linkedlist() {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
    if (list) {
        list->first = list->last = NULL;
        list->length = 0;
    }
    return list;
}

static LinkedListElement* new_linkedlistelement(size_t datasize) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH("with size: %lu", datasize);
#endif

    LinkedListElement *ll_element = (LinkedListElement*) malloc(sizeof(LinkedListElement)+datasize);
    if (ll_element) {
        /*ll_element->data = NULL;*/
        ll_element->next = NULL;
        ll_element->prev = NULL;
        ll_element->datasize = datasize;
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
    EXPLAIN_FUNC_WITH("%i", i);
#endif

    LinkedListElement *current = list->first;
    unsigned int j;
    for(j = 0; j != i && current && (current = next(current)); j++);
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
    // see bottom of function
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
    EXPLAIN_FUNC_WITH(": %i", i);
#endif

    return i;
}

void * ll_last( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH(": %p",list->last->data);
#endif

    return list->last->data;
}

void * ll_first( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH(": %p",list->first->data);
#endif

    return list->first->data;
}

/*
 * Get datasize info
 */

size_t ll_datasize_last( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    return list->last->datasize;
}

size_t ll_datasize_first( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    return list->first->datasize;
}

size_t ll_datasize_by_index( LinkedList *list, unsigned int index ) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH(": index: %i", index);
#endif

    unsigned int i, len = ll_len(list, 1);
    LinkedListElement *curr;
    for( i = 0, curr = list->first; i < len && i != index; i++, (curr=next(curr)) );
    return curr->datasize;
}

/*
 * Calculates the size of the memory, used for the linkedlist stuff
 */
size_t ll_datasize_list( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    LinkedListElement *curr;
    unsigned int list_len = ll_len(list, 1);
    size_t result = 0;
    result += sizeof( LinkedList );
    result += sizeof( LinkedListElement ) * list_len;

    curr = list->first;
    result += curr->datasize;
    while( curr && (curr = next(curr)) ) {
        result += curr->datasize; 
    }
    return result;
}


/*
 * Work with elements in the List
 */

void * ll_element( LinkedList * l , unsigned int i ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement * listelement = linkedlistelement_at( l, i );
    if( listelement ) 
        return &(listelement->data);
    else
        return NULL; // e == NULL
}

void * ll_pop( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    return ll_destroy_by_element( list, list->first );
} 

void ll_push( LinkedList *list, void *data, size_t datasize ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement * element = new_linkedlistelement(datasize); 
    memcpy( element->data, data, datasize);

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
    EXPLAIN_FUNC; 
#endif
    
    void *data;
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
    else {
        list->first = list->last = NULL;
    }
    
    data = listelement->data; // save element value
    free( listelement );
    savdeclen(list);
    return data;
}

void * ll_destroy_by_index( LinkedList * list, unsigned int i ){
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    LinkedListElement * dataelement = linkedlistelement_at( list, i );
    return ll_destroy_by_element( list, dataelement );
}  

void ll_destroy( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
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

int ll_element_in_list( LinkedList *list, void *data, size_t datasize ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement * curr = list->first;
    int found = 0;

    while( !found && curr->next ) {
        found = memcmp( curr->data, data, datasize ) == 0;
        curr = next(curr);
    }
    return found;
}

LinkedList * ll_dump( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedList * newlist = linkedlist( list->first->data, list->first->datasize ); 
    LinkedListElement * c = list->first;
    while( (c = next(c)) ) // O(n)
        ll_push( newlist, c->data, c->datasize );

    return newlist;
}

LinkedList * ll_get_by_cond( LinkedList * list, int(*cnd)(void*, size_t) ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedList * newlist = empty_linkedlist();
    LinkedListElement *current = list->first;
    
    if( cnd(list->first->data, list->first->datasize) )
       ll_push(newlist, list->first->data, list->first->datasize);

    while( (current = next(current)) ) {
        if( cnd(current->data, current->datasize) )
            ll_push(newlist, current->data, current->datasize);
    } 

    return newlist;
}

/*
 * Executes for each element in the list the passed function, by passing the
 * element to the function.
 * If the function returns 0, the execution is completely stopped and 
 * there is no function call for any other element from the list.
 */
void ll_for_each_element_do( LinkedList * list, int (*func)(void*, size_t) ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    int go = func(list->first->data, list->first->datasize);
    LinkedListElement *current = list->first;
    while( (current = next(current)) && go ) {
        go = func(current->data, current->datasize);
    }

}

/*
 * Executes for each element the cond function. If the return value is 1, 
 * the func function is called with the value. So if the func function is very 
 * complex, the condition-function can control, which elements are processed
 * by the func function.
 * Anyway, if the func function returns 0, the whole process is aborted.
 */
void ll_for_each_element_by_condition_do( LinkedList *list, 
        int (*cond)(void*, size_t), int (*func)(void*, size_t) ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement *curr = list->first;
    int lastresult = 1;
    if ( cond(curr->data, curr->datasize) ) 
        lastresult = func(curr->data, curr->datasize); 

    while( lastresult && (curr = next(curr)) && curr ) {
        if(cond(curr->data, curr->datasize)) 
            lastresult = func(curr->data, curr->datasize);
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
    EXPLAIN_FUNC; 
#endif

    LinkedList * result = empty_linkedlist();

    LinkedListElement *current = list1->first;

    ll_push( result, current->data, current->datasize );
    while( current && (current = current->next)) {
        ll_push(result, current->data, current->datasize);
    }
    
    /* for second list, do the stuff again */
    current = list2->first;

    ll_push( result, current->data, current->datasize );
    while( current && (current = current->next)) {
        ll_push(result, current->data, current->datasize);
    }

    return result;
}

#ifdef LL_PRINTABLE
void ll_print( LinkedList *list, void (*print_element)(void*, size_t) ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif //DEBUG

    LinkedListElement *curr = list->first;
    print_element(list->first->data, list->first->datasize);
    while( (curr = next(curr)) ) 
        print_element(curr->data, curr->datasize);
}

void ll_print_binary(LinkedList *list) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement *curr = list->first;
    print_binary(curr);
    while( (curr=next(curr)) && curr ) {
        print_binary(curr);
    }
}

static void print_binary(LinkedListElement *lle) {
    unsigned char mask = 0x01;
    unsigned int ptr, bit;

    for(ptr = 0; ptr < lle->datasize; ptr++ ) {
        for(bit = 7; bit != 0 ; bit-- ) {
            if( (mask<<bit) & (unsigned char) *(lle->data+ptr) ) {
                printf("1");
            }
            else {
                printf("0");
            }
        }
        printf(" ");
        if( (ptr+1) % 8 == 0 ) 
            printf("\n");
    }
}
#endif //LL_PRINTABLE
