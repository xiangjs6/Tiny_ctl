//
// Created by xjs on 2020/10/9.
//

#include "tctl_rb_tree.h"
#include "tctl_allocator.h"
#include <memory.h>
//private:
static struct __rb_tree_node *__creat_rb_node(size_t memb_size)
{
    struct __rb_tree_node *node = allocate(sizeof(struct __rb_tree_node) + memb_size);
    node->parent = node->left = node->right = NULL;
    node->color = __rb_tree_red;
    return node;
}

static void __rb_free_node(struct __rb_tree_node *node, size_t memb_size)
{
    deallocate(node, sizeof(struct __rb_tree_node) + memb_size);
}

static struct __rb_tree_node *minimum(struct __rb_tree_node *root)
{
    struct __rb_tree_node *node = root;
    while (node->left)
        node = node->left;
    return node;
}

static struct __rb_tree_node *maximum(struct __rb_tree_node *root)
{
    struct __rb_tree_node *node = root;
    while (node->right)
        node = node->right;
    return node;
}

static bool __get_left_right_node(struct __rb_tree_node *node)
{
    return node->parent->right == node;
}

static struct __rb_tree_node *__get_uncle_node(struct __rb_tree_node *node)
{
    struct __rb_tree_node *g_parent = node->parent->parent;
    if (__get_left_right_node(node->parent))
        return g_parent->left;
    else
        return g_parent->right;
}

