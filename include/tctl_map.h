//
// Created by xjs on 2020/11/18.
//

#ifndef TINY_CTL_TCTL_MAP_H
#define TINY_CTL_TCTL_MAP_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"
#include "tctl_utility.h"

#define MAP_FUNC                    \
struct {                            \
    CLASS_FUNC;                     \
    Iterator (*begin)(void);        \
    Iterator (*end)(void);          \
    size_t (*size)(void);           \
    bool (*empty)(void);            \
    void (*erase)(Iterator iter);   \
    Iterator (*insert)(Pair x);     \
    size_t (*count)(FormWO_t x);    \
    Iterator (*find)(FormWO_t x);   \
    void (*clear)(void);            \
    void (*swap)(Map);              \
}

typedef struct _Map *Map;
struct _Map {
    METAOBJECT_HEAD(MAP_FUNC);
};

void initMap(void) __attribute__((constructor));
Form_t _Map(void);
#define MAP Map : _Map()
#endif //TINY_CTL_TCTL_MAP_H
