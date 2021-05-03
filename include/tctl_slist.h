//
// Created by xjs on 2020/9/29.
//

#ifndef TINY_CTL_TCTL_SLIST_H
#define TINY_CTL_TCTL_SLIST_H

#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
#include "tctl_class.h"

#define SLIST_FUNC                                          \
struct {                                                    \
    CLASS_FUNC;                                             \
    Iterator (*begin)(void);                                \
    Iterator (*end)(void);                                  \
    void* (*front)(void);                                   \
    size_t (*size)(void);                                   \
    bool (*empty)(void);                                    \
    void (*push_front)(const void *x);                      \
    void (*pop_front)(void);                                \
    Iterator (*erase_after)(Iterator iter);                 \
    Iterator (*insert_after)(Iterator iter, const void *x); \
    void (*remove)(const void *x);                          \
    void (*unique)(void);                                   \
    void (*splice_after)(Iterator position, Slist l, ...);  \
    void (*merge)(Slist l, Compare cmp);                    \
    void (*reverse)(void);                                  \
    void (*sort)(Compare cmp);                              \
    void (*clear)(void);                                    \
    void (*swap)(Slist l);                                  \
}

typedef struct _Slist *Slist;
struct _Slist {
    METAOBJECT_HEAD(SLIST_FUNC);
};

const void *_Slist(void);
#define SLIST Slist : _Slist()
#endif //TINY_CTL_TCTL_SLIST_H
