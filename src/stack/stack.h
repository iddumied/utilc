#ifndef __STACK__H__
#define __STACK__H__

#include <stdlib.h>
#include <string.h>

struct stack_element {
   struct stack_element *next;
   long long int data_size;
   char data[]; 
};
typedef struct stack_element StackElement;

typedef struct {
    StackElement *first;
} Stack;

Stack * empty_stack(void);
void stackpush(Stack *stack, long long int data_size, void *data);

#endif
