/*
 * This is the testing file for the linkedlist library
 */

#include "linkedlist/linkedlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define len(x) (sizeof(x)/sizeof(x[0]))

/*
 * Prototypes : Testing functions
 */
static bool test_creating_and_removing(void);
static bool test_pushing(void);
static bool test_poping(void);
static bool test_get_first(void);
static bool test_get_last(void);
static bool test_get_by_index(void);
static bool test_destroy_by_element(void);
static bool test_destroy_by_index(void);
static bool test_element_in_list(void);
static bool test_dump(void);
static bool test_sort(void);
static bool test_get_by_cond(void);
static bool test_for_each_do(void);
static bool test_for_each_by_cond(void);
static bool test_join(void);

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
    {"destroy by element",  false,  test_destroy_by_element},
    {"destroy by index",    false,  test_destroy_by_index},
    {"element in list",     false,  test_element_in_list},
    {"dump",                false,  test_dump},
    {"sort",                false,  test_sort},
    {"get by condition",    false,  test_get_by_cond},
    {"for each do",         false,  test_for_each_do},
    {"for each by cond do", false,  test_for_each_by_cond},
    {"join",                false,  test_join},
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
int comparefunction(void*, void*);
bool condition_is_bigger_three(void*);
bool do_foreach_inc(void*);
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
    for( i = 1 ; i<len(ary); i++) {
        ll_push( list, &ary[i] );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++) {
        a = (double*)ll_element(list, i);
        b = &ary[i];
        worked = a == b;
    }

    cleanup(list);
    return worked;
}

static bool test_destroy_by_element() {
    bool worked = true;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    int i;
    LinkedList *list = linkedlist(&ary[0]);
    for( i = 1 ; i<len(ary) ; i++ ) {
        ll_push( list, &ary[i] );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++ ) {
        a = (double*)ll_destroy_by_element(list, list->first);
        b = &ary[i];
        worked = a == b;
    }
    return worked;
}

static bool test_destroy_by_index() {
    bool worked = true;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    int i;
    LinkedList *list = linkedlist(&ary[0]);
    for( i = 1 ; i<len(ary) ; i++ ) {
        ll_push( list, &ary[i] );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++ ) {
        a = (double*)ll_destroy_by_index(list, 0);
        b = &ary[i];
        worked = a == b;
    }
    return worked;
}

static bool test_element_in_list() {
    double value1 = 5.0;
    double value2 = 6.0;
    LinkedList *list = linkedlist(&value1);
    ll_push(list, &value2);
    bool a = ll_element_in_list(list, &value1);
    bool b = !ll_element_in_list(list, &value2);

    return (a&&b);
}

static bool test_dump() {
    bool worked = true;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary[0]);
    int i;
    for( i = 1; i<len(ary); i++ ) {
        ll_push(list, &ary[i] );
    }
    LinkedList *dump = ll_dump(list);

    double *a;
    for( i = 0; i<len(ary) && worked; i++) {
        a = (double*)ll_element(dump,i);
        worked = a == &ary[i];
    }
    return (worked && list != dump);
}

static bool test_sort() {
    bool worked = true;
    double ary1[] = { 2.0, 5.5, 2.0, 4.9, 1.0 };
    double ary2[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i] );
    }
    ll_sort(list, comparefunction);

    double *a;
    for( i = 0 ; i<len(ary1); i++ ) {
        a = (double*) ll_element( list, i );
        worked = a == &ary2[i];
    }

    return worked;
}

static bool test_get_by_cond(void){
    bool worked = true;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    LinkedList *cmpList = linkedlist(&ary2[0]);
    int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i] );
    }
    for( i = 0; i<len(ary2); i++ ) {
        ll_push(list, &ary2[i] );
    }
    LinkedList *new = ll_get_by_cond(list, condition_is_bigger_three);

    for( i = 0 ; i<len(ary2); i++ ) {
        worked = &ary2[i] == (double*)ll_element(new,i);
    }
    return worked;
}

static bool test_for_each_do() {
    bool worked = true;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 2.0, 3.0, 4.5, 5.9, 6.5 };
    LinkedList *list = linkedlist(&ary1[0]);

    ll_for_each_element_do(list, do_foreach_inc );
    int i;
    double *a, *b;
    for( i = 0 ; i<len(ary2); i++ ) {
        a = &ary2[i];
        b = (double*)ll_element(list, i);
        worked = a == b; 
    }
    return worked;
}

static bool test_for_each_by_cond() {
    bool worked = true;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 1.0, 2.0, 4.5, 5.9, 6.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    int i;
    for( i = 0 ; i<len(ary1); i++ ) {
        ll_push(list,&ary1[i]);
    }

    ll_for_each_element_by_condition_do(list, condition_is_bigger_three, do_foreach_inc );
    double *a, *b;
    for( i = 0 ; i<len(ary2) ; i++ ) {
        a = &ary2[i];
        b = (double*)ll_element(list, i);
        worked = a == b; 
    }
    return worked;
}

static bool test_join() {
    bool worked = true;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 1.0, 2.0, 4.5, 5.9, 6.5 };
    double res[] = { 1.0, 2.0, 3.5, 4.9, 5.5, 
                     1.0, 2.0, 4.5, 5.9, 6.5 };

    LinkedList *list1 = linkedlist(&ary1[0]);
    LinkedList *list2 = linkedlist(&ary2[0]);
    LinkedList *res_list;

    int i;
    for( i = 0; i<len(ary1); i++ ) {
        ll_push(list1, &ary1[i]);
    }
    for( i = 0; i<len(ary2); i++ ){
        ll_push(list2, &ary2[i]);
    }

    res_list = ll_join(list1,list2);
    double *a, *b;

    for( i = 0 ; i<len(res); i++ ) {
        a = (double*) ll_element(res_list, i);
        b = &res[i];
        worked = a == b;
    }

    return worked;
}

/*
 * function definitions : helper functions
 */

static void testing(char* desc) {
    printf( "\n\n\t:: linkedlist-test: %s\n", desc);
}

static void success(char* desc) {
    printf( "\t[success]\n");
}

static void failure(char* desc, bool strict) {
    if (strict)
        printf("\t-[STRICT FAILED]\n");
    else
        printf( "\t[FAIL]\n");
}

static void cleanup(LinkedList *l) {
    ll_destroy(l);
}

/*
 * Compare function for sort testing
 */
int comparefunction(void *a, void *b) {
    double *doubleA, *doubleB;
    doubleA = (double*)a;
    doubleB = (double*)b;

    if( doubleA == doubleB )
        return 0;
    if( doubleA < doubleB )
        return 1;
    return -1;
}

/*
 * Condition function for testing 
 * - get by condition
 * - for each by condition do
 */ 
bool condition_is_bigger_three( void* value ) {
    double a = 3;
    double *aptr = &a;
    return ((double*)value)>aptr;
}

/*
 * do for each function for testing for each and for each by cond function
 */
bool do_foreach_inc( void *value ) {
    double double_value = *((double*)value);
    double_value++;
    return true;
};

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
