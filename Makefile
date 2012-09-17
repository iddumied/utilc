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

#
# Some variables
#
HEADER_SEARCH_DIR = -I./src/

LL = ./src/linkedlist/linkedlist.c
LL_OUT = ./bin/linkedlist

#
# just do it...
#

all: ll 

ll:
	${CC} ${HEADER_SEARCH_DIR} ${CFLAGS} ${LL} -o ${LL_OUT}
