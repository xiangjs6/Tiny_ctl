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

static void __balance_tree_erase(__private_rb_tree *p_private, struct __rb_tree_node *cur)
{
    struct __rb_tree_node *parent = cur->parent;
    struct __rb_tree_node *brother_node = __get_left_right_node(cur) ? parent->left : parent->right;
    if (cur->parent->parent == cur) {//删除节点为根 2
        p_private->header->left = p_private->header->right = p_private->header->parent = p_private->header;
        return;
    }
    else if (cur->color == __rb_tree_red) {//删除节点是红色 1
        return;
    } else {//删除节点是黑色
        if (cur->right && cur->right->color == __rb_tree_red) {//删除节点右节点是红色 3
            cur->right->color = __rb_tree_black;
            return;
        } else if (brother_node->color == __rb_tree_black) {//删除节点的兄弟节点是黑色
            if ((!brother_node->left || brother_node->left->color == __rb_tree_black) &&
                (!brother_node->right || brother_node->right->color == __rb_tree_black)) {//兄弟节点的子节点是黑色
                if (parent->color == __rb_tree_black) {//父亲节点为黑色 8
                    brother_node->color = __rb_tree_red;
                    __balance_tree_erase(p_private, parent);
                } else {//父亲节点为红色 4
                    brother_node->color = __rb_tree_red;
                    parent->color = __rb_tree_black;
                }
            } else if (__get_left_right_node(brother_node)) {//兄弟节点在父节点右侧
                if (brother_node->right && brother_node->right->color == __rb_tree_red) {//兄弟节点的右节点是红色 5
                    brother_node->color = parent->color;
                    parent->color = __rb_tree_black;
                    brother_node->right->color = __rb_tree_black;
                    __turn_left_node(parent);
                } else if (brother_node->left && brother_node->left->color == __rb_tree_red) {//兄弟节点的左节点是红色 6
                    __turn_right_node(brother_node);
                    brother_node = brother_node->parent;
                    brother_node->color = __rb_tree_black;
                    brother_node->right->color = __rb_tree_red;
                    //回到'5'的状态
                    brother_node->color = parent->color;
                    parent->color = __rb_tree_black;
                    brother_node->right->color = __rb_tree_black;
                    __turn_left_node(parent);
                }
            } else {//兄弟节点在父节点左侧 7
                if (brother_node->left && brother_node->left->color == __rb_tree_red) {//兄弟节点的左节点是红色
                    brother_node->color = parent->color;
                    parent->color = __rb_tree_black;
                    brother_node->left->color = __rb_tree_black;
                    __turn_right_node(parent);
                } else if (brother_node->right && brother_node->right->color == __rb_tree_red) {//兄弟节点的右节点是红色
                    __turn_left_node(brother_node);
                    brother_node = brother_node->parent;
                    brother_node->color = __rb_tree_black;
                    brother_node->left->color = __rb_tree_red;
                    //回到'兄弟节点左节点是红色'的状态
                    brother_node->color = parent->color;
                    parent->color = __rb_tree_black;
                    brother_node->left->color = __rb_tree_black;
                    __turn_left_node(parent);
                }
            }
        } else {//兄弟节点是红色
            if (!__get_left_right_node(cur)) {//删除节点为父亲节点左边 9
                brother_node->color = parent->color;
                parent->color = __rb_tree_red;
                __turn_left_node(parent);
                __balance_tree_erase(p_private, cur);
            } else {//删除节点为父亲节点右边 10
                brother_node->color = parent->color;
                parent->color = __rb_tree_red;
                __turn_right_node(parent);
                __balance_tree_erase(p_private, cur);
            }
        }
    }
}

static struct __rb_tree_node *__insert(struct __rb_tree_node *node, struct __rb_tree_node **access_node, __private_rb_tree *p_private)
{
    struct __rb_tree_node *new_node = __creat_rb_node(p_private->memb_size);
    new_node->parent = node;
    *access_node = new_node;
    __balance_tree_insert(p_private, new_node);
    return new_node;
}

