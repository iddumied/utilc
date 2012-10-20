#include "stack/stack.h"

#ifdef DEBUG
#define LIBRARYNAME "stack"
#include "debug.h"
#endif

static StackElement * stackelement( size_t data_size, void *data );
static StackElement * next(StackElement* curr);

#ifdef STACK_PRINTABLE
static void print_stackelement_binary(StackElement *ste);
#endif //STACK_PRINTABLE

Stack * empty_stack() {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    Stack *stack = (Stack*) malloc( sizeof(Stack) );
    if (stack)
        stack->first = NULL;
    return stack;
}

static StackElement * stackelement( size_t data_size, void *data ) {
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

static StackElement *next(StackElement *curr) {
    return curr->next;
}

void stackpush( Stack *stack, size_t data_size, void *data ) {
#ifdef DEBUG
    EXPLAIN_FUNC; 
#endif // DEBUG

    StackElement *element = stackelement(data_size, data);
    if (element) {
        element->next = stack->first;
        stack->first = element;
    }
}

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

#ifdef STACK_PRINTABLE
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
