#include "stack/stack.h"

static StackElement * stackelement( long long int data_size, void *data );

Stack * empty_stack() {
    Stack *stack = (Stack*) malloc( sizeof(Stack) );
    stack->first = NULL;
    return stack;
}

static StackElement * stackelement( long long int data_size, void *data ) {
    StackElement *element = (StackElement*) malloc( sizeof(StackElement) + data_size );
    element->next = NULL;
    element->data_size = data_size;
    memcpy( element->data, data, data_size );
    return element;
}

void stackpush( Stack *stack, long long int data_size, void *data ) {
    StackElement *element = stackelement(data_size, data);
    element->next = stack->first;
    stack->first = element;
}
