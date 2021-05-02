//
// Created by xjs on 2021/1/19.
//

#ifndef TINY_CTL__TCTL_LIST_H
#define TINY_CTL__TCTL_LIST_H
#include "../../include/tctl_list.h"
#include "_tctl_class.h"
struct ListSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    void* (*front)(void);
    void* (*back)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*push_back)(const void *x);
    void (*push_front)(const void *x);
    void (*pop_back)(void);
    void (*pop_front)(void);
    Iterator (*erase)(Iterator iter);
    Iterator (*insert)(Iterator iter, const void *x);
    void (*remove)(const void *x);
    void (*unique)(void);
    void (*splice)(Iterator position, List l, ...);
    void (*merge)(List l, Compare cmp);
    void (*reverse)(void);
    void (*sort)(Compare cmp);
    void (*clear)(void);
    void (*swap)(List _l);
};
const void *_ListClass(void);
extern const struct ListSelector *_ListS;
#endif //TINY_CTL__TCTL_LIST_H
