/**
 * smal definitin of an large array > 1 MB
 *
 * using an underlying field
 */
#ifndef LONGARRAY
#define LONGARRAY
#include "Array.h"
#include <math.h>

/**
 * defines an new LongArray struct with a given type as content
 * and a given Name
 */
#define DEFINE_LONG_ARRAY(TYPE, NAME)   \
typedef struct {                        \
  TYPE **ptr;                           \
  u_int64_t length;            \
  u_int64_t col_len;           \
  u_int64_t max_rows;          \
  u_int64_t cur_rows;          \
  u_int64_t max_len;           \
} NAME;


/**
 * Initialize an given Array with an given maximal length
 * also initialize rand
 */
#define LONG_ARY_INIT(TYPE, ARY, MAX_LEN)                               \
  ARY.col_len = ARY.max_rows = (u_int64_t) sqrt(MAX_LEN) + 1;  \
  ARY.ptr = (TYPE **) malloc(sizeof(TYPE *) * ARY.col_len);             \
  ARY.ptr[0] = (TYPE *) malloc(sizeof(TYPE) * ARY.col_len);             \
  ARY.length = 0;                                                       \
  ARY.max_len = MAX_LEN;                                                \
  ARY.cur_rows = 1;                                                     \
  srand(time(NULL) * rand());

/**
 * Makro for accressing an Long Arry index
 */
#define LONG_ARY_AT(ARY, INDEX)\
    ARY.ptr[INDEX / ARY.col_len][INDEX % ARY.col_len]
  
/**
 * Makro for the address of an Long Array Element at index
 */
#define LONG_ARY_PTR_AT(ARY, INDEX)\
  (ARY.ptr[INDEX / ARY.col_len] + (INDEX % ARY.col_len))

/**
 * Makro for Accressing the Last Element of the given Array
 */
#define LONG_ARY_LAST(ARY) LONG_ARY_AT(ARY, (ARY.length - 1))

/**
 * Makro for Accressing the Element after the Last Element of the given Array
 */
#define LONG_ARY_AT_LENGTH(ARY) LONG_ARY_AT(ARY, ARY.length)

/**
 * Insert an given element at the end of a given Array if possible
 *
 * NOTE the Arry content type should be the same as the type of 
 * the given element and TYPE
 *
 * TODO Realloc space if needed
 */
#define LONG_ARY_ADD(TYPE, ARY, E)                                                                   \
  if (ARY.length < ARY.max_len) {                                                                    \
    if (ARY.length / ARY.col_len == ARY.cur_rows) {                                                  \
      ARY.ptr[ARY.cur_rows] = (TYPE *) malloc(sizeof(TYPE) * ARY.col_len);                           \
      ARY.cur_rows++;                                                                                \
    }                                                                                                \
                                                                                                     \
    LONG_ARY_AT(ARY, ARY.length) = E;                                                                \
    ARY.length++;                                                                                    \
  } else {                                                                                           \
    printf("[ERROR] couldn't add element to Array max size (%" PRIu64 ") reatched\n", ARY.length);   \
  }

/**
 * Reserves Space for one more Element if nedded
 *
 * NOTE the Arry content type should be the same as the type of 
 * the given element and TYPE
 *
 * TODO Realloc space if needed
 */
#define LONG_ARY_ADD_SPACE(TYPE, ARY)                                                                        \
  if (ARY.length < ARY.max_len) {                                                                            \
    if (ARY.length / ARY.col_len == ARY.cur_rows) {                                                          \
      ARY.ptr[ARY.cur_rows] = (TYPE *) malloc(sizeof(TYPE) * ARY.col_len);                                   \
      ARY.cur_rows++;                                                                                        \
    }                                                                                                        \
  } else {                                                                                                   \
    printf("[ERROR] couldn't reserve more space in Array, max size (%" PRIu64 ") reatched\n", ARY.length);   \
  }

/**
 * Removes and Returns the Last Element of a given Array if possible
 *
 * NOTE the Arry content type should be the same as the type of 
 * the given element and TYPE
 */
