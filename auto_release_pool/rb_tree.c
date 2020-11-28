//
// Created by xjs on 2020/10/9.
//

#include "rb_tree.h"
#include <memory.h>
#include <stdbool.h>
//private:
enum {LEFT = 1, RIGHT = 2, EQUAL = 4, NONE = 8, EXPIRE = 16};
static char cmp(ARP_ResId_t x, ARP_ResId_t y)
{
    if (x.major < y.major)
        return LEFT;
    if (x.major > y.major)
        return RIGHT;
    if (x.minor != y.minor)
        return EXPIRE;
    return EQUAL;
}
static struct __rb_tree_node *__creat_rb_node(void)
{
    struct __rb_tree_node *node = malloc(sizeof(struct __rb_tree_node));
    node->parent = node->left = node->right = NULL;
    node->color = __rb_tree_red;
    node->id = (ARP_ResId_t){0, 0};
    node->p_node = NULL;
    return node;
}

static void __rb_free_node(struct __rb_tree_node *node)
{
    free(node);
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

static void __balance_tree_insert(struct __rb_tree_node *header, struct __rb_tree_node *cur)
{
    if (header->parent == cur) {//该节点为根节点
        cur->color = __rb_tree_black;
    } else if(cur->parent->color == __rb_tree_red) {//父节点是红色
        struct __rb_tree_node *uncle_node = __get_uncle_node(cur);
        if (uncle_node && uncle_node->color == __rb_tree_red) {//叔叔节点是红色
            uncle_node->color = __rb_tree_black;
            cur->parent->color = __rb_tree_black;
            cur->parent->parent->color = __rb_tree_red;
            __balance_tree_insert(header, cur->parent->parent);
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

static struct __rb_tree_node *__insert(struct __rb_tree_node *node, struct __rb_tree_node **access_node, struct __rb_tree_node *header)
{
    struct __rb_tree_node *new_node = __creat_rb_node();
    new_node->parent = node;
    *access_node = new_node;
    __balance_tree_insert(header, new_node);
    return new_node;
}

static bool __find(struct __rb_tree_node *header, ARP_ResId_t x, struct __rb_tree_node **parent)
{
    struct __rb_tree_node **next = &header->parent;
    *parent = header;
    if (header->parent == header)//树里没有节点
        return NONE;
    char flag;
    while (*next && (flag = cmp(x, (*next)->id)) != EQUAL)
    {
        switch (flag)
        {
            case LEFT:
                *parent = *next;
                next = &(*next)->left;
                break;
            case RIGHT:
                *parent = *next;
                next = &(*next)->left;
                break;
            case EXPIRE:
                flag = __get_left_right_node(*next) ? RIGHT : LEFT;
                return EXPIRE | flag;
        }
    }
    if (!*next)
        return NONE | flag;
    return __get_left_right_node(*next) ? RIGHT : LEFT;
}

struct __rb_tree_node *insert_unique(rb_tree tree, ARP_ResId_t id)
{
    struct __rb_tree_node *new_node;
    struct __rb_tree_node *parent;
    char flag = __find(&tree.header, id, &parent);
    if (flag == LEFT || flag == RIGHT) {//有找到相应node
        return flag & RIGHT ? parent->right : parent->left;
    } else if (flag == NONE) {//没有根节点
        new_node = __insert(&tree.header, &tree.header.parent, &tree.header);
        //tree.header.left = tree.header.right = new_node;
    } else if(flag & EXPIRE) {//找到的node过期
        new_node = flag & RIGHT ? parent->right : parent->left;
    } else {//没找到node
        struct __rb_tree_node **node = flag & RIGHT ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = __insert(parent, node, &tree.header);
    }
    new_node->id = id;
    return new_node;
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
