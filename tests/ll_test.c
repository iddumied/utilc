/*
 * This is the testing file for the linkedlist library
 */

#include "linkedlist/linkedlist.h"
#include "utilc_test_utils.h"

#include <stdlib.h>
#include <stdio.h>

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

static int test_datasize_of_first(void);
static int test_datasize_of_last(void);
static int test_datasize_of_element(void);
static int test_datasize_of_list(void);

#ifdef LL_PRINTABLE
static int test_print(void);
static int test_print_binary(void);
#endif //LL_PRINTABLE

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
    {"get by condition",    0,  test_get_by_cond,           0 },
    {"for each do",         0,  test_for_each_do,           0 },
    {"for each by cond do", 0,  test_for_each_by_cond,      0 },
    {"join",                0,  test_join,                  0 },

    {"datasize: first",     0,  test_datasize_of_first,     0 },
    {"datasize: last",      0,  test_datasize_of_last,      0 },
    {"datasize: element",   0,  test_datasize_of_element,   0 },
    {"datasize: list",      0,  test_datasize_of_list,      0 },

#ifdef LL_PRINTABLE
    {"list print",          0,  test_print,                 0 },
    {"list print binary",   0,  test_print_binary,          0 },
#endif //LL_PRINTABLE
    { NULL },
};

/*
 * Prototypes : Helper functions 
 */
static void cleanup(LinkedList*);
int comparefunction(void*, void*);
int condition_is_bigger_three(void*, size_t);
int do_foreach_inc(void*, size_t);

#ifdef LL_PRINTABLE
void print_element(void*, size_t);
#endif //LL_PRINTABLE

/*
 * function definitions : testing functions
 */

static int test_creating_and_removing() {
    double value = 5.00;
    LinkedList *list = linkedlist( &value, sizeof(value) );

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
    ll_push(list, &value1, sizeof(value1));
    ll_push(list, &value2, sizeof(value2));
    
    res = ( (0 == memcmp(list->first->data, &value1, sizeof(double)))
        &&  (0 == memcmp(list->first->next->data, &value2, sizeof(double))));
    cleanup(list);

    return res;
}

static int test_poping() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1, sizeof(value1));
    ll_push(l, &value2, sizeof(value2));
    void *poped_ptr = ll_pop(l);
    int res = memcmp( (double*)poped_ptr, &value1, sizeof(double)) == 0;

    cleanup(l); // does not affect the poped_ptr
    return res;
}

static int test_length() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d ) return 0;

    int result = 0;
    unsigned int i;
    double ary[] = { 1.0, 2.0, 3.0, 4.0 };
    LinkedList *l = empty_linkedlist();

    for( i = 0 ; i<len(ary) ; i++ ) {
        result = i == ll_len(l, 1);
        ll_push(l, &ary[i], sizeof(ary[i]));
    }

    return result;
}

static int test_get_first() {
    int d = depends( test_creating_and_removing );
    if (!d) return 0;

    double value = 5.00;
    LinkedList *l = linkedlist(&value, sizeof(value));
    double *poped_ptr = (double*) ll_first(l);
    int res = 0 == memcmp(poped_ptr, &value, sizeof(double));

    cleanup(l); // does not affect the poped_ptr
    return res;
}

static int test_get_last() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1, sizeof(value1));
    ll_push(l, &value2, sizeof(value2));
    double *poped_ptr = (double*) ll_last(l);

    int res = 0 == memcmp(poped_ptr, &value2, sizeof(double));
    cleanup(l); // does not affect the poped_ptr
    return res;
}

static int test_get_by_index() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    int worked = 1;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    unsigned int i;
    LinkedList *list = linkedlist(&ary[0], sizeof(ary[0]));
    for( i = 1 ; i<len(ary); i++) {
        ll_push( list, &ary[i], sizeof(ary[i]) );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++) {
        a = (double*)ll_element(list, i);
        b = &ary[i];
        worked = 0 == memcmp(a,b, sizeof(double));
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
    unsigned int i;
    LinkedList *list = linkedlist(&ary[0], sizeof(ary[0]));
    for( i = 1 ; i<len(ary) ; i++ ) {
        ll_push( list, &ary[i], sizeof(ary[i]) );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++ ) {
        a = (double*)ll_destroy_by_element(list, list->first);
        b = &ary[i];
        worked = 0 == memcmp(a, b, sizeof(double));
    }
    return worked;
}

