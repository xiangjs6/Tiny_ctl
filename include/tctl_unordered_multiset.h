//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_UNORDERED_MULTISET_H
#define TINY_CTL_TCTL_UNORDERED_MULTISET_H

#include "tctl_hashtable.h"
#include "tctl_def.h"

typedef struct {
    hashtable ht;
} __private_unordered_multiset;

typedef struct unordered_multiset{
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
    void (*swap)(struct unordered_multiset*);
    byte __obj_private[sizeof(__private_unordered_multiset)];
} unordered_multiset;

void init_unordered_multiset(unordered_multiset *p_s, size_t memb_size, Compare equal, HashFunc hash);
void destory_unordered_multiset(unordered_multiset *p_s);
unordered_multiset creat_unordered_multiset(size_t memb_size, Compare equal, HashFunc hash);
#endif //TINY_CTL_TCTL_UNORDERED_MULTISET_H
