#include "stack/stack.h"

#ifdef DEBUG
#define LIBRARYNAME "stack"
#include "debug.h"
#endif

/*
 * static function prototypes
 */ 
static StackElement * stackelement( void *data, size_t data_size );
static StackElement * next(StackElement* curr);

static void remove_stackelement(StackElement *se);

#ifdef STACK_PRINTABLE
static void print_stackelement_binary(StackElement *ste);
#endif //STACK_PRINTABLE

/*
 * ===============================
 * Static function implementations
 * ===============================
 */

/*
 * create new stackelement with data
 *
 * @return StackElement*
 * @param data pointer to the data which should be stored
 * @param data_size size of the data to store
 */
static StackElement * stackelement( void *data, size_t data_size ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    StackElement *element = (StackElement*) malloc( sizeof(StackElement) + data_size );
    if (element) {
        element->next = NULL;
        element->data_size = data_size;
        memcpy( element->data, data, data_size );
    }
    return element;
}

/*
 * return the next StackElement based on the current
 *
 * @return StackElement*
 * @param curr the current stackelement to get the next from
 */
static StackElement * next(StackElement *curr) {
    return curr->next;
}

static void remove_stackelement(StackElement *se) {
    if( se->next )
        remove_stackelement(se->next);
    else
        free(se);
}

#ifdef STACK_PRINTABLE
/*
 * print the StackElement data binary
 *
 * @param str the StackElement
 */
static void print_stackelement_binary(StackElement *ste) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif //DEBUG

    unsigned char mask = 0x01;
    unsigned int ptr, bit;

    for(ptr = 0; ptr < ste->data_size; ptr++ ) {
        for(bit = 7; bit != 0 ; bit-- ) {
            if( (mask<<bit) & (unsigned char) *(ste->data+ptr) ) {
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
#endif //STACK_PRINTABLE


/*
 * ========================
 * Function implementations
 * ========================
 */

/*
 * Create a new, empty Stack
 *
 * @return Stack*
 */
Stack * empty_stack() {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    Stack *stack = (Stack*) malloc( sizeof(Stack) );
    if (stack)
        stack->first = NULL;
    return stack;
}

/*
 * Push data to the Stackpush
 *
 * @param stack the Stack where the data should be pushed include
 * @param data a pointer to the data
 * @param data_size the size of the data
 */
void stackpush(Stack *stack, void *data, size_t data_size) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    StackElement *element = stackelement(data, data_size);
    if (element) {
        element->next = stack->first;
        stack->first = element;
    }
}

/*
 * Pop some data from the Stack
 *
 * @return void*
 * @param stack the stack to pop from
 */
void * stackpop( Stack *stack ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    void *ret = malloc( stack->first->data_size );
    if (ret) {
        memcpy( ret, &stack->first->data, stack->first->data_size );
        StackElement *next = stack->first->next;
        free( stack->first );
        stack->first = next;
    }
    return ret;
}

/*
 * Removes Stack from memory
 *
 * @param stack the Stack to remove
 */
void stackdelete(Stack *stack) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif //DEBUG
    
    if( stack->first )
        remove_stackelement(stack->first);
    free(stack);
}

#ifdef STACK_PRINTABLE
/*
 * Prints each stack elements data binary.
 *
 * @param stack the stack which should be print
 */
void stack_print_binary(Stack *stack) {
#ifdef DEBUG
    EXPLAIN_FUNC;
#endif //DEBUG

    StackElement *curr = stack->first;
    print_stackelement_binary(curr);
    while( (curr = next(curr)) && curr ) {
        print_stackelement_binary(curr);
    }
    printf("\n");
}
#endif //STACK_PRINTABLE
