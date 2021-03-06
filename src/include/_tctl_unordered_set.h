//
// Created by xjs on 2021/2/8.
//

#ifndef TINY_CTL__TCTL_UNORDERED_SET_H
#define TINY_CTL__TCTL_UNORDERED_SET_H
#include "../../include/tctl_unordered_set.h"
#include "_tctl_class.h"
struct Unordered_SetSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*erase)(Iterator iter);
    Iterator (*insert)(const void *x);
    size_t (*count)(const void *x);
    Iterator (*find)(const void *x);
    size_t (*bucket_count)(void);
    size_t (*max_bucket_count)(void);
    void (*reserve)(size_t);
    void (*clear)(void);
    void (*swap)(Unordered_Set);
};          

const void *_Unordered_SetClass(void);
extern const struct Unordered_SetSelector *_Unordered_SetS;
#endif //TINY_CTL__TCTL_UNORDERED_SET_H
