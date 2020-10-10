//
// Created by xjs on 2020/10/9.
//

#include "tctl_rb_tree.h"
#include "tctl_allocator.h"
//private:
static struct __rb_tree_node *creat_rb_node(size_t memb_size)
{
    struct __rb_tree_node *node = allocate(sizeof(struct __rb_tree_node) + memb_size);
    node->parent = node->left = node->right = NULL;
    node->color = __rb_tree_red;
    return node;
}

static bool get_left_right_node(struct __rb_tree_node *node)
{
    return node->parent->right == node;
}

static struct __rb_tree_node *get_uncle_node(struct __rb_tree_node *node)
{
    struct __rb_tree_node *g_parent = node->parent->parent;
    if (get_left_right_node(node->parent))
        return g_parent->left;
    else
        return g_parent->right;
}

static void turn_left_node(struct __rb_tree_node *drop_node)
{
    struct __rb_tree_node *parent = drop_node->parent;
    //父亲节点变成爷爷节点
    if (get_left_right_node(drop_node))
        parent->right = drop_node->right;
    else
        parent->left = drop_node->right;
    //指向父亲节点的指针指向新节点
    drop_node->parent = drop_node->right;
    //右节点指针指向新父亲节点的左子树
    drop_node->right = drop_node->parent->left;
    //修改新父节点的父节点
    drop_node->parent->parent = parent;
    //修改新父节点的左子树
    drop_node->parent->left = drop_node;
    //修改新右子树的父节点
    if (drop_node->right)
        drop_node->right->parent = drop_node;
}

static void turn_right_node(struct __rb_tree_node *drop_node)
{
    struct __rb_tree_node *parent = drop_node->parent;
    if (get_left_right_node(drop_node))
        parent->right = drop_node->left;
    else
        parent->left = drop_node->left;
    drop_node->parent = drop_node->left;
    drop_node->left = drop_node->parent->right;
    drop_node->parent->parent = parent;
    drop_node->parent->right = drop_node;
    if (drop_node->left)
        drop_node->left->parent = drop_node;
}

static void balance_tree(__private_rb_tree *p_private, struct __rb_tree_node *cur)
{
    if (p_private->header->parent == cur) {//该节点为跟节点
        cur->color = __rb_tree_black;
    } else if(cur->parent->color == __rb_tree_red) {//父节点是红色
        struct __rb_tree_node *uncle_node = get_uncle_node(cur);
        if (uncle_node->color == __rb_tree_red) {//叔叔节点是红色
            uncle_node->color = __rb_tree_black;
            cur->parent->color = __rb_tree_black;
            cur->parent->parent->color = __rb_tree_red;
            balance_tree(p_private, cur->parent->parent);
        } else {//叔叔节点是黑色
            if (!get_left_right_node(cur->parent)) {//插入节点的父节点在爷爷节点的左侧
                if (get_left_right_node(cur)) {//插入节点在父节点的右侧
                    turn_left_node(cur->parent);
                    turn_right_node(cur->parent);
                } else {//插入节点在父节点的左侧
                    turn_right_node(cur->parent->parent);
                }
            } else {//插入节点在爷爷节点右侧
                if (!get_left_right_node(cur)) {//插入节点在父亲节点左侧
                    turn_right_node(cur->parent);
                    turn_left_node(cur->parent);
                } else {//插入节点在父亲节点右侧
                    turn_left_node(cur->parent->parent);
                }
            }
        }
    }
}

static struct __rb_tree_node *insert(struct __rb_tree_node *node, struct __rb_tree_node **access_node, __private_rb_tree *p_private)
{
    struct __rb_tree_node *new_node = creat_rb_node(p_private->memb_size);
    new_node->parent = node;
    *access_node = new_node;
    balance_tree(p_private, new_node);
    return new_node;
}
//public:
static __iterator const *begin(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->start_ptr.node = p_private->header->left;
    p_private->start_ptr.val = p_private->header->left->data;
    return (__iterator*)&p_private->start_iter;
}

static __iterator const *end(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->finish_ptr.node = p_private->header;
    p_private->finish_ptr.val = p_private->header->data;
    return (__iterator*)&p_private->finish_iter;
}

static bool empty(void)
{
    rb_tree *this = pop_this();
    return THIS(this).size();
}

static size_t size(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    return p_private->size;
}

static __iterator_obj_func  __def_rb_tree_iter = {
        NULL,
        //iter_increment,
        //iter_decrement,
        NULL,
        NULL,
        NULL,
        //iter_equal
};
static const iterator_func __def_rb_tree_iter_func = INIT_ITER_FUNC(&__def_rb_tree_iter);

static const rb_tree _def_rb_tree = {

};
void init_rb_tree(rb_tree *p_tree, size_t memb_size, Compare cmp)
{
    *p_tree = _def_rb_tree;
    __private_rb_tree *p_private = (__private_rb_tree *)p_tree->__obj_private;
    p_private->memb_size = memb_size;
    p_private->size = 0;
    p_private->cmp = cmp;
    p_private->start_iter = __creat_iter(sizeof(__rb_tree_iter), p_tree, memb_size, &__def_rb_tree_iter_func);
    p_private->finish_iter = __creat_iter(sizeof(__rb_tree_iter), p_tree, memb_size, &__def_rb_tree_iter_func);
    struct __rb_tree_node *node= creat_rb_node(0);
    node->left = node->right = node->parent = node;
    p_private->header = node;
}

void destory_rb_tree(rb_tree *p_tree)
{
    //__private_rb_tree *p_private = (__private_rb_tree*)p_tree->__obj_private;
    THIS(p_tree).clear();
    deallocate(p_tree, 0);
}

rb_tree creat_rb_tree(size_t memb_size, Compare cmp)
{
    rb_tree tree;
    init_rb_tree(&tree, memb_size, cmp);
    return tree;
}