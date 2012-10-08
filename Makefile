SRC = ./src
BIN = ./bin
TESTS= ./tests

CC = /usr/bin/gcc

# Warn
CFLAGS += -W -Wextra -Wall 

# -Wmissing-prototypes
#  Warn, if a prototype is missing
CFLAGS += -Wmissing-prototypes 

# -Wmissing-declarations
#  Warn, if function is not declared
CFLAGS += -Wmissing-declarations 

# -Wstrict-prototypes:
# 	Warn if a function is declared or defined without specifying 
# 	the argument types
CFLAGS += -Wstrict-prototypes 

# -Wreturn-type:
#  Return statement is required if function returns a value!
CFLAGS += -Wreturn-type 

# -g
#  I want to have debugging information
CFLAGS += -g 

# Write C99
CFLAGS += -std=c99

# Do not link
CFLAGS += -c

# Debug output please!
CFLAGS += -D DEBUG

#
# Some variables
#
HEADERS = -I./src/
TESTHEADERS = -I./tests/

TESTUTILS=${TESTS}/utilc_test_utils.c
TESTUTILS_OUT=${BIN}/utilc_test_utils

LL = ./src/linkedlist/linkedlist.c
LL_OUT = ./bin/linkedlist

LL_TEST = ./tests/ll_test.c
LL_TEST_OUT = ./bin/ll_test

STACK = ${SRC}/stack/stack.c
STACK_OUT = ${BIN}/stack

STACKTEST = ./tests/stack_test.c
STACKTEST_OUT = ${BIN}/stacktest

DEBUG = -g

#
# just do it...
#
clean:
	@rm ${BIN}/*

clean_outs:
	@rm ${BIN}/*.o

testutils:
	${CC} ${CFLAGS} ${HEADERS} ${TESTHEADERS} ${TESTUTILS} -o ${TESTUTILS_OUT}.o

#
# stack stuff
#

stack:
	${CC} ${HEADERS} ${CFLAGS} ${STACK} -o ${STACK_OUT}.o

stacktest: stack
	${CC}  ${HEADERS} ${TESTHEADERS} ${CFLAGS} ${STACKTEST} -o ${STACKTEST_OUT}.o

link_stacktest: testutils stack stacktest
	${CC} ${TESTUTILS_OUT}.o ${STACK_OUT}.o ${STACKTEST_OUT}.o -o ${STACKTEST_OUT}

#
# linkedlist stuff
#

ll:
	${CC} ${HEADERS} ${CFLAGS} ${LL} -o ${LL_OUT}.o

ll_test:
	${CC} ${HEADERS} ${TESTHEADERS} ${CFLAGS} ${DEBUG} ${LL_TEST} -o ${LL_TEST_OUT}.o 

link_ll_test: testutils ll ll_test
	${CC} ${TESTUTILS_OUT}.o ${LL_OUT}.o ${LL_TEST_OUT}.o -o ${LL_TEST_OUT}
