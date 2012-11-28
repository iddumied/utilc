#include "linkedlist/linkedlist.h"

#ifdef DEBUG
#define LIBRARYNAME "ll"
#include "debug.h"
#endif

/*
 * ==========================
 * Static function prototypes 
 * ==========================
 */
static LinkedListElement * new_linkedlistelement(size_t datasize);
static LinkedListElement * linkedlistelement_at( LinkedList *, unsigned int );
#define savinclen(l) if(l->length) l->length++
#define savdeclen(l) if(l->length) l->length--
#define next(c) c->next
#define previous(c) c->prev

#ifdef LL_PRINTABLE
static void print_binary(LinkedListElement *lle);
#endif //LL_PRINTABLE

/* 
 * ===============================
 * Static function implementations
 * ===============================
 */

/*
 * returns a new LinkedListElement*,
 * sets the variables inside to NULL if allocating was successfull.
 *
 * @return LinkedListElement*
 * @param datasize size of the data stored inside
 */
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
 *
 * @return LinkedListElement*
 * @param list the linkedlist to search int
 * @param i position of the element to get
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

#ifdef LL_PRINTABLE

/*
 * prints the data from the LinkedListElement which is passed bitwise.
 * 1 for bit is set
 * 0 for bit is not sets
 *
 * @param lle the LinkedListElement to print the data
 */
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


/*
 * ========================
 * Function implementations
 * ========================
 */

/*
 * Returns a new LinkedList with a first entry in it
 *
 * @return LinkedList* 
 * @param data data for the first element
 * @param datasize datasize of the first element
 */
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

/*
 * returns a new LinkedList without data
 * 
 * @return LinkedList*
 */
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

/*
 * -----------------------------
 * Get info about the LinkedList
 * -----------------------------
 */

/*
 * Saves the calculated value in the LinkedList struct.
 * So when called first time, it calculates the len int
 * O(n)
 *
 * afterwards it will take O(1)
 *
 * @return unsigned int
 * @param list get the length for this list
 * @param force_recalc if false, it will take O(1), but maybe untrustable,
 *  if true, the length is calculated (again), takes O(n)
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

/*
 * returns a pointer to the last elements data
 *
 * @return void*
 * @param list the list to get the last element data from
 */
void * ll_last( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH(": %p",list->last->data);
#endif

    return list->last->data;
}

/*
 * returns a pointer to the first elements data
 *
 * @return void*
 * @param list the list to get the first element data from
 */
void * ll_first( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC_WITH(": %p",list->first->data);
#endif

    return list->first->data;
}

/*
 * -----------------
 * Get datasize info
 * -----------------
 */

/*
 * get the datasize of the last element
 *
 * @return size_t
 * @param list the list to get the last elements data from
 */
size_t ll_datasize_last( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    return list->last->datasize;
}

/*
 * get the datasize of the first element
 *
 * @return size_t
 * @param list the list to get the first elements data from
 */
size_t ll_datasize_first( LinkedList *list ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    return list->first->datasize;
}

/*
 * returns the datasize of the element, the index points to
 *
 * @return size_t
 * @param list the list to search int
 * @param index the index for the element
 */
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
 *
 * @return size_t
 * @param list the list to calculate the memuse for
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
 * ------------------------------
 * Work with elements in the List
 * ------------------------------
 */

/*
 * returns the data of the i'th element
 *
 * @return void*
 * @param l the list to search int
 * @param i the index for the element
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

/*
 * pop the first element from the list and return it's data
 *
 * @return void*
 * @param list the list to pop from
 */
void * ll_pop( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    return ll_destroy_by_element( list, list->first );
} 

/*
 * push some data to the list
 *
 * @param list the list to push in 
 * @param data the data to push into the list
 * @param datasize the datasize of the data to push
 */
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
 * ---------------------------------------------
 * Remove elements or the LinkedList from memory
 * ---------------------------------------------
 */

/*
 * destroy a specific element in the linkedlist, return it's data
 * This function the pointer-fix automatically.
 *
 * @return void*
 * @param list the list to destroy the element in
 * @param listelement the element which should be destroyed
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

/*
 * Destroy an element from the list by index and returns its data.
 * This is kind of a wrapper, it simply uses the internal function
 * linkedlistelement_at and ll_destroy_by_element.
 *
 * @return void*
 * @param list the list to destroy int
 * @param i the index of the element which should be destroyed
 */
