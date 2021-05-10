//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_UNORDERED_MAP_H
#define TINY_CTL_TCTL_UNORDERED_MAP_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"
#include "tctl_utility.h"

#define UNORDERED_MAP_FUNC            \
struct {                              \
    CLASS_FUNC;                       \
    Iterator (*begin)(void);          \
    Iterator (*end)(void);            \
    size_t (*size)(void);             \
    bool (*empty)(void);              \
    void (*erase)(Iterator iter);     \
    Iterator (*insert)(Pair x);       \
    size_t (*count)(const void *x);   \
    Iterator (*find)(const void *x);  \
    size_t (*bucket_count)(void);     \
    size_t (*max_bucket_count)(void); \
    void (*reserve)(size_t);          \
    void (*clear)(void);              \
    void (*swap)(Unordered_Map);      \
}

typedef struct _Unordered_Map *Unordered_Map;
struct _Unordered_Map {
    METAOBJECT_HEAD(UNORDERED_MAP_FUNC);
};

const void *_Unordered_Map(void);
#define UNORDERED_MAP Unordered_Map : _Unordered_Map()
#endif //TINY_CTL_TCTL_UNORDERED_MAP_H
