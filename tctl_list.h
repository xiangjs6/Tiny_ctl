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
    byte data[0];
};

typedef void *__list_iter;
/*typedef struct {
    void *val;
    struct __list_node *node;
} list_iter;*/

typedef struct {
    const size_t memb_size;
    struct __list_node *node;
    struct {
        struct __inner_iterator start_iter;
        __list_iter start_ptr;
    };
    struct {
        struct __inner_iterator finish_iter;
        __list_iter finish_ptr;
    };
} __private_list;

struct __list{
    void *(*at)(int);
    void (*push_front)(void *x);
    void (*push_back)(void *x);
    IterType (*erase)(IterType iter);
    IterType (*insert)(IterType iter, void *x);
    void (*pop_front)(void);
    void (*pop_back)(void);
    void (*clear)(void);
    void (*remove)(void *value);
    void (*unique)(void);
    void (*splice)(const IterType position, list *l, const IterType first, const IterType last);
    void (*merge)(list *l, Compare);
    void (*reverse)(void);
    void (*swap)(list *l);
    void (*sort)(Compare);
    const IterType (*begin)(void);
    const IterType (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void const *(*front)(void);
    void const *(*back)(void);
    byte __obj_private[sizeof(__private_list)];
};

void init_list(list *p_list, size_t memb_size);
void destory_list(list *p_list);
list creat_list(size_t memb_size);
#endif //TINY_CTL_TCTL_LIST_H
