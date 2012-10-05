#include "stack/stack.h"

static StackElement * stackelement( size_t data_size, void *data );

Stack * empty_stack() {
    Stack *stack = (Stack*) malloc( sizeof(Stack) );
    stack->first = NULL;
    return stack;
}

static StackElement * stackelement( size_t data_size, void *data ) {
    StackElement *element = (StackElement*) malloc( sizeof(StackElement) + data_size );
    element->next = NULL;
    element->data_size = data_size;
    memcpy( element->data, data, data_size );
    return element;
}

void stackpush( Stack *stack, size_t data_size, void *data ) {
    StackElement *element = stackelement(data_size, data);
    element->next = stack->first;
    stack->first = element;
}

void * stackpop( Stack *stack ) {
    void *ret = malloc( stack->first->data_size );
    memcpy( ret, &stack->first->data, stack->first->data_size );
    StackElement *next = stack->first->next;
    free( stack->first );
    stack->first = next;
    return ret;
}