static int test_destroy_by_index() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    int worked = 1;
    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    unsigned int i;
    LinkedList *list = linkedlist(&ary[0], sizeof(ary[0]));
    for( i = 1 ; i<len(ary) ; i++ ) {
        ll_push( list, &ary[i], sizeof(ary[i]) );
    }

    double *a, *b;
    for( i = 0; i<len(ary) && worked; i++ ) {
        a = (double*)ll_destroy_by_index(list, 0);
        b = &ary[i];
        worked = 0 == memcmp(a, b, sizeof(double));
    }
    return worked;
}

static int test_element_in_list() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    if (!d) return 0;

    double value1 = 5.0;
    double value2 = 6.0;
    LinkedList *list = linkedlist(&value1, sizeof(value1));
    ll_push(list, &value2, sizeof(value2));
    int a = ll_element_in_list(list, &value1, sizeof(value1));
    int b = !ll_element_in_list(list, &value2, sizeof(value2));

    return (a&&b);
}

static int test_dump() {
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
    unsigned int i;

    double ary[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary[0], sizeof(double));
    for( i = 1; i<len(ary); i++ )
        ll_push(list, &ary[i], sizeof(double) );

    LinkedList *dump = ll_dump(list);

    double *a, *b;
    for( i = 0; (i<len(ary)) && worked; i++) {
        a = (double*)ll_element(dump,i);
        b = &ary[i];
        worked = 0 == memcmp(a, b, sizeof(double));
        printf("[%u/%lu] : %f == %f, size: %lu\n", i, len(ary), *a, *b, sizeof(double) );
    }
    return (worked && list != dump);
}

static int test_get_by_cond(void){
    int d = depends( test_creating_and_removing );
    d = d && depends( test_pushing );
    d = d && depends( test_get_by_index );
    if (!d) return 0;

    int worked = 1;
    double ary1[] = { 1.0, 2.0, 3.5, 4.9, 5.5 };
    double ary2[] = { 3.5, 4.9, 5.5 };
    LinkedList *list = linkedlist(&ary1[0], sizeof(ary1[0]));
    unsigned int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i], sizeof(ary1[i]) );
    }
    LinkedList *new = ll_get_by_cond(list, condition_is_bigger_three);

    for( i = 0 ; i<len(ary2); i++ )
        worked = 0 == memcmp( (double*)ll_element(new,i), &ary2[i], sizeof(double));

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
    LinkedList *list = linkedlist(&ary1[0], sizeof(ary1[0]));
    unsigned int i;
    for( i = 1; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i], sizeof(ary1[i]));
    }

    ll_for_each_element_do(list, do_foreach_inc );
    double *a, *b;
    for( i = 0 ; i<len(ary2); i++ ) {
        a = &ary2[i];
        b = (double*)ll_element(list, i);
        worked = 0 == memcmp(a, b, sizeof(double)); 
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
    LinkedList *list = linkedlist(&ary1[0], sizeof(ary1[0]));
    unsigned int i;
    for( i = 1 ; i<len(ary1); i++ ) {
        ll_push(list, &ary1[i], sizeof(ary1[i]));
    }

    ll_for_each_element_by_condition_do(list, condition_is_bigger_three, do_foreach_inc );

    double *a, *b;
    for( i = 0 ; i<len(ary2) ; i++ ) {
        a = &ary2[i];
        b = (double*)ll_element(list, i);
        worked = 0 == memcmp(a, b, sizeof(double)); 
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

    unsigned int i;
    for( i = 0; i<len(ary1); i++ ) {
        ll_push(list1, &ary1[i], sizeof(ary1[0]));
    }
    for( i = 0; i<len(ary2); i++ ){
        ll_push(list2, &ary2[i], sizeof(ary2[0]));
    }

    res_list = ll_join(list1,list2);

    double *a, *b;
    for( i = 0 ; i<len(res); i++ ) {
        a = (double*) ll_element(res_list, i);
        b = &res[i];
        worked = 0 == memcmp(a, b, sizeof(double));
    }

    return worked;
}

/*
 * Datasize tests:
 */
static int test_datasize_of_first() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    int result;
    LinkedList *list1 = empty_linkedlist();
    LinkedList *list2 = empty_linkedlist();
    double d_data = 5.0;
    int i_data = 1;

    ll_push(list1, &d_data, sizeof( d_data ) );
    ll_push(list2, &i_data, sizeof( i_data ) );

    result = (sizeof(d_data) == ll_datasize_first(list1));
    result = result && (sizeof(i_data) == ll_datasize_first(list2));

    return result;
}

