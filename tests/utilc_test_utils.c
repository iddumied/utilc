#include "utilc_test_utils.h"
/*
 * function definitions : helper functions
 */

void testing(char* desc) {
    printf( "\n\n\t:: linkedlist-test: %s\n", desc);
}

void success(char* desc) {
    printf( "\t[success]: %s\n", desc);
}

void failure(char* desc, bool strict) {
    if (strict)
        printf("\t-[STRICT FAILED]: %s\n", desc);
    else
        printf( "\t[FAIL]: %s\n", desc);
}

bool __depends(const char *func, bool (*other)(void) ) {
    int i;
    bool res = false;
    for( i = 0 ; tests[i].desc && !res ; i++ ) {
        if( tests[i].testfunc == other )
            res = tests[i].result;
    }

    if (!res) {
        printf( "%s has dependencies which are not working!\n", func );
    }
    return res;
} 

/*
 * Test exists, if description and function exists
 */
bool test_exists(Test *test) {
    return (test->desc != NULL && test->testfunc != NULL);
}

bool test_exec( Test *test ) {
    testing(test->desc);
    bool res = test->testfunc();
    if (res)
        success(test->desc);
    else
        failure(test->desc, test->strict);

    test->result = res;
    return res;
}

