//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_TCTL_RB_TREE_H
#define TINY_CTL_TCTL_RB_TREE_H

#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
#include "tctl_class.h"

#define RB_TREE_FUNC                          \
struct {                                      \
    CLASS_FUNC;                               \
    Iterator (*begin)(void);                  \
    Iterator (*end)(void);                    \
    size_t (*size)(void);                     \
    bool (*empty)(void);                      \
    Iterator (*insert_unique)(const void *x); \
    Iterator (*insert_equal)(const void *x);  \
    void (*erase)(Iterator it);               \
    Iterator (*find)(const void *x);          \
    size_t (*count)(const void *);            \
    void (*clear)(void);                      \
    void (*swap)(RB_tree t);                  \
}

typedef struct _RB_tree *RB_tree;
struct _RB_tree {
    METAOBJECT_HEAD(RB_TREE_FUNC);
};

const void *_RB_tree(void);
#define RB_TREE RB_tree: _RB_tree()
#endif //TINY_CTL_TCTL_RB_TREE_H
