#include "map.h"

/*
 * ==========================
 * Static function prototypes
 * ==========================
 */
static signed int compare_elements(MapElement *a, MapElement *b);
static signed int compare_keys( void *keya, 
                                size_t keyasize, 
                                void* keyb, 
                                size_t keybsize);
static MapElement * find_element(   Map *m, 
                                    void *key, 
                                    size_t keysize, 
                                    unsigned int range_start,
                                    unsigned int range_end );

#define swap_elements(a,b) do { \
        MapElement *x = (MapElement*) malloc(sizeof(MapElement));   \
        x = memcpy(x, a, sizeof(MapElement));                       \
        (a) = memcpy(a, b, sizeof(MapElement));                     \
        (b) = memcpy(b, x, sizeof(MapElement));                     \
        free(x);                                                    \
    } while(0);

/*
 * ===============================
 * Static function implementations
 * ===============================
 */

/*
 * @return -1 when a is bigger, 0 if they are equal, 1 if b is bigger
 */
#define compare_elements(a,b) compare_keys(a->key,a->key_size,b->key,b->key_size)

/*
 * Comares keys
 *
 * @return -1 when keya is bigger, 0 if they are equal, 1 if keyb is bigger
 *
 * Interal key sorting is done by some simple rules:
 * 1) keys with longer key_size are greater
 * 2) keys with equal key_size are compared by memcmp()
 */
static signed int compare_keys( void *keya, 
                                size_t keyasize, 
                                void* keyb, 
                                size_t keybsize) {

    if( keyasize != keybsize ) {
        if( keyasize > keybsize )
            return -1;
        /* else */
            return 1;
    }
    else
        return memcmp( keya, keyb, keyasize /* or keybsize */ );
} 

/*
 * internal function to search for a element in the map 
 *
 * @return the MapElement or NULL if it was not found
 *
 * @param m the map where to search in 
 * @param key the key to identify the element 
 * @param keysize the size of the key 
 * @param range_start (internal) start of the range to search in
 * @param range_end (internal) end of the range to search in
 */
static MapElement * find_element(   Map *m, 
                                    void *key, 
                                    size_t keysize, 
                                    unsigned int range_start,
                                    unsigned int range_end ) {

    MapElement *result = NULL;
    MapElement *test;

    if( range_end - range_start == 0 ) {
        test = &(m->elements[range_start]);
        if ( 0 == compare_keys(test->key, test->key_size, key, keysize) ) {
            result = test;
        } 
        /* if keys are not equal, result = NULL */
    }
    else {
       unsigned int mid = (range_end - range_start) / 2;
       result = find_element(m, key, keysize, range_start, mid);
       if( result == NULL )
           result = find_element(m, key, keysize, mid, range_end);
    }

    return result;
}

/*
 * ========================
 * Function implementations
 * ========================
 */

/*
 * Allocates memory for a new map.
 *
 * @param prealloc allocate memory for elements when allocating memory for map.
 * @return Map*
 */
Map * map_new_map(  unsigned int prealloc, 
                    signed int (*cmpfunc)(void* key_x, void* key_y) ) {
    
    unsigned int i;
    Map * map = (Map*) malloc( sizeof(Map) + prealloc*sizeof(MapElement) );
    map->len = prealloc;
    map->cnt_set_elements = 0;
    map->cmpfunc = cmpfunc;

    for( i = 0 ; i < prealloc ; i++ ) {
        map->elements[i].value_size    = 0;
        map->elements[i].key           = NULL;
        map->elements[i].value         = NULL;
    }

    return map;
}

/*
 * Deletes map from memory
 *
 * @param map the map to delete
 *
 * Currently, this function does not remove the MapElements from the memory, i
 * guess. It has to be improved, surely!
 */
void map_delete_map(Map *m) {
    free(m);
}

/*
 * Inserts value at key. Returns a status, what happened.
 *
 * @param m the map where to insert
 * @param key the key where to insert the value_size
 * @param value the value for the key
 *
 * @return a MapInsertStatus type
 */
MapInsertStatus map_insert( Map *m, 
                            void *key, 
                            size_t keysize, 
                            void *value, 
                            size_t valuesize ) {

    /*
     * Here, some checks are required: has the map this key already? If the
     * config says, it's okay to have multiple keys, go on...
     */

    /*
     * Check if the map is full and memory needs to be allocated.
     *
     * If there is enough space for all keys, insert. Else allocate more mem.
     */
    if( m->len == m->cnt_set_elements ) {
        Map * res;
        res = (Map*) realloc(m, sizeof(Map) + 2*(m->len * sizeof(MapElement) ) );
        if( res == NULL ) goto r_unable_to_alloc;
        m = res;
        m->len *= 2;
    }

    unsigned int i;
    
    /* keep map sorted */
#define curr_e m->elements[i]
    for( i = 0; 
            i < m->len && 
            1 != compare_keys(curr_e.key, curr_e.key_size, key, keysize); 
            i++ );
#undef curr_e
    /* i points to the first key which is smaller or equal than the key to insert */

    MapElement * src = &(m->elements[i]);
    MapElement * dest = &(m->elements[i+1]);
    dest = memmove(dest, src, sizeof(MapElement)*(m->cnt_set_elements - (i-1)) );
    
    /* Copy the new data into the map */
    src->key        = (void*) malloc( keysize );
    if( src->key == NULL ) goto r_unable_to_alloc;
    src->key        = memcpy( src->key, key, keysize );
    src->key_size   = keysize;

    src->value      = (void*) malloc( valuesize );
    src->value      = memcpy( src->value, value, valuesize );
    if( src->value == NULL ) goto r_unable_to_alloc;

    src->value_size = valuesize; 

    return MAP_OK;

r_unable_to_alloc:
    return MAP_UNABLE_TO_ALLOC;
r_key_exists:
    return MAP_KEY_EXISTS;
r_unknown_error:
    return MAP_UNKNOWN_ERROR;
}

/*
 * find alias for user.
 *
 * @return the MapElement or NULL if it was not found
 *
 * @param m the map where to search in 
 * @param key the key to identify the element 
 * @param keysize the size of the key to compare with one in the map
 */
MapElement * map_find(Map *m, void *key, size_t keysize) {
    return find_element(m, key, keysize, 0, m->cnt_set_elements-1);
}

