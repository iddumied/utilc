#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define len(x) (sizeof(x)/sizeof(x[0]))
#define depends(x) __depends(__func__, x )

typedef struct {
    char *desc;
    int strict;
    int (*testfunc)(void);
    int result;
} Test;

extern Test tests[];

/*
 * Prototypes : Helper functions 
 */
void testing(char*);
void success(char*);
void failure(char*, int);
int __depends(const char*, int (*)(void));
int test_exists(Test *test);
int test_exec(Test *test);
