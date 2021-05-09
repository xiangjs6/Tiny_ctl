//
// Created by xjs on 2021/2/8.
//

#ifndef TINY_CTL__TCTL_UNORDERED_MULTISET_H
#define TINY_CTL__TCTL_UNORDERED_MULTISET_H
#include "../../include/tctl_unordered_multiset.h"
#include "_tctl_class.h"
struct Unordered_MultiSetSelector {
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
    void (*swap)(Unordered_MultiSet);
};          

const void *_Unordered_MultiSetClass(void);
extern const struct Unordered_MultiSetSelector *_Unordered_MultiSetS;
#endif //TINY_CTL__TCTL_UNORDERED_MULTISET_H
