#include "map.h"

/*
 * Static function prototypes
 */
static signed int compare_elements(MapElement *a, MapElement *b);
static signed int compare_keys(void *keya, void* keyb);

#define swap_elements(a,b) do { \
        MapElement *x = (MapElement*) malloc(sizeof(MapElement));   \
        x = memcpy(x, a, sizeof(MapElement));                       \
        (a) = memcpy(a, b, sizeof(MapElement));                     \
        (b) = memcpy(b, x, sizeof(MapElement));                     \
        free(x);                                                    \
    } while(0);

/*
 * Static function implementations
 */

/*
 * @return -1 when a is bigger, 0 if they are equal, 1 if b is bigger
 */
static signed int compare_elements(MapElement *a, MapElement *b) {
}

/*
 * Comares keys
 */
static signed int compare_keys(void *keya, void* keyb) {
} 

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
    MapElement * current_element;
    for( i = 0; i < m->len && 1 != compare_keys(m->elements[i].key, key) ; i++ );
    /* i points to the first key which is smaller or equal than the key to insert */

    MapElement * src = &(m->elements[i]);
    MapElement * dest = &(m->elements[i+1]);
    dest = memmove(dest, src, sizeof(MapElement)*(m->cnt_set_elements - (i-1)) );
    
    /* Copy the new data into the map */
    src->key    = (void*) malloc( keysize );
    if( src->key == NULL ) goto r_unable_to_alloc;
    src->key    = memcpy( src->key, key, keysize );

    src->value  = (void*) malloc( valuesize );
    src->value  = memcpy( src->value, value, valuesize );
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