#define LONG_ARY_PULL(ARY, VALUE)                   \
  if (ARY.length > 0) {                             \
    ARY.length--;                                   \
    VALUE = LONG_ARY_AT(ARY, ARY.length);           \
  } else {                                          \
    printf("[ERROR] no more elements in Array\n");  \
  }

/**
 * Extract a random element from a given array
 * and saves it into a given value if possible
 *
 * note it changes the order of the given array
 * NOTE the Arry content type should be the same as th type of 
 * the given value
 */
#define LONG_ARY_EXTRACT(ARY, VALUE)\
  if (ARY.length > 0) {\
    u_int64_t ary_ext_i = RAND_INDEX(ARY);\
    VALUE = LONG_ARY_AT(ARY, ary_ext_i);\
    ARY.length--;\
    LONG_ARY_AT(ARY, ary_ext_i) = LONG_ARY_AT(ARY, ARY.length);\
  }

/**
 * Frees an given Long Array
 */
#define LONG_ARY_FREE(ARY)                                            \
  {                                                                   \
    u_int64_t laryfree_i;                                    \
    for (laryfree_i = 0; laryfree_i < ARY.cur_rows; laryfree_i++) {   \
      free(ARY.ptr[laryfree_i]);                                     \
    }                                                                 \
    free(ARY.ptr);                                                    \
  }

/**
 * Clones an given LongArray into DST.
 *
 * Note both Arrays should have the same TYPE
 */
#define LONG_ARY_CLONE(TYPE, DST, SRC)                                                      \
  {                                                                                         \
    DST.length   = SRC.length;                                                              \
    DST.col_len  = SRC.col_len;                                                             \
    DST.max_rows = SRC.max_rows;                                                            \
    DST.cur_rows = SRC.cur_rows;                                                            \
    DST.max_len  = SRC.max_len;                                                             \
    DST.ptr      = (TYPE **) malloc(sizeof(TYPE *) * DST.col_len);                          \
                                                                                            \
    u_int64_t lary_clone_i;                                                                 \
    for (lary_clone_i = 0; lary_clone_i < DST.cur_rows; lary_clone_i++) {                   \
      DST.ptr[lary_clone_i] = (TYPE *) malloc(sizeof(TYPE) * DST.col_len);                  \
      memcpy(DST.ptr[lary_clone_i], SRC.ptr[lary_clone_i], sizeof(TYPE) * DST.col_len);     \
    }                                                                                       \
  }

/**
 * Some shorter Markros
 */
#ifndef LARY
#define DEF_LARY(TYPE, NAME)            DEFINE_LONG_ARRAY(TYPE, NAME)
#define LARY_INIT(TYPE, ARY, MAX_LEN)   LONG_ARY_INIT(TYPE, ARY, MAX_LEN)
#define LARY_AT(ARY, INDEX)             LONG_ARY_AT(ARY, INDEX)
#define LARY_PTR(ARY, INDEX)            LONG_ARY_PTR_AT(ARY, INDEX)
#define LARY_RAND_PULL(ARY, VALUE)      LONG_ARY_EXTRACT(ARY, VALUE)
#define LARY_ADD(TYPE, ARY, E)          LONG_ARY_ADD(TYPE, ARY, E)
#define LARY_PUSH(TYPE, ARY, E)         LONG_ARY_ADD(TYPE, ARY, E)
#define LARY_PULL(ARY, VALUE)           LONG_ARY_PULL(ARY, VALUE)
#define LARY_FREE(ARY)                  LONG_ARY_FREE(ARY)
#define LARY_CLONE(TYPE, DST, SRC)      LONG_ARY_CLONE(TYPE, DST, SRC)
#define LARY_LAST(ARY)                  LONG_ARY_LAST(ARY)
#define LARY_AT_LEN(ARY)                LONG_ARY_AT_LENGTH(ARY)
#define LARY_ADD_SPACE(TYPE, ARY)       LONG_ARY_ADD_SPACE(TYPE, ARY)
#endif

#endif // end of LongArray.h
