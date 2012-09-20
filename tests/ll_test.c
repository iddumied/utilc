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
/*
static bool test_get_by_index(void);
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
/*
 * Prototypes : Helper functions 
 */
static void testing(char*);
static void success(char*);
static void failure(char*);
static void cleanup(LinkedList*);

/*
 * function definitions : testing functions
 */

static bool test_creating_and_removing() {
    testing("Creating");
    double value = 5.00;
    LinkedList *list = linkedlist( &value );
    success("Creating");

    testing("Deleting");
    ll_destroy(list);
    success("Deleting");
    printf("\n");
    return true;
}

static bool test_pushing() {
    testing("Pushing a value");
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *list = linkedlist( &value1 );
    ll_push(list, &value2);
    success("Pushing a value");
    cleanup(list);

    return true;
}

static bool test_poping() {
    char *desc = "Poping a value";
    testing(desc);
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *poped_ptr = ((double*) ll_pop(l));

    if( poped_ptr == &value1 ) {
        success(desc);
        cleanup(l);
        return true;
    }
    else {
        failure(desc);
        cleanup(l);
        return false;
    }
}

static bool test_get_first() {
    char *desc = "get first";
    testing(desc);
    double value = 5.00;
    LinkedList *l = linkedlist(&value);
    double *res = (double*) ll_first(l);
    if( res == &value ) {
        success(desc);
        cleanup(l);
        return true;
    }
    else {
        failure(desc);
        cleanup(l);
        return false;
    }
}

static bool test_get_last() {
    char *desc = "get last";
    testing(desc);
    double value1 = 5.00;
    double value2 = 6.00;
    LinkedList *l = linkedlist(&value1);
    ll_push(l, &value2);
    double *res = (double*) ll_last(l);

    if( res == &value2 ) {
        success(desc);
        cleanup(l);
        return true;
    }
    else {
        failure(desc);
        cleanup(l);
        return false;
    }
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

static void failure(char* desc) {
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

int main( int argc, char ** argv ) {
    bool create_remove, push, pop, getf, getl;
    if( create_remove = test_creating_and_removing() ) {
        push = test_pushing();
        pop = test_poping();
        getf = test_get_first();
        getl = test_get_last();
    }
    else {
        printf("Creating and removing linkedlist does not work,\naborting.\n");
        return 1; 
    }
}
