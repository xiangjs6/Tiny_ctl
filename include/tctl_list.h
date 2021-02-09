//
// Created by xjs on 2020/9/8.
//

#ifndef TINY_CTL_TCTL_LIST_H
#define TINY_CTL_TCTL_LIST_H

#include "tctl_iterator.h"
#include "tctl_common.h"
#include "tctl_metaclass.h"

#define LIST_FUNC                                    \
struct {                                             \
    CLASS_FUNC;                                      \
    Iterator (*begin)(void);                         \
    Iterator (*end)(void);                           \
    void* (*front)(void);                            \
    void* (*back)(void);                             \
    size_t (*size)(void);                            \
    bool (*empty)(void);                             \
    void (*push_back)(FormWO_t x);                   \
    void (*push_front)(FormWO_t x);                  \
    void (*pop_back)(void);                          \
    void (*pop_front)(void);                         \
    Iterator (*erase)(Iterator iter);                \
    Iterator (*insert)(Iterator iter, FormWO_t x);   \
    void (*remove)(FormWO_t x);                      \
    void (*unique)(void);                            \
    void (*splice)(Iterator position, List l, ...);  \
    void (*merge)(List l, Compare cmp);              \
    void (*reverse)(void);                           \
    void (*sort)(Compare cmp);                       \
    void (*clear)(void);                             \
    void (*swap)(List l);                            \
}

typedef struct _List *List;
struct _List {
    METAOBJECT_HEAD(LIST_FUNC);
};

Form_t _List(void);
#define LIST List : _List()
#endif //TINY_CTL_TCTL_LIST_H
