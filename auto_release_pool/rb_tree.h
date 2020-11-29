//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_RB_TREE_H
#define TINY_CTL_RB_TREE_H
#include "auto_release_pool.h"
enum rb_tree_color {rb_tree_red, rb_tree_black};

struct rb_tree_node {
    struct rb_tree_node *parent;
    struct rb_tree_node *left;
    struct rb_tree_node *right;
    enum rb_tree_color color;
    ARP_ResId_t id;
    void *p_node;
};

typedef struct {
    struct rb_tree_node header;
} rb_tree;

struct rb_tree_node *insert_unique(rb_tree *tree, ARP_ResId_t id);
struct rb_tree_node *find(rb_tree *tree, ARP_ResId_t id);

void init_rb_tree(rb_tree *p_tree);
void destory_rb_tree(rb_tree *p_tree);
#endif //TINY_CTL_RB_TREE_H
