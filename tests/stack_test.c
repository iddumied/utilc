#include "stack/stack.h"
#include <stdbool.h>
#include <stdio.h>

#define len(x) (sizeof(x)/sizeof(x[0]))

int main(void) {
    Stack *stack = empty_stack();
    int test_ints[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    //double test_doubles[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 0.0 };

    int i;
    for( i = 0 ; i < len(test_ints); i++ ) {
        stackpush( stack, sizeof( test_ints[i] ), &test_ints[i] );
    }
    
    bool worked = true;
    while( i > 0 && worked ) {
        worked = test_ints[i] == *((int*)stackpop(stack));
    }
    if(worked)
        printf("Worked!\n");
    else
        printf("Does not work!\n");
}
