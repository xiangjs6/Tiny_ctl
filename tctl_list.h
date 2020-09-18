//
// Created by xjs on 2020/9/8.
//

#ifndef TINY_CTL_TCTL_LIST_H
#define TINY_CTL_TCTL_LIST_H

#include "tctl_iterator.h"
#include "tctl_common.h"

typedef struct __list list;

struct __list_node {
    struct __list_node *pre;
    struct __list_node *next;
    void *data;
};

typedef struct {
    void *val;
    struct __list_node *node;
} list_iter;

typedef struct {
    const size_t memb_size;
    struct __list_node *node;
    struct {
        iterator const start;
        list_iter start_iter;
    } BYTE_ALIGNED;
    struct {
        iterator const finish;
        list_iter finish_iter;
    } BYTE_ALIGNED;
} __private_list;

struct __list{
    void *(*at)(int);
    void (*push_front)(void *x);
    void (*push_back)(void *x);
    list_iter *(*erase)(list_iter *iter);
    list_iter *(*insert)(list_iter *iter, void *x);
    void (*pop_front)(void);
    void (*pop_back)(void);
    void (*clear)(void);
    void (*remove)(void *value);
    void (*unique)(void);
    void (*splice)(list_iter *position, list *l, list_iter *first, list_iter *last);
    void (*merge)(list *l, bool (*cmp)(void const *, void const *));
    void (*reverse)(void);
    void (*swap)(list *l);
    void (*sort)(bool (*cmp)(void const *, void const *));
    void const * const * (*begin)(void);
    void const * const * (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void const *(*front)(void);
    void const *(*back)(void);
    byte __obj_private[sizeof(__private_list)];
};

void init_list(list *p_list, size_t memb_size);
void destory_list(list *p_list);
#endif //TINY_CTL_TCTL_LIST_H
