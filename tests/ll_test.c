/*
 * This is the testing file for the linkedlist library
 */

#include "linkedlist/linkedlist.h"

#include <stdlib.h>
#include <stdio.h>

#define len(x) (sizeof(x)/sizeof(x[0]))

#define depends(x) __depends(__func__, x )

/*
 * Prototypes : Testing functions
 */
static int test_creating_and_removing(void);
static int test_pushing(void);
static int test_poping(void);
static int test_length(void);
static int test_get_first(void);
static int test_get_last(void);
static int test_get_by_index(void);
static int test_destroy_by_element(void);
static int test_destroy_by_index(void);
static int test_element_in_list(void);
static int test_dump(void);
//static int test_sort(void); // no sorting in version 1
static int test_get_by_cond(void);
static int test_for_each_do(void);
static int test_for_each_by_cond(void);
static int test_join(void);

typedef struct {
    char *desc;
    int strict;
    int (*testfunc)();
    int result;
} Test;

Test tests[] = {
    {"Creating/Deleting",   1,   test_creating_and_removing, 0 },
    {"Pushing",             1,   test_pushing,               0 },
    {"Poping",              1,   test_poping,                0 },
    {"Length",              1,   test_length,                0 },
    {"get first",           0,  test_get_first,             0 },
    {"get last",            0,  test_get_last,              0 },
    {"get by index",        0,  test_get_by_index,          0 },
    {"destroy by element",  0,  test_destroy_by_element,    0 },
    {"destroy by index",    0,  test_destroy_by_index,      0 },
    {"element in list",     0,  test_element_in_list,       0 },
    {"dump",                0,  test_dump,                  0 },
    //{"sort",                0,  test_sort,                  0 },
    {"get by condition",    0,  test_get_by_cond,           0 },
    {"for each do",         0,  test_for_each_do,           0 },
    {"for each by cond do", 0,  test_for_each_by_cond,      0 },
    {"join",                0,  test_join,                  0 },
    {NULL, NULL, NULL},
};

/*
 * Prototypes : Helper functions 
 */
static void testing(char*);
static void success(char*);
static void failure(char*, int);
static void cleanup(LinkedList*);
static int test_exec(Test*);
static int __depends(const char*, int (*)(void));
int comparefunction(void*, void*);
int condition_is_bigger_three(void*);
int do_foreach_inc(void*);
/*
 * function definitions : testing functions
 */

static int test_creating_and_removing() {
    double value = 5.00;
    LinkedList *list = linkedlist( &value );

    ll_destroy(list);
    return 1;
}

static int test_pushing() {
    int d = depends( test_creating_and_removing );
    if (!d) return 0;

    int res;
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *list = empty_linkedlist();
    ll_push(list, &value1);
    ll_push(list, &value2);
    res = ((double*)list->first->data == &value1 && 
            (double*)list->first->next->data == &value2);
    cleanup(list);

    return res;
}

static int test_poping() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *poped_ptr = ((double*) ll_pop(l));
    int poped_away = poped_ptr != l->first->e;

    cleanup(l); // does not affect the poped_ptr
    return (poped_ptr == &value1) && poped_away;
}

static int test_length() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d ) return 0;

    int result = 0;
    int i;
    double ary[] = { 1.0, 2.0, 3.0, 4.0 };
    LinkedList *l = empty_linkedlist();

    for( i = 0 ; i<len(ary) ; i++ ) {
        result = i == ll_len(l, 1);
        ll_push(l, &ary[i]);
    }

    return result;
}

static int test_get_first() {
    int d = depends( test_creating_and_removing );
    if (!d) return 0;

    double value = 5.00;
    LinkedList *l = linkedlist(&value);
    double *poped_ptr = (double*) ll_first(l);
    cleanup(l); // does not affect the poped_ptr
    return poped_ptr == &value;
}

static int test_get_last() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *poped_ptr = (double*) ll_last(l);
    cleanup(l); // does not affect the poped_ptr
    return poped_ptr == &value2;
}