static void __erase(struct __rb_tree_node *node, __private_rb_tree *p_private)
{
    struct __rb_tree_node *rep_node = node->right;
    if (!rep_node)
        rep_node = node;
    else
        while (rep_node->left)
            rep_node = rep_node->left;
    if (node != rep_node)
        memcpy(node->data, rep_node->data, p_private->memb_size);
    __balance_tree_erase(p_private, rep_node);
    //删除
    struct __rb_tree_node *parent = rep_node->parent;
    struct __rb_tree_node *next_node = rep_node == node ? rep_node->left : rep_node->right;
    if (p_private->header->left == rep_node) {
        p_private->header->left = rep_node->parent;
    } else if (p_private->header->right == rep_node) {
        if (next_node)
            parent->right->right = maximum(next_node);
        else
            p_private->header->right = parent;
    }
    if (rep_node == rep_node->parent->parent) {
        next_node = next_node ? next_node : p_private->header;
        if (next_node == p_private->header)
            parent->left = parent->right = parent->parent = next_node;
        else
            parent->right = parent->parent = next_node;
    } else if (!__get_left_right_node(rep_node)) {
        parent->left = next_node;
        if (next_node)
            next_node->parent = parent;
    } else {
        parent->right = next_node;
        if (next_node)
            next_node->parent = parent;
    }
    __rb_free_node(rep_node, p_private->memb_size);
}

static bool __find(struct __rb_tree_node *header, void const *x, struct __rb_tree_node **parent, size_t *is_unique, Compare cmp)
{
    struct __rb_tree_node **next = &header->parent;
    *parent = header;
    if (header->parent == header)//树里没有节点
        return false;
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

//public:
static __iterator const *begin(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->start_ptr.node = p_private->header->left;
    p_private->start_ptr.val = p_private->header->left->data;
//    p_private->start_ptr.move_from = 0;
    return (__iterator*)&p_private->start_iter;
}

static __iterator const *end(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->finish_ptr.node = p_private->header;
    p_private->finish_ptr.val = p_private->header->data;
//    p_private->finish_ptr.move_from = 0;
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

static __iterator *insert_unique(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *new_node;
    struct __rb_tree_node *parent;
    size_t is_unique = true;
    bool left_right = __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    if (is_unique) {
        p_private->change_ptr.node = NULL;
        p_private->change_ptr.val = NULL;
        return (__iterator*)&p_private->change_iter;
    }
    if (parent == p_private->header) {//没有根节点
        new_node = __insert(p_private->header, &p_private->header->parent, p_private);
        p_private->header->left = p_private->header->right = new_node;
    } else {
        struct __rb_tree_node **node = left_right ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = __insert(parent, node, p_private);
    }
    memcpy(new_node->data, x, p_private->memb_size);
    p_private->change_ptr.node = new_node;
    p_private->change_ptr.val = new_node->data;

    //重新分配header的指向,left总是指向最小值,right总是指向最大值
    if (left_right)
        p_private->header->right = maximum(p_private->header->right);
    else
        p_private->header->left = minimum(p_private->header->left);
    return (__iterator*)&p_private->change_iter;
}

static __iterator *insert_equal(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *new_node;
    struct __rb_tree_node *parent;
    size_t is_unique = false;
    bool left_right = __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    if (parent->parent == parent) {//没有根节点
        new_node = __insert(p_private->header, &p_private->header->parent, p_private);
        p_private->header->left = p_private->header->right = new_node;
    } else {
        struct __rb_tree_node **node = left_right ? &parent->right : &parent->left;//判断插入节点是父节点的左节点还是右节点
        new_node = __insert(parent, node, p_private);
    }
    memcpy(new_node->data, x, p_private->memb_size);
    p_private->change_ptr.node = new_node;
    p_private->change_ptr.val = new_node->data;

    //重新分配header的指向,left总是指向最小值,right总是指向最大值
    if (left_right)
        p_private->header->right = maximum(p_private->header->right);
    else
        p_private->header->left = minimum(p_private->header->left);
    return (__iterator*)&p_private->change_iter;
}

static void erase(__iterator *it)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    __rb_tree_iter *_rb_iter = (__rb_tree_iter*)it->__inner.__address;
    struct __rb_tree_node *erase_node = _rb_iter->node;
    __erase(erase_node, p_private);
}

static void clear(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *node = p_private->start_ptr.node, *next;
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
    p_private->size = 0;
    p_private->header->parent = p_private->header->left = p_private->header->right = p_private->header;
}

static __iterator *find(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    size_t is_unique = true;
    struct __rb_tree_node *parent;
    bool left_right = __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    if (!is_unique)
        return NULL;
    else {
        __iterator *iter = NEW_ITER(THIS(this).begin());
        __rb_tree_iter *rb_iter = (__rb_tree_iter*)iter->__inner.__address;
        if (parent == p_private->header)
            rb_iter->node = parent->parent;
        else
            rb_iter->node = !left_right ? parent->left : parent->right;
        rb_iter->val = rb_iter->node->data;
        return iter;
    }
}

static size_t count(void *x)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    struct __rb_tree_node *parent;
    size_t is_unique = false;
    __find(p_private->header, x, &parent, &is_unique, p_private->cmp);
    return is_unique;
}

