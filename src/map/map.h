#ifndef __UTILC_MAP_H__
#define __UTILC_MAP_H__

typedef struct {
    size_t value_size;
    void * key;
    void * value;
} MapElement;

typedef struct {
    unsigned int len;
    signed int (* cmpfunc)( void * key_x, void * key_y );

    MapElement elements[];
} Map;

typedef enum {
    MAP_OK,
    MAP_UNABLE_TO_ALLOC,
    MAP_KEY_EXISTS,
    MAP_OTHER_ERROR,

    MAP_STATUS_LEN,
} MapInsertStatus;

Map *               map_new_map(void);
void                map_delete_map(Map *m);

MapInsertStatus     map_insert(void *key, void *value);
void *              map_find(void *key);
void *              map_delete_by_key(void *key);
void *              map_delete_by_value(void *val);

#endif // __UTILC_MAP_H__
