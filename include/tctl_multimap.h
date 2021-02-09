//
// Created by xjs on 2020/11/19.
//

#ifndef TINY_CTL_TCTL_MULTIMAP_H
#define TINY_CTL_TCTL_MULTIMAP_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"
#include "tctl_utility.h"

#define MULTIMAP_FUNC               \
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
    void (*swap)(MultiMap);         \
}

typedef struct _MultiMap *MultiMap;
struct _MultiMap {
    METAOBJECT_HEAD(MULTIMAP_FUNC);
};

Form_t _MultiMap(void);
#define MULTIMAP MultiMap : _MultiMap()
#endif //TINY_CTL_TCTL_MULTIMAP_H
