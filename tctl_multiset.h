//
// Created by xjs on 2020/11/19.
//

#ifndef TINY_CTL_TCTL_MULTISET_H
#define TINY_CTL_TCTL_MULTISET_H
#include "tctl_rb_tree.h"

typedef struct {
    rb_tree t;
} __private_multiset;

typedef struct {
    IterType (*begin)(void);
    IterType (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*erase)(IterType iter);
    IterType (*insert)(void *x);
    size_t (*count)(void *x);
    IterType (*find)(void *x);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_multiset)];
} multiset;

void init_multiset(multiset *p_multiset, size_t memb_size, Compare);
void destory_multiset(multiset *p_multiset);
multiset creat_multiset(size_t memb_size, Compare);
#endif //TINY_CTL_TCTL_MULTISET_H
