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

TESTUTILS= ./testutils/src/testutils.c
TESTHEADERS = -I./testutils/src/
TESTUTILS_OUT=${BIN}/testutils

LL = ${SRC}/linkedlist/linkedlist.c
LL_OUT = ${BIN}/linkedlist

LL_TEST = ${TESTS}/ll_test.c
LL_TEST_OUT = ${BIN}/ll_test

STACK = ${SRC}/stack/stack.c
STACK_OUT = ${BIN}/stack

STACKTEST = ./tests/stack_test.c
STACKTEST_OUT = ${BIN}/stacktest

LL_FLAGS = -D LL_PRINTABLE
STACK_FLAGS = -D STACK_PRINTABLE

#
# just do it...
#
clean:
	@rm ${BIN}/*

clean_outs:
	@rm ${BIN}/*.o

test_utils:
	${CC} ${CFLAGS} ${HEADERS} ${TESTHEADERS} ${TESTUTILS} -o ${TESTUTILS_OUT}.o

#
# stack stuff
#

stack:
	${CC} ${HEADERS} ${CFLAGS} ${STACK} ${STACK_FLAGS} -o ${STACK_OUT}.o

stacktest: stack testutils
	@echo "Compiling test files"
	${CC}  ${HEADERS} ${TESTHEADERS} ${CFLAGS} ${STACKTEST} ${STACK_FLAGS} -o ${STACKTEST_OUT}.o
	@echo ""
	@echo "Linking test files"
	${CC} ${TESTUTILS_OUT}.o ${STACK_OUT}.o ${STACKTEST_OUT}.o -o ${STACKTEST_OUT}

#
# linkedlist stuff
#

ll:
	${CC} ${HEADERS} ${CFLAGS} ${LL_FLAGS} ${LL} -o ${LL_OUT}.o

ll_test: ll testutils
	@echo "Compiling test files"
	${CC} ${HEADERS} ${TESTHEADERS} ${CFLAGS} ${DEBUG} ${LL_FLAGS} ${LL_TEST} -o ${LL_TEST_OUT}.o 
	@echo ""
	@echo "Linking test files"
	${CC} ${TESTUTILS_OUT}.o ${LL_OUT}.o ${LL_TEST_OUT}.o -o ${LL_TEST_OUT}

#
# make all tests and runners for them
#

tests: testutils stack stacktest ll ll_test

testrun: tests
	${BIN}/stacktest && sleep 5 && ${BIN}/ll_test

run_ll_test: testutils ll ll_test
	${BIN}/ll_test

run_stack_test: testutils stack stacktest
	${BIN}/stacktest
