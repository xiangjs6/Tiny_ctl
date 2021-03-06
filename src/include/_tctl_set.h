//
// Created by xjs on 2021/2/5.
//

#ifndef TINY_CTL__TCTL_SET_H
#define TINY_CTL__TCTL_SET_H
#include "../../include/tctl_set.h"
#include "_tctl_class.h"
struct SetSelector {
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
    void (*swap)(Set);
};          

const void *_SetClass(void);
extern const struct SetSelector *_SetS;
#endif //TINY_CTL__TCTL_SET_H
