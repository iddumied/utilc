#ifndef __STACK__H__
#define __STACK__H__

#include <stdlib.h>
#include <string.h>

struct stack_element {
   struct stack_element *next;
   size_t data_size;
   char data[]; /* data, multible of 1 byte */
};
typedef struct stack_element StackElement;

typedef struct {
    StackElement *first;
} Stack;

Stack * empty_stack(void);
void stackpush(Stack *stack, size_t data_size, void *data);
void * stackpop(Stack *stack);

#ifdef STACK_PRINTABLE
void stack_print_binary(Stack *stack);
#endif //STACK_PRINTABLE

#endif
