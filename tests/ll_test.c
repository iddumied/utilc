/*
 * This is the testing file for the linkedlist library
 */

#include "linkedlist/linkedlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Prototypes : Testing functions
 */
static bool test_creating_and_removing(void);
static bool test_pushing(void);
static bool test_poping(void);
static bool test_get_first(void);
static bool test_get_last(void);
static bool test_get_by_index(void);
/*
static bool test_destroy_by_index(void);
static bool test_destroy_by_element(void);
static bool test_element_in_list(void);
static bool test_dump(void);
static bool test_sort(void);
static bool test_get_by_cond(void);
static bool test_for_each(void);
static bool test_for_each_by_cond(void);
static bool test_join(void);
*/

typedef struct {
    char *desc;
    bool strict;
    bool (*testfunc)();
} Test;

Test tests[] = {
    {"Creating/Deleting",   true,   test_creating_and_removing},
    {"Pushing",             true,   test_pushing},
    {"Poping",              true,   test_poping},
    {"get first",           false,  test_get_first},
    {"get last",            false,  test_get_last},
    {"get by index",        false,  test_get_by_index},
    {NULL, NULL, NULL},
};

/*
 * Prototypes : Helper functions 
 */
static void testing(char*);
static void success(char*);
static void failure(char*, bool);
static void cleanup(LinkedList*);
static bool test_exec(char*, bool, bool (*func)());

/*
 * function definitions : testing functions
 */

static bool test_creating_and_removing() {
    double value = 5.00;
    LinkedList *list = linkedlist( &value );

    ll_destroy(list);
    return true;
}

static bool test_pushing() {
    bool res;
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *list = linkedlist( &value1 );
    ll_push(list, &value2);
    res = ((double*)list->first->e == &value1 && 
            (double*)list->first->next->e == &value2);
    cleanup(list);

    return res;
}

static bool test_poping() {
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *poped_ptr = ((double*) ll_pop(l));

    cleanup(l); // does not affect the poped_ptr
    return poped_ptr == &value1;
}

static bool test_get_first() {
    double value = 5.00;
    LinkedList *l = linkedlist(&value);
    double *poped_ptr = (double*) ll_first(l);
    cleanup(l); // does not affect the poped_ptr
    return poped_ptr == &value;
}

static bool test_get_last() {
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *poped_ptr = (double*) ll_last(l);
    cleanup(l); // does not affect the poped_ptr
    return poped_ptr == &value2;
}

static bool test_get_by_index() {
    bool worked = true;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    int i;
    LinkedList *list = linkedlist(&ary[0]);
    for( i = 1 ; ary[i]; i++) {
        ll_push( list, &ary[i] );
    }

    double *a, *b;
    for( i = 0; ary[i] && worked; i++) {
        a = (double*)ll_element(list, i);
        b = &ary[i];
        worked = a == b;
    }

    cleanup(list);
    return worked;
}

/*
 * function definitions : helper functions
 */

static void testing(char* desc) {
    printf( "linkedlist-test: %s\n", desc);
}

static void success(char* desc) {
    printf( "\t- success: %s\n", desc);
}

static void failure(char* desc, bool strict) {
    if (strict)
        printf("\t- STRICT FAILED: %s\n", desc );
    else
        printf( "\t- FAIL: %s\n", desc);
}

static void cleanup(LinkedList *l) {
    printf("Cleanup.\n");
    ll_destroy(l);
    printf("\n");
}

/*
 * Main
 */

static bool test_exec(char *desc, bool strict, bool (*func)() ) {
    testing(desc);
    bool res = func();
    if (res)
        success(desc);
    else
        failure(desc, strict);
    return res;
}

int main( int argc, char ** argv ) {
    int i;
    bool worked = true;
    for( i = 0; tests[i].desc && worked; i++ ) {
        worked = test_exec( tests[i].desc, tests[i].strict, tests[i].testfunc );
    }
}
