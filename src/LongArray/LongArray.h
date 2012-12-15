#ifndef __LONG_ARRAY_H__
#define __LONG_ARRAY_H__

/**
 * defines an new Long Array struct with a given type as content
 * and a given Name
 */
#define DEFINE_LONG_ARRAY(TYPE, NAME)                                         \
typedef struct {                                                              \
  TYPE **ptr;                                                                 \
  uint64_t length;                                                            \
  uint64_t col_len;                                                           \
  uint64_t max_rows;                                                          \
  uint64_t cur_rows;                                                          \
  uint64_t max_len;                                                           \
} NAME;


/**
 * Initialize an given Long Array with an given block length
 * also initialize rand
 *
 * NOTE: TYPE should be the same you used by initalizing the given Long Array
 */
#define LONG_ARY_INIT(TYPE, ARY, LEN)                                         \
  ARY.col_len = ARY.max_rows = (uint64_t) LEN;                                \
  ARY.ptr     = (TYPE **) malloc(sizeof(TYPE *) * ARY.col_len);               \
  ARY.ptr[0]  = (TYPE *) malloc(sizeof(TYPE) * ARY.col_len);                  \
  ARY.length  = 0;                                                            \
  ARY.max_len = LEN;                                                          \
  ARY.cur_rows = 1;                                                           \
  srand(time(NULL) * rand());

/**
 * Makro for accressing an Long Array index
 */
#define LONG_ARY_AT(ARY, INDEX)                                               \
    ARY.ptr[INDEX / ARY.col_len][INDEX % ARY.col_len]
  
/**
 * Makro to access the address of an Long Array element at a given index
 */
#define LONG_ARY_PTR_AT(ARY, INDEX)                                           \
  (ARY.ptr[INDEX / ARY.col_len] + (INDEX % ARY.col_len))

/**
 * Makro to access the last element of the given Long Array
 */
#define LONG_ARY_LAST(ARY) LONG_ARY_AT(ARY, (ARY.length - 1))

/**
 * Insert an given element at the end of a given Long Array
 */
#define LONG_ARY_ADD(ARY, E)                                                  \
  if (ARY.length >= ARY.max_len) {                                            \
    ARY.max_len *= 2;                                                         \
    ARY.ptr = realloc(ARY.ptr, ARY.max_len * sizeof(*ARY.ptr));               \
  }                                                                           \
                                                                              \
  if (ARY.length / ARY.col_len == ARY.cur_rows) {                             \
    ARY.ptr[ARY.cur_rows] = malloc(sizeof(**ARY.ptr) * ARY.col_len);          \
    ARY.cur_rows++;                                                           \
  }                                                                           \
                                                                              \
  LONG_ARY_AT(ARY, ARY.length) = E;                                           \
  ARY.length++;                                                               

/**
 * Returns the last element of a given Array if possible
 *
 * @return the last element of a givven Long Array or NULL 
 * if the Array is emply
 */
#define LONG_ARY_PULL(ARY)                                                    \
  (ARY.length > 0) ? LONG_ARY_LAST(ARY) : NULL;                               \
  if (ARY.length > 0) {                                                       \
    ARY.length--;                                                             \
  }

/**
 * Extract a random element from a given Long Array
 * and saves it in a given value
 * NOTE it changes the order of the given array
 */
#define LONG_ARY_EXTRACT(ARY, VALUE)\
  if (ARY.length > 0) {                                                       \
    u_int64_t ary_ext_i = RAND_INDEX(ARY);                                    \
    VALUE = LONG_ARY_AT(ARY, ary_ext_i);                                      \
    LONG_ARY_AT(ARY, ary_ext_i) = LONG_ARY_LAST(ARY);                         \
    ARY.length--;                                                             \
  }

/**
 * Frees an given Long Array
 */
#define LONG_ARY_FREE(ARY)                                                    \
  {                                                                           \
    u_int64_t laryfree_i;                                                     \
    for (laryfree_i = 0; laryfree_i < ARY.cur_rows; laryfree_i++) {           \
      free(ARY.ptr[laryfree_i]);                                              \
    }                                                                         \
    free(ARY.ptr);                                                            \
  }

/**
 * Clones an given LongArray into DST.
 *
 * Note both Arrays should have the same TYPE
 */
#define LONG_ARY_CLONE(DST, SRC)                                              \
  {                                                                           \
    DST.length   = SRC.length;                                                \
    DST.col_len  = SRC.col_len;                                               \
    DST.max_rows = SRC.max_rows;                                              \
    DST.cur_rows = SRC.cur_rows;                                              \
    DST.max_len  = SRC.max_len;                                               \
    DST.ptr      = malloc(sizeof(*SRC.ptr) * DST.col_len);                    \
                                                                              \
    u_int64_t lary_clone_i;                                                   \
    for (lary_clone_i = 0; lary_clone_i < DST.cur_rows; lary_clone_i++) {     \
      DST.ptr[lary_clone_i] = malloc(sizeof(**SRC.ptr) * DST.col_len);        \
      memcpy( DST.ptr[lary_clone_i],                                          \
              SRC.ptr[lary_clone_i],                                          \
              sizeof(**SRC.ptr) * DST.col_len);                               \
    }                                                                         \
  }

/**
 * Some shorter Markros
 */
#ifndef NO_LARY
#define DEF_LARY(TYPE, NAME)            DEFINE_LONG_ARRAY(TYPE, NAME)
#define LARY_INIT(TYPE, ARY, MAX_LEN)   LONG_ARY_INIT(TYPE, ARY, MAX_LEN)
#define LARY_AT(ARY, INDEX)             LONG_ARY_AT(ARY, INDEX)
#define LARY_PTR(ARY, INDEX)            LONG_ARY_PTR_AT(ARY, INDEX)
#define LARY_RAND_PULL(ARY, VALUE)      LONG_ARY_EXTRACT(ARY, VALUE)
#define LARY_ADD(ARY, E)                LONG_ARY_ADD(ARY, E)
#define LARY_PUSH(ARY, E)               LONG_ARY_ADD(ARY, E)
#define LARY_PULL(ARY)                  LONG_ARY_PULL(ARY)
#define LARY_FREE(ARY)                  LONG_ARY_FREE(ARY)
#define LARY_CLONE(DST, SRC)            LONG_ARY_CLONE(DST, SRC)
#define LARY_LAST(ARY)                  LONG_ARY_LAST(ARY)
#endif

#endif // end of LongArray.h
