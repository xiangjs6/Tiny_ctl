//
// Created by xjs on 2021/2/5.
//

#ifndef TINY_CTL__TCTL_MAP_H
#define TINY_CTL__TCTL_MAP_H
#include "../../include/tctl_map.h"
#include "_tctl_class.h"
struct MapSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*erase)(Iterator iter);
    Iterator (*insert)(Pair x);
    size_t (*count)(const void *x);
    Iterator (*find)(const void *x);
    void (*clear)(void);
    void (*swap)(Map);
};          

const void *_MapClass(void);
extern const struct MapSelector *_MapS;
#endif //TINY_CTL__TCTL_MAP_H
