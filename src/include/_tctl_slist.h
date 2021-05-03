//
// Created by xjs on 2021/2/1.
//

#ifndef TINY_CTL__TCTL_SLIST_H
#define TINY_CTL__TCTL_SLIST_H
#include "../../include/tctl_slist.h"
#include "_tctl_class.h"
struct SlistSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    void* (*front)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*push_front)(const void *x);
    void (*pop_front)(void);
    Iterator (*erase_after)(Iterator iter);
    Iterator (*insert_after)(Iterator iter, const void *x);
    void (*remove)(const void *x);
    void (*unique)(void);
    void (*splice_after)(Iterator position, Slist l, ...);
    void (*merge)(Slist l, Compare cmp);
    void (*reverse)(void);
    void (*sort)(Compare cmp);
    void (*clear)(void);
    void (*swap)(Slist l);
};
const void *_SlistClass(void);
extern const struct SlistSelector *_SlistS;
#endif //TINY_CTL__TCTL_SLIST_H
