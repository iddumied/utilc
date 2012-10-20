#include "stack/stack.h"
#include "utilc_test_utils.h"

static int test_pushing(void);
static int test_poping(void);
static int test_print_bin(void);

static void cleanup(Stack *stack);
static void cleanup_element(StackElement *ste);

/* desc, strict, int(*testfunc)(), result*/
Test tests[] = {
    {"Pushing",     1,  test_pushing,       0 },
    {"Poping",      1,  test_poping,        0 },

#ifdef STACK_PRINTABLE
    {"printing",    1,  test_print_bin,     0 },
#endif //STACK_PRINTABLE
    {NULL}
};

static int test_pushing() {
    Stack *stack = empty_stack();
    int result = 0;
    int topush = 1;

    stackpush(stack, sizeof(topush), &topush);
    result =    (stack->first->next == NULL) && 
                (stack->first->data_size == sizeof(topush)) &&
                ((int)*stack->first->data == topush);

    cleanup(stack);
    return result;
}

static int test_poping() {
    int d = depends(test_pushing);
    if (!d) return 0;

    Stack *stack = empty_stack();
    int ary[] = { 1, 2, 3, 4, 5 };
    int res[5];
    int worked = 1;

    unsigned int i, j;
    for( i = 0; i<len(ary); i++ )
        stackpush(stack, sizeof(ary[i]), &ary[i] );

    for( i = 0 ; i<len(res); i++ )
        res[i] = *(int*)stackpop(stack);

    for( i = 0, j = len(res)-1 ; i<len(res) && j != 0 && worked; i++, j-- )
        worked = res[i] == ary[j];

    return worked;
}

#ifdef STACK_PRINTABLE
static int test_print_bin() {
    int d = depends(test_pushing);
    if (!d) return 0;

    Stack *stack = empty_stack();
    double ary[] = { 1, 2, 3, 4, 5 };

    printf(":: Stacktest: Testing with double, which is %lu on this platform!\n",sizeof(double));
    unsigned int i;
    for( i = 0; i<len(ary); i++ )
        stackpush(stack, sizeof(double), &ary[i] );

    stack_print_binary(stack);

    return 1;
}
#endif //STACK_PRINTABLE

/*
 * Cleanup
 */
static void cleanup(Stack *stack) {
    cleanup_element(stack->first);
    free(stack);
}

static void cleanup_element(StackElement *ste) {
    if( ste->next )
        cleanup_element(ste->next);
    else
        free(ste);
}

int main(void) {
    unsigned int i;
    int worked = 1;
    for( i = 0; test_exists(&tests[i]) && worked; i++ ) {
        worked = test_exec(&tests[i]);
    }

    return !worked;
}
