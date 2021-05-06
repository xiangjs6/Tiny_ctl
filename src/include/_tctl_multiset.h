//
// Created by xjs on 2021/2/8.
//

#ifndef TINY_CTL__TCTL_MULTISET_H
#define TINY_CTL__TCTL_MULTISET_H
#include "../../include/tctl_multiset.h"
#include "_tctl_class.h"
struct MultiSetSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*erase)(Iterator iter);
    Iterator (*insert)(const void *x);
    size_t (*count)(const void *x);
    Iterator (*find)(const void *x);
    void (*clear)(void);
    void (*swap)(MultiSet);
};          

const void *_MultiSetClass(void);
extern const struct MultiSetSelector *_MultiSetS;
#endif //TINY_CTL__TCTL_MULTISET_H
