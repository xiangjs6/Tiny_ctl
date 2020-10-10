//
// Created by xjs on 2020/10/9.
//

#include "tctl_rb_tree.h"
#include "tctl_allocator.h"
//private:
struct __rb_tree_node *creat_rb_node(size_t memb_size)
{
    struct __rb_tree_node *node = allocate(sizeof(struct __rb_tree_node) + memb_size);
    node->parent = node->left = node->right = NULL;
    node->color = __rb_tree_red;
    return node;
}
//public:
__iterator const *begin(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->start_ptr.node = p_private->header->left;
    p_private->start_ptr.val = p_private->header->left->data;
    return (__iterator*)&p_private->start_iter;
}

__iterator const *end(void)
{
    rb_tree *this = pop_this();
    __private_rb_tree *p_private = (__private_rb_tree*)this->__obj_private;
    p_private->finish_ptr.node = p_private->header;
    p_private->finish_ptr.val = p_private->header->data;
    return (__iterator*)&p_private->finish_iter;
}

bool empty(void)
{
    rb_tree *this = pop_this();
    return THIS(this).size();
}

size_t size(void)
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
    __private_rb_tree *p_private = (__private_rb_tree*)p_tree->__obj_private;
    THIS(p_tree).clear();
    deallocate(p_tree, 0);
}

rb_tree creat_rb_tree(size_t memb_size, Compare cmp)
{
    rb_tree tree;
    init_rb_tree(&tree, memb_size, cmp);
    return tree;
}