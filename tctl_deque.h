//
// Created by xjs on 2020/9/12.
//

#ifndef TINY_CTL_TCTL_DEQUE_H
#define TINY_CTL_TCTL_DEQUE_H

#include "tctl_object.h"
#include "tctl_def.h"
#include "tctl_iterator.h"

typedef struct {
    void *cur;
    void *first;
    void *last;
    void **map_node;
} __deque_iter;

typedef struct {
    size_t nmemb;
    size_t memb_size;
    void **mmap;
    size_t mmap_len;
    size_t block_nmemb;
    struct {
        struct __inner_iterator start_iter;
        __deque_iter start_ptr;
    };
    struct {
        struct __inner_iterator finish_iter;
        __deque_iter finish_ptr;
    };
} __private_deque;

typedef struct {
    __iterator const *(*begin)(void);
    __iterator const *(*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void *(*at)(int);
    void const *(*front)(void);
    void const *(*back)(void);
    void (*push_back)(void *x);
    void (*push_front)(void *x);
    void (*pop_back)(void);
    void (*pop_front)(void);
    __iterator *(*insert)(__iterator *iter, void *x);
    __iterator *(*erase)(__iterator *iter);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_deque)];
} deque;

void init_deque(deque *p_deque, size_t memb_size, size_t block_nmemb);
void destory_deque(deque *p_deque);
deque creat_deque(size_t memb_size, size_t block_nmemb);
#endif //TINY_CTL_TCTL_DEQUE_H
