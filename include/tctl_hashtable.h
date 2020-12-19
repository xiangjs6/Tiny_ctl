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
    const size_t memb_size;
    size_t nmemb;
    Compare equal;
    HashFunc hash;
    ExtractKey get_key;
    size_t start;
    size_t finish;
    //上面成员会在copy函数中复制，所以要和下面成员分开
    vector buckets;
} __private_hashtable;

typedef struct hashtable{
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
    size_t (*max_bucket_count)(void);
    void (*resize)(size_t);
    void (*swap)(struct hashtable*);
    void (*copy_from)(const struct hashtable*);
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