static void iter_increment(__iterator *iter)
{
    struct __rb_tree_node *node = ((__rb_tree_iter*)iter->__inner.__address)->node;
    if(node->right != NULL)
    {
        node = node->right;
        while(node->left != NULL)
            node=node->left;
    } else {//往父节点上寻找
        struct __rb_tree_node *parents=node->parent;
        while(node == parents->right)  //为父节点右孩子时，继续往上找
        {
            node = parents;
            parents = parents->parent;
        }
        if (node->right != parents)
            node = parents;
/*
        if(parents != NULL)
            node = parents;
        else                       //找到根基点还没找到，则++应该指向空节点
            node = NULL;
*/
    }
    ((__rb_tree_iter*)iter->__inner.__address)->node = node;
    iter->val = node->data;
}

static void iter_decrement(__iterator *iter)
{
    struct __rb_tree_node *node = ((__rb_tree_iter *) iter->__inner.__address)->node;
    if (node->color == __rb_tree_red && node->parent->parent == node)
        node = node->right;
    else if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL)
            node = node->right;
    } else {
        struct __rb_tree_node *parents = node->parent;
        while (parents->left == node)
        {
            node = parents;
            parents = parents->parent;
        }
        node = parents;
    }
    ((__rb_tree_iter *) iter->__inner.__address)->node = node;
    iter->val = node->data;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    __rb_tree_iter *__it1 = (__rb_tree_iter*)it1->__inner.__address;
    __rb_tree_iter *__it2 = (__rb_tree_iter*)it2->__inner.__address;
    return __it1->node != __it2->node;
}

static __iterator_obj_func  __def_rb_tree_iter = {
        NULL,
        iter_increment,
        iter_decrement,
        NULL,
        NULL,
        NULL,
        iter_equal
};
static const iterator_func __def_rb_tree_iter_func = INIT_ITER_FUNC(&__def_rb_tree_iter);

static const rb_tree _def_rb_tree = {
    begin,
    end,
    empty,
    size,
    insert_unique,
    insert_equal,
    erase,
    clear,
    find,
    count
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
    p_private->change_iter = __creat_iter(sizeof(__rb_tree_iter), p_tree, memb_size, &__def_rb_tree_iter_func);
    struct __rb_tree_node *node= __creat_rb_node(0);
    node->parent = node->left = node->right = node;
    p_private->header = node;
}

void destory_rb_tree(rb_tree *p_tree)
{
    __private_rb_tree *p_private = (__private_rb_tree*)p_tree->__obj_private;
    THIS(p_tree).clear();
    deallocate(p_private->header, 0);
}

rb_tree creat_rb_tree(size_t memb_size, Compare cmp)
{
    rb_tree tree;
    init_rb_tree(&tree, memb_size, cmp);
    return tree;
}