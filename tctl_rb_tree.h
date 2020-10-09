//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_TCTL_RB_TREE_H
#define TINY_CTL_TCTL_RB_TREE_H

#include "tctl_def.h"
#include "tctl_iterator.h"

typedef struct {
    void *val;
} __rb_tree_iter;
struct __rb_tree_node {
    struct __rb_tree_node *parent;
    struct __rb_tree_node *left;
    struct __rb_tree_node *right;
    byte data[0];
};

typedef struct {
    size_t memb_size;
    size_t size;
    Compare cmp;
    struct __rb_tree_node header;
    struct {
        struct __inner_iterator start_iter;
        __rb_tree_iter start_ptr;
    };
    struct {
        struct __inner_iterator end_iter;
        __rb_tree_iter end_ptr;
    };
} __private_rb_tree;

typedef struct {
    __iterator const *(*begin)(void);
    __iterator const *(*end)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    __iterator *(*insert_unique)(void*);
    __iterator *(*insert_equal)(void*);
    void (*erase)(__iterator*);
    void (*clear)(void);
    __iterator *(*find)(void*);
    size_t (*count)(void*);
    byte __obj_private[sizeof(__private_rb_tree)];
} rb_tree;

#endif //TINY_CTL_TCTL_RB_TREE_H
