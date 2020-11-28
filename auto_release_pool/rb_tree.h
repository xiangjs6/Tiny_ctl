//
// Created by xjs on 2020/10/9.
//

#ifndef TINY_CTL_TCTL_RB_TREE_H
#define TINY_CTL_TCTL_RB_TREE_H

enum __rb_tree_color {__rb_tree_red, __rb_tree_black};

struct __rb_tree_node {
    struct __rb_tree_node *parent;
    struct __rb_tree_node *left;
    struct __rb_tree_node *right;
    enum __rb_tree_color color;
    ARP_ResId id;
    struct mem_node *p_node;
};

typedef struct {
    struct __rb_tree_node *header;
} __private_rb_tree;

int insert_unique();
struct __rb_tree_node *find(ARP_ResId);

void init_rb_tree(rb_tree *p_tree, size_t memb_size, Compare cmp);
void destory_rb_tree(rb_tree *p_tree);
rb_tree creat_rb_tree(size_t memb_size, Compare cmp);
#endif //TINY_CTL_TCTL_RB_TREE_H
