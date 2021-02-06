//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_TCTL_RB_TREE_H
#define TINY_CTL_TCTL_RB_TREE_H

#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
#include "tctl_class.h"

#define RB_TREE_FUNC                     \
struct {                                 \
    CLASS_FUNC;                          \
    Iterator (*begin)(void);             \
    Iterator (*end)(void);               \
    size_t (*size)(void);                \
    bool (*empty)(void);                 \
    Iterator (*insert_unique)(FormWO_t); \
    Iterator (*insert_equal)(FormWO_t);  \
    void (*erase)(Iterator);             \
    Iterator (*find)(FormWO_t);          \
    size_t (*count)(FormWO_t);           \
    void (*clear)(void);                 \
    void (*swap)(RB_tree t);             \
}

typedef struct _RB_tree *RB_tree;
struct _RB_tree {
    METAOBJECT_HEAD(RB_TREE_FUNC);
};

void initRB_tree(void) __attribute__((constructor));
Form_t _RB_tree(void);
#define RB_TREE RB_tree: _RB_tree()
#endif //TINY_CTL_TCTL_RB_TREE_H
