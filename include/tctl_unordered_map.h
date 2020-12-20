//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_UNORDERED_MAP_H
#define TINY_CTL_TCTL_UNORDERED_MAP_H

#include "tctl_hashtable.h"
#include "tctl_def.h"

typedef struct {
    hashtable ht;
} __private_unordered_map;

typedef struct unordered_map{
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
    void (*swap)(struct unordered_map*);
    byte __obj_private[sizeof(__private_unordered_map)];
} unordered_map;

void init_unordered_map(unordered_map *p_m, size_t key_size, size_t val_size, Compare equal, HashFunc hash);
void destory_unordered_map(unordered_map *p_m);
unordered_map creat_unordered_map(size_t key_size, size_t val_size, Compare equal, HashFunc hash);
#endif //TINY_CTL_TCTL_UNORDERED_MAP_H
