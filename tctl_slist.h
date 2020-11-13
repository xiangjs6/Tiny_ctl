//
// Created by xjs on 2020/9/29.
//

#ifndef TINY_CTL_TCTL_SLIST_H
#define TINY_CTL_TCTL_SLIST_H

#include "tctl_def.h"
#include "tctl_iterator.h"
typedef void *__slist_iter;

struct __slist_node{
    struct __slist_node *next;
    byte data[0];
} ;

typedef struct {
    size_t memb_size;
    struct __slist_node head;
    struct {
        struct __inner_iterator start_iter;
        __slist_iter start_ptr;
    };
    struct {
        struct __inner_iterator finish_iter;
        __slist_iter finish_ptr;
    };
} __private_slist;

typedef struct slist {
    const IterType (*begin)(void);
    const IterType (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*swap)(struct slist*);
    void *(*front)(void);
    void (*push_front)(void *x);
    void (*pop_front)(void);
    IterType (*insert_after)(IterType iter, void *x);
    IterType (*erase_after)(IterType iter);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_slist)];
} slist;

void init_slist(slist *p_slist, size_t memb_size);
void destory_slist(slist *p_slist);
slist creat_slist(size_t memb_size);
#endif //TINY_CTL_TCTL_SLIST_H