void * ll_destroy_by_index( LinkedList * list, unsigned int i ){
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif

    LinkedListElement * dataelement = linkedlistelement_at( list, i );
    return ll_destroy_by_element( list, dataelement );
}  

/*
 * Destroy the complete linkedlist and remove all data from memory
 *
 * @param list the list to destroy
 */
void ll_destroy( LinkedList * list ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif

    LinkedListElement * curr = list->first;
    LinkedListElement * next_element;

    while( curr ) {
        next_element = next(curr);
        ll_destroy_by_element(list, curr);
        curr = next_element;
    }
    free( list );
} 

/*
 * -------------------
 * Other functionality
 * -------------------
 */

/*
 * Copy the linkedlist and return the new one.
 * -> Does not change the passed list
 *
 * @return LinkedList*
 * @param list the "old" linkedlist
 */
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

/*
 * Easy join function. 
 * The ll_join function joins two LinkedLists together by allocating mem for a 
 * new LinkedList and running through every element of the passed lists and 
 * appending them. 
 * So the expense of this function is O(n^2)
 * -> Does not change the passed lists
 *
 * Notice: The function does not take every value one time. It really just joins
 * two lists.
 *
 * @return LinkedList*
 * @param list1 the first list
 * @param list2 the second list
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


/*
 *
 * You can define this macro if you want to have some more functionality in the 
 * linkedlist library. If not, this will not be compiled and will not use any 
 * memory on your machine.
 */
#ifdef LL_EXTENDED


/*
 * A checker if specific data is already in the linkedlist.
 *
 * @return int : 1 if data is in the linkedlist, else 0
 * @param list the list to search int
 * @param data a pointer to the data which should be checked
 * @param datasize the size of the data which should be checked
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

/*
 * get some data from the linkedlist in a new linkedlist based on a condition
 * -> Does not change the passed list
 *
 * @return LinkedList*
 * @param list the list to search in
 * @param cnd the condition-function. It gets a void* to the data, and a size_t 
 *  which is the size of the data. Based on this, it should do some checking and
 *  return a value not 0 if the data should be in the new list, else 0.
 */
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
 * This functions limits the length of the list to 'len'. The elements which are 
 * not in the range from element 0 to len-1 will be completely removed from mem.
 *
 * @param list the list which should be limited
 * @param len the length which the list should have afterwards
 * @param take_start if set to true, the first 'len' entries will be kept, else
 * the last 'len' entries will be kept
 */
void ll_limit( LinkedList *list, unsigned int len, int take_start ) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif 

    unsigned int currlen = ll_len( list, 1 );
    unsigned int to_remove, i;
    if ( currlen > len ) {
        to_remove = currlen - len;
        for( i = 0 ; i < to_remove ; i++ ) {
            if( take_start ) 
                ll_destroy_by_element( list, list->last );
            else
                ll_destroy_by_element( list, list->first );
        }
    }
    else {
        /* nothing to do! */
    }
}

/*
 * Executes for each element in the list the passed function, by passing the
 * element to the func function.
 * -> Does not change the passed list
 *
 * @param list the list to iterate through 
 * @param func the function which should be executed for each element. This 
 *  function gets a void* to the data and size_t, which is the size of the data.
 *  If the func function returns zero, the execution of this is stopped. If it
 *  returns a value != 0, it continues.
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
 * Executes the func function for each element in the list, the cond function 
 * returns a value != 0.
 * -> Does not change the passed list
 *
 * @param list the list to iterate through
 * @param cond the condition-function
 * @param func the function to execute
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

#endif // LL_EXTENDED

#ifdef LL_PRINTABLE
/*
 * prints the list
 * -> Does not change the passed list
 *
 * >>> This function is redundant to the ll_for_each_element_do function. <<<
 *
 * @param list the list to iterate through
 * @param print_element the print-function
 */
void ll_print( LinkedList *list, void (*print_element)(void*, size_t) ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif //DEBUG

    LinkedListElement *curr = list->first;
    print_element(list->first->data, list->first->datasize);
    while( (curr = next(curr)) ) 
        print_element(curr->data, curr->datasize);
}

/*
 * prints the data inside the list binary
 *
 * @param list the list to iterate through
 */
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

#endif //LL_PRINTABLE

/*
 * Undef "static" macros
 */
#undef savinclen
#undef savdeclen
