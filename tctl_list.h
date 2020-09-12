//
// Created by xjs on 2020/9/8.
//

#ifndef TINY_CTL_TCTL_LIST_H
#define TINY_CTL_TCTL_LIST_H

#include "tctl_iterator.h"

typedef struct __list list;

struct __list_node {
    struct __list_node *pre;
    struct __list_node *next;
    void *data;
};

typedef struct {
    const size_t memb_size;
    struct __list_node *node;
    iterator start;
    iterator finish;
} __private_list;

struct __list{
    void *(*at)(int);
    void (*push_front)(void *x);
    void (*push_back)(void *x);
    iter_ptr (*erase)(iter_ptr iter);
    iter_ptr (*insert)(iter_ptr iter, void *x);
    void (*pop_front)(void);
    void (*pop_back)(void);
    void (*clear)(void);
    void (*remove)(void *value);
    void (*unique)(void);
    void (*splice)(iter_ptr position, list *l, iter_ptr first, iter_ptr last);
    void (*merge)(list *l, bool (*cmp)(iter_ptr, iter_ptr));
    void (*reverse)(void);
    void (*swap)(list *l);
    void (*sort)(bool (*cmp)(iter_ptr, iter_ptr));
    iter_ptr const *(*begin)(void);
    iter_ptr const *(*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void *(*front)(void);
    void *(*back)(void);
    byte __obj_private[sizeof(__private_list)];
};

void init_list(list *p_list, size_t memb_size);
list creat_list(size_t memb_size);
void destory_list(list *p_list);
#endif //TINY_CTL_TCTL_LIST_H
