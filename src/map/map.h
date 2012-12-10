#ifndef __UTILC_MAP_H__
#define __UTILC_MAP_H__

#include <stdlib.h>
#include <string.h>

typedef struct {
    void * key;
    size_t key_size;
    void * value;
    size_t value_size;
} MapElement;

typedef struct {
    unsigned int len;
    unsigned int cnt_set_elements;
    signed int (* cmpfunc)( void * key_x, void * key_y );

    MapElement elements[];
} Map;

typedef enum {
    MAP_OK,
    MAP_UNABLE_TO_ALLOC,
    MAP_KEY_EXISTS,
    MAP_UNKNOWN_ERROR,

    MAP_STATUS_LEN,
} MapInsertStatus;

Map *               map_new_map(unsigned int prealloc, 
                                signed int (*cmpfunc)(void* key_x, void* key_y));
void                map_delete_map(Map *m);

MapInsertStatus     map_insert( Map *m, 
                                void *key, 
                                size_t keysize,
                                void *value,
                                size_t valuesize);
MapElement *        map_find(Map *m, void *key, size_t keysize);
void *              map_delete_by_key(void *key);
void *              map_delete_by_value(void *val);

#endif // __UTILC_MAP_H__