static int test_datasize_of_last() { 
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    unsigned int i;
    LinkedList *list = empty_linkedlist();
    double data[] = { 1.0, 2.0, 3.0 };

    for( i = 0 ; i < len(data) ; i++ )
        ll_push( list, &data[i], sizeof(data[i]) );

    return ( sizeof(data[2]) == ll_datasize_last(list) );
}

static int test_datasize_of_element() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    int result = 1;
    unsigned int i;
    LinkedList *list = empty_linkedlist();
    double data[] = { 1.0, 2.0, 3.0 };

    for( i = 0 ; i < len(data) ; i++ )
        ll_push( list, &data[i], sizeof(data[i]) );

    for( i = 0 ; i < len(data) ; i++ )
        result = result && ll_datasize_by_index(list,i) == sizeof(data[i]);

    return result;
}

static int test_datasize_of_list() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    size_t expected_size;
    unsigned int i;
    LinkedList *list = empty_linkedlist();
    double data[] = { 1.0, 2.0, 3.0 };

    for( i = 0 ; i < len(data) ; i++ )
        ll_push( list, &data[i], sizeof(data[i]) );

    expected_size = sizeof(LinkedList) + 
        (sizeof(LinkedListElement)+sizeof(double)) * 3;

    return ( ll_datasize_list(list) == expected_size );
}

#ifdef LL_PRINTABLE
static int test_print() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    unsigned int i;
    LinkedList *list = empty_linkedlist();
    double data[] = { 1.0, 2.0, 3.0 };
    for( i = 0 ; i < len(data) ; i++ )
        ll_push( list, &data[i], sizeof(double));

    ll_print(list, print_element ); // if no memory corruption, this was right
    printf("\n");
    return 1;
}

static int test_print_binary() {
    int d = depends(test_creating_and_removing);
    d = d && depends(test_pushing);
    if (!d) return 0;

    unsigned int i;
    LinkedList *list = empty_linkedlist();
    double data[] = { 1.0, 2.0, 3.0 };
    for( i = 0 ; i < len(data) ; i++ )
        ll_push( list, &data[i], sizeof(double));

    ll_print_binary(list); // if no memory corruption, this was right
    printf("\n");
    return 1;
}
#endif //LL_PRINTABLE

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
int condition_is_bigger_three( void* value, size_t size ) {
    // TODO: memcmp function should be used!
    /*
    double three = 3;
    return memcmp( value, &three, size ) == 1;
    */
    printf("using unused parameter 'size': %lu\n", size);
    return *(double*)value > 3.0;
}

/*
 * do for each function for testing for each and for each by cond function
 */
int do_foreach_inc( void *value, size_t size) {
    printf("using unused parameter 'size': %lu\n", size);
    (*(double*)value)++;
    return 1;
};

static void cleanup(LinkedList *l) {
    ll_destroy(l);
}

/*
 * Helper for printing a linkedlist-element
 */
#ifdef LL_PRINTABLE
void print_element(void* element, size_t datasize) {
    printf("%f [size: %lu], ", *(double*)element, datasize);
}
#endif // LL_PRINTABLE

/*
 * Main
 */

int main(void) {
    unsigned int i;
    int worked = 1;
    for( i = 0; tests[i].desc && worked; i++ ) {
        worked = test_exec( &tests[i] );
    }

    return !worked;
}
