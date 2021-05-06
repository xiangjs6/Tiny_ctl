//
// Created by xjs on 2021/2/8.
//

#ifndef TINY_CTL__TCTL_MULTIMAP_H
#define TINY_CTL__TCTL_MULTIMAP_H
#include "../../include/tctl_multimap.h"
#include "_tctl_class.h"
struct MultiMapSelector {
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
    void (*swap)(MultiMap);
};          

const void *_MultiMapClass(void);
extern const struct MultiMapSelector *_MultiMapS;
#endif //TINY_CTL__TCTL_MULTIMAP_H
