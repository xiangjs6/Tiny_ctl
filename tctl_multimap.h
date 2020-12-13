//
// Created by xjs on 2020/11/19.
//

#ifndef TINY_CTL_TCTL_MULTIMAP_H
#define TINY_CTL_TCTL_MULTIMAP_H
#include "tctl_rb_tree.h"
#include "tctl_def.h"

typedef struct {
    rb_tree t;
} __private_multimap;

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
    byte __obj_private[sizeof(__private_multimap)];
} multimap;

void init_multimap(multimap *p_multimap, size_t key_size, size_t val_size, Compare);
void destory_multimap(multimap *p_multimap);
multimap creat_multimap(size_t key_size, size_t val_size, Compare);
#endif //TINY_CTL_TCTL_MULTIMAP_H
