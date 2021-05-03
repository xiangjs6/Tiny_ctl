//
// Created by xjs on 2020/9/12.
//

#ifndef TINY_CTL_TCTL_DEQUE_H
#define TINY_CTL_TCTL_DEQUE_H

#include "tctl_metaclass.h"
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_class.h"

#define DEQUE_FUNC                                    \
struct {                                              \
    CLASS_FUNC;                                       \
    Iterator (*begin)(void);                          \
    Iterator (*end)(void);                            \
    void* (*front)(void);                             \
    void* (*back)(void);                              \
    size_t (*size)(void);                             \
    bool (*empty)(void);                              \
    void (*push_back)(const void *x);                 \
    void (*push_front)(const void *x);                \
    void (*pop_back)(void);                           \
    void (*pop_front)(void);                          \
    Iterator (*erase)(Iterator iter);                 \
    Iterator (*insert)(Iterator iter, const void *x); \
    void (*resize)(size_t new_size);                  \
    void (*clear)(void);                              \
    void (*swap)(Deque _v);                           \
}

typedef struct _Deque *Deque;
struct _Deque {
    METAOBJECT_HEAD(DEQUE_FUNC);
};

const void *_Deque(void);
#define DEQUE Deque: _Deque()
#endif //TINY_CTL_TCTL_DEQUE_H
