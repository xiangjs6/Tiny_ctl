//
// Created by xjs on 2020/12/19.
//

#ifndef TINY_CTL_TCTL_UNORDERED_SET_H
#define TINY_CTL_TCTL_UNORDERED_SET_H

#include "tctl_hashtable.h"
#include "tctl_def.h"

typedef struct {
    hashtable ht;
} __private_unordered_set;

typedef struct unordered_set{
    IterType (*begin)(void);
    IterType (*end)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    size_t (*max_size)(void);
    IterType (*insert)(void*);
    void (*erase)(IterType);
    void (*clear)(void);
    IterType (*find)(void*);
    size_t (*count)(void*);
    size_t (*bucket_count)(void);
    size_t (*max_bucket_count)(void);
    void (*reserve)(size_t);
    void (*swap)(struct unordered_set*);
    byte __obj_private[sizeof(__private_unordered_set)];
} unordered_set;

void init_unordered_set(unordered_set *p_s, size_t memb_size, Compare equal, HashFunc hash);
void destory_unordered_set(unordered_set *p_s);
unordered_set creat_unordered_set(size_t memb_size, Compare equal, HashFunc hash);
#endif //TINY_CTL_TCTL_UNORDERED_SET_H
