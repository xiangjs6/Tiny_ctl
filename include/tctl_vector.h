//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_common.h"
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_portable.h"
#include "tctl_class.h"

#define INHERIT_VECTOR                          \
struct {                                        \
    INHERIT_CLASS;                              \
    Iterator (*begin)(void);                    \
    Iterator (*end)(void);                      \
    void* (*front)(void);                       \
    void* (*back)(void);                        \
    size_t (*size)(void);                       \
    size_t (*capacity)(void);                   \
    bool (*empty)(void);                        \
    void (*push_back)(const void* x);           \
    void (*pop_back)(void);                     \
    Iterator (*erase)(Iterator iter);           \
    Iterator (*insert)(Iterator iter, void* x); \
    void (*resize)(size_t new_size);            \
    void (*clear)(void);                        \
    void (*swap)(struct _Vector *_v);           \
}

typedef struct _Vector {
    union {
        INHERIT_VECTOR *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
} *Vector;

void initVector(void) __attribute__((constructor));
Form_t _Vector(void);
#define VECTOR Vector : _Vector()
#endif //TINY_CTL_TCTL_VECTOR_H