static void __turn_left_node(struct __rb_tree_node *drop_node)
{
    struct __rb_tree_node *parent = drop_node->parent;
    //父亲节点变成爷爷节点
    if (parent->parent == drop_node)//如果爷爷节点为header节点
        parent->parent = drop_node->right;
    else if (__get_left_right_node(drop_node))
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

static void __turn_right_node(struct __rb_tree_node *drop_node)
{
    struct __rb_tree_node *parent = drop_node->parent;
    if (parent->parent == drop_node)
        parent->parent = drop_node->left;
    else if (__get_left_right_node(drop_node))
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

static void __balance_tree_insert(__private_rb_tree *p_private, struct __rb_tree_node *cur)
{
    if (p_private->header->parent == cur) {//该节点为根节点
        cur->color = __rb_tree_black;
    } else if(cur->parent->color == __rb_tree_red) {//父节点是红色
        struct __rb_tree_node *uncle_node = __get_uncle_node(cur);
        if (uncle_node && uncle_node->color == __rb_tree_red) {//叔叔节点是红色
            uncle_node->color = __rb_tree_black;
            cur->parent->color = __rb_tree_black;
            cur->parent->parent->color = __rb_tree_red;
            __balance_tree_insert(p_private, cur->parent->parent);
        } else {//叔叔节点是黑色
            if (!__get_left_right_node(cur->parent)) {//插入节点的父节点在爷爷节点的左侧
                if (__get_left_right_node(cur)) {//插入节点在父节点的右侧
                    __turn_left_node(cur->parent);
                    __turn_right_node(cur->parent);
                } else {//插入节点在父节点的左侧
                    __turn_right_node(cur->parent->parent);
                    cur = cur->parent;//将cur赋值为新转动到父节点的地址
                }
            } else {//插入节点在爷爷节点右侧
                if (!__get_left_right_node(cur)) {//插入节点在父亲节点左侧
                    __turn_right_node(cur->parent);
                    __turn_left_node(cur->parent);
                } else {//插入节点在父亲节点右侧
                    __turn_left_node(cur->parent->parent);
                    cur = cur->parent;//将cur赋值为新转动到父节点的地址
                }
            }
            //调整旋转后的节点颜色
            cur->color = __rb_tree_black;
            cur->left->color = cur->right->color = __rb_tree_red;
        }
    }
}

static struct __rb_tree_node *__insert(struct __rb_tree_node *node, struct __rb_tree_node **access_node, __private_rb_tree *p_private)
{
    struct __rb_tree_node *new_node = __creat_rb_node(p_private->memb_size);
    new_node->parent = node;
    *access_node = new_node;
    __balance_tree_insert(p_private, new_node);
    p_private->nmemb++;
    return new_node;
}

static bool __find(struct __rb_tree_node *header, void const *x, struct __rb_tree_node **parent, size_t *is_unique, Compare cmp)
{
    struct __rb_tree_node **next = &header->parent;
    *parent = header;
    if (header->parent == header) {//树里没有节点
        *is_unique = 0;
        return false;
    }
    byte flag = 1;
    size_t save_is_unique = 0;
    while (*next && ((flag = cmp(x, (*next)->data)) || !*is_unique))
    {
        if (!flag)
            save_is_unique++;
        *parent = *next;
        next = flag < 0 ? &(*next)->left : &(*next)->right;
    }
    if (*next && !flag)
        save_is_unique++;
    *is_unique = save_is_unique;
    return &(*parent)->right == next;
    //return next == &(*parent)->right;
    //return flag > 0;
}

static const IterType insert_unique(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *new_node;
    struct __rb_tree_node *parent;
    size_t is_unique = true;
    bool left_right = __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    if (is_unique) {
        return THIS(this).end();
    }
    if (parent == p_private->header) {//没有根节点
        new_node = __insert(p_private->header, &p_private->header->parent, p_private);
        p_private->header->left = p_private->header->right = new_node;
    } else {
        struct __rb_tree_node **node = left_right ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = __insert(parent, node, p_private);
    }
    memcpy(new_node->data, x, p_private->memb_size);
    struct inner_iter *four_iter = thread_getspecific(p_private->iter_key);
    if (four_iter == NULL)
        four_iter = iter_once_init(p_private);
    if (four_iter->write_iter == NULL || four_iter->write_iter->used_by_out) {
        four_iter->write_iter = allocate(sizeof(struct __inner_iterator) + sizeof(__rb_tree_iter));
        *four_iter->write_iter = __creat_iter(sizeof(__rb_tree_iter), this, p_private->memb_size, &__def_rb_tree_iter_func);
    }
    __rb_tree_iter *out_iter = (__rb_tree_iter*)four_iter->write_iter->__address;
    out_iter->node = new_node;
    out_iter->val = new_node->data;

    //重新分配header的指向,left总是指向最小值,right总是指向最大值
    if (left_right)
        p_private->header->right = maximum(p_private->header->right);
    else
        p_private->header->left = minimum(p_private->header->left);
    return four_iter->write_iter;
}

static void clear(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *node = p_private->header->left, *next;
    while (node == p_private->header)
    {
        node = minimum(node);
        node = maximum(node);
        if (node->left)
            continue;
        next = node->parent;
        __rb_free_node(node, p_private->memb_size);
        node = next;
    }
    p_private->nmemb = 0;
    p_private->header->parent = p_private->header->left = p_private->header->right = p_private->header;
}

static const IterType find(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    size_t is_unique = true;
    struct __rb_tree_node *parent;
    bool left_right = __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    if (!is_unique)
        return THIS(this).end();
    else {
        struct inner_iter *four_iter = thread_getspecific(p_private->iter_key);
        if (four_iter == NULL)
            four_iter = iter_once_init(p_private);
        if (four_iter->read_iter == NULL || four_iter->read_iter->used_by_out) {
            four_iter->read_iter = allocate(sizeof(struct __inner_iterator) + sizeof(__rb_tree_iter));
            *four_iter->read_iter = __creat_iter(sizeof(__rb_tree_iter), this, p_private->memb_size, &__def_rb_tree_iter_func);
        }
        __rb_tree_iter *rb_iter = (__rb_tree_iter*)four_iter->read_iter->__address;
        if (parent == p_private->header)
            rb_iter->node = parent->parent;
        else
            rb_iter->node = !left_right ? parent->left : parent->right;
        rb_iter->val = rb_iter->node->data;
        return four_iter->read_iter;
    }
}

void init_rb_tree(rb_tree *p_tree, size_t memb_size, Compare cmp)
{
    *p_tree = _def_rb_tree;
    __private_rb_tree *p_private = (__private_rb_tree *)p_tree->__obj_private;
    p_private->memb_size = memb_size;
    p_private->nmemb = 0;
    p_private->cmp = cmp;
    struct __rb_tree_node *node= __creat_rb_node(0);
    node->parent = node->left = node->right = node;
    p_private->header = node;
    thread_key_create(&p_private->iter_key, free_inner_iter);
}

void destory_rb_tree(rb_tree *p_tree)
{
    __private_rb_tree *p_private = (__private_rb_tree*)p_tree->__obj_private;
    THIS(p_tree).clear();
    deallocate(p_private->header, 0);
    thread_key_delete(p_private->iter_key);
}

rb_tree creat_rb_tree(size_t memb_size, Compare cmp)
{
    rb_tree tree;
    init_rb_tree(&tree, memb_size, cmp);
    return tree;
}