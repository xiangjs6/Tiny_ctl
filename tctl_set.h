//
// Created by xjs on 2020/11/16.
//

#ifndef TINY_CTL_TCTL_SET_H
#define TINY_CTL_TCTL_SET_H
#include "tctl_rb_tree.h"

typedef struct {
    rb_tree t;
} __private_set;

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
    byte __obj_private[sizeof(__private_set)];
} set;

void init_set(set *p_set, size_t memb_size, Compare);
void destory_set(set *p_set);
set creat_set(size_t memb_size, Compare);
#endif //TINY_CTL_TCTL_SET_H
