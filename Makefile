SOURCE_DIR = src
BUILD_DIR = bin

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

LL = ./src/linkedlist/linkedlist.c
LL_OUT = ./bin/linkedlist

LL_TEST = ./tests/ll_test.c
LL_TEST_OUT = ./bin/ll_test

STACK = ${SRC}/stack/stack.c
STACKTEST = ./tests/stack_tests.c
STACKTEST_OUT = ${BUILD_DIR}/stack

DEBUG = -g

#
# just do it...
#

all: ll tests 

stacktest:
	${CC} ${HEADERS} ${CFLAGS} ${STACKTEST} -o ${STACKTEST_OUT}.o

ll:
	${CC} ${HEADERS} ${CFLAGS} ${LL} -o ${LL_OUT}.o

tests: ll_test link_tests

ll_test:
	${CC} ${HEADERS} ${CFLAGS} ${DEBUG} ${LL_TEST} -o ${LL_TEST_OUT}.o 

link_tests:
	${CC} ${LL_OUT}.o ${LL_TEST_OUT}.o -o ${LL_TEST_OUT}
