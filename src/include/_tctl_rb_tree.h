//
// Created by xjs on 2021/2/2.
//

#ifndef TINY_CTL__TCTL_RB_TREE_H
#define TINY_CTL__TCTL_RB_TREE_H
#include "../../include/tctl_rb_tree.h"
#include "_tctl_class.h"
struct RB_treeSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    Iterator (*insert_unique)(const void *x);
    Iterator (*insert_equal)(const void *x);
    void (*erase)(Iterator);
    Iterator (*find)(const void *x);
    size_t (*count)(const void *x);
    void (*clear)(void);
    void (*swap)(RB_tree t);
};
const void *RB_treeClass(void);
extern const struct RB_treeSelector *_RB_treeS;
#endif //TINY_CTL__TCTL_RB_TREE_H
