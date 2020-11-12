//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_TCTL_RB_TREE_H
#define TINY_CTL_TCTL_RB_TREE_H

#include "tctl_def.h"
#include "tctl_iterator.h"

enum __rb_tree_color {__rb_tree_red, __rb_tree_black};

struct __rb_tree_node {
    struct __rb_tree_node *parent;
    struct __rb_tree_node *left;
    struct __rb_tree_node *right;
    enum __rb_tree_color color;
    byte data[0];
};

typedef struct {
    void *val;
    struct __rb_tree_node *node;
} __rb_tree_iter;

typedef struct {
    size_t memb_size;
    size_t size;
    Compare cmp;
    struct __rb_tree_node *header;
    struct {
        struct __inner_iterator start_iter;
        __rb_tree_iter start_ptr;
    };
    struct {
        struct __inner_iterator finish_iter;
        __rb_tree_iter finish_ptr;
    };
    struct {
        struct __inner_iterator change_iter;
        __rb_tree_iter change_ptr;
    };
} __private_rb_tree;

typedef struct {
    const IterType (*begin)(void);
    const IterType (*end)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    IterType (*insert_unique)(void*);
    IterType (*insert_equal)(void*);
    void (*erase)(IterType);
    void (*clear)(void);
    const IterType (*find)(void*);
    size_t (*count)(void*);
    byte __obj_private[sizeof(__private_rb_tree)];
} rb_tree;

void init_rb_tree(rb_tree *p_tree, size_t memb_size, Compare cmp);
void destory_rb_tree(rb_tree *p_tree);
rb_tree creat_rb_tree(size_t memb_size, Compare cmp);
#endif //TINY_CTL_TCTL_RB_TREE_H
