#include "stack/stack.h"
#include "utilc_test_utils.h"

static bool test_pushing(void);
static bool test_poping(void);

static void cleanup(Stack *stack);
static void cleanup_element(StackElement *ste);

/* desc, strict, bool(*testfunc)(), result*/
Test tests[] = {
    {"Pushing", true, test_pushing, false },
    {"Poping",  true, test_poping,  false },
    {NULL, NULL, NULL, NULL}
};

static bool test_pushing() {
    Stack *stack = empty_stack();
    bool result = false;
    int topush = 1;

    stackpush(stack, sizeof(topush), &topush);
    result =    (stack->first->next == NULL) && 
                (stack->first->data_size == sizeof(topush)) &&
                ((int)*stack->first->data == topush);

    cleanup(stack);
    return result;
}

static bool test_poping() {
    bool d = depends(test_pushing);
    if (!d) return false;

    Stack *stack = empty_stack();
    int ary[] = { 1, 2, 3, 4, 5 };
    int res[5];
    bool worked = true;

    int i, j;
    for( i = 0; i<len(ary); i++ )
        stackpush(stack, sizeof(ary[i]), &ary[i] );

    for( i = 0 ; i<len(res); i++ )
        res[i] = *(int*)stackpop(stack);

    for( i = 0, j = len(res)-1 ; i<len(res) && j >= 0 && worked; i++, j-- )
        worked = res[i] == ary[j];

    return worked;
}

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
    bool worked = true;
    for( i = 0; test_exists(&tests[i]) && worked; i++ ) {
        worked = test_exec(&tests[i]);
    }

    return worked;
}
