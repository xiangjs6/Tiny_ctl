//
// Created by xjs on 2020/12/14.
//

#ifndef TINY_CTL_TCTL_HASHTABLE_H
#define TINY_CTL_TCTL_HASHTABLE_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_vector.h"

struct __bucket_node {
    struct __bucket_node *next;
    byte data[0];
};

typedef struct {
    size_t memb_size;
    size_t nmemb;
    Compare equal;
    HashFunc hash;
    ExtractKey get_key;
    vector buckets;
    struct __bucket_node **start;
    struct __bucket_node **finish;
} __private_hashtable;

typedef struct {
    IterType (*begin)(void);
    IterType (*end)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    IterType (*insert_unique)(void*);
    IterType (*insert_equal)(void*);
    void (*erase)(IterType);
    void (*clear)(void);
    IterType (*find)(void*);
    size_t (*count)(void*);
    size_t (*bucket_count)(void);
    byte __obj_private[sizeof(__private_hashtable)];
} hashtable;

typedef struct {
    void *val;
    struct __bucket_node *cur;
    hashtable *ht;
} __hashtable_iter;

void init_hashtable(hashtable *p_ht, size_t memb_size, Compare equal, HashFunc hash, ExtractKey get_key);
void destory_hashtable(hashtable *p_ht);
hashtable creat_hashtable(size_t memb_size, Compare equal, HashFunc hash, ExtractKey get_key);
#endif //TINY_CTL_TCTL_HASHTABLE_H
