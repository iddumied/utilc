#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define len(x) (sizeof(x)/sizeof(x[0]))
#define depends(x) __depends(__func__, x )

typedef struct {
    char *desc;
    bool strict;
    bool (*testfunc)(void);
    bool result;
} Test;

extern Test tests[];

/*
 * Prototypes : Helper functions 
 */
void testing(char*);
void success(char*);
void failure(char*, bool);
bool __depends(const char*, bool (*)(void));
bool test_exists(Test *test);
bool test_exec(Test *test);