static int test_get_by_index() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    int worked = 1;
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

static int test_destroy_by_element() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    int worked = 1;
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

static int test_destroy_by_index() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    int worked = 1;
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

static int test_element_in_list() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.0;
    double value2 = 6.0;
    LinkedList *list = linkedlist(&value1);
    ll_push(list, &value2);
    int a = ll_element_in_list(list, &value1);
    int b = !ll_element_in_list(list, &value2);

    return (a&&b);
}

static int test_dump() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
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

/*
static int test_sort() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
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
*/

static int test_get_by_cond(void){
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    LinkedList *cmpList = linkedlist(&ary2[0]);
    int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i] );
    }
    LinkedList *new = ll_get_by_cond(list, condition_is_bigger_three);

    for( i = 0 ; i<len(ary2); i++ ) {
        worked = ary2[i] == *((double*)ll_element(new,i));
    }
    return worked;
}

static int test_for_each_do() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 2.0, 3.0, 4.5, 5.9, 6.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i]);
    }

    ll_for_each_element_do(list, do_foreach_inc );
    double a, b;
    for( i = 0 ; i<len(ary2); i++ ) {
        a = ary2[i];
        b = *((double*)ll_element(list, i));
        worked = a == b; 
    }
    return worked;
}

static int test_for_each_by_cond() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    d = d && depends( test_for_each_do );
    if (!d) return 0;

    int worked = 1;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 1.0, 2.0, 4.5, 5.9, 6.5 };
    LinkedList *list = linkedlist(&ary1[0]);
    int i;
    for( i = 1 ; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i]);
    }

    ll_for_each_element_by_condition_do(list, condition_is_bigger_three, do_foreach_inc );

    double a, b;
    for( i = 0 ; i<len(ary2) ; i++ ) {
        a = ary2[i];
        b = *((double*)ll_element(list, i));
        printf( "Compare: %f == %f\n", a, b );
        worked = a == b; 
    }
    return worked;
}

static int test_join() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
    double ary1[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    double ary2[] = { 6.0, 7.0, 8.0, 9.0, 10.0 };
    double res[] =  { 1.0, 2.0, 3.0, 4.0, 5.0, 
                      6.0, 7.0, 8.0, 9.0, 10.0 };

    LinkedList *list1 = empty_linkedlist();
    LinkedList *list2 = empty_linkedlist();
    LinkedList *res_list;

    int i;
    for( i = 0; i<len(ary1); i++ ) {
        ll_push(list1, &ary1[i]);
    }
    for( i = 0; i<len(ary2); i++ ){
        ll_push(list2, &ary2[i]);
    }

    res_list = ll_join(list1,list2);
    double a, b;

    for( i = 0 ; i<len(res); i++ ) {
        printf( "[%i] ", i );
        a = *((double*) ll_element(res_list, i));
        b = res[i];
        worked = a == b;
        printf( "Compare: %f == %f\n\n", i, a, b);
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

static void failure(char* desc, int strict) {
    if (strict)
        printf("\t-[STRICT FAILED]\n");
    else
        printf( "\t[FAIL]\n");
}

static void cleanup(LinkedList *l) {
    ll_destroy(l);
}

static int __depends(const char *func, int (*other)(void) ) {
    int i;
    int res = 0;
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
int condition_is_bigger_three( void* value ) {
    return ( (*(double*)value) > 3 );
}

/*
 * do for each function for testing for each and for each by cond function
 */
int do_foreach_inc( void *value ) {
    (*(double*)value)++;
    return 1;
};

/*
 * Main
 */

static int test_exec( Test *test ) {
    testing(test->desc);
    int res = test->testfunc();
    if (res)
        success(test->desc);
    else
        failure(test->desc, test->strict);

    test->result = res;
    return res;
}

int main( int argc, char ** argv ) {
    int i;
    int worked = 1;
    for( i = 0; tests[i].desc && worked; i++ ) {
        worked = test_exec( &tests[i] );
    }
    return worked;
}
