//
// Created by xjs on 2020/9/1.
//

#include "tctl_iterator.h"
#include "tctl_allocator.h"
#include <memory.h>

static void *at(int x)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    return p_it->__inner.iterator_func_p->private_iter_func->iter_at(p_it, x);
}
static void *increment(void)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    void *ptr = p_it->val;
    p_it->__inner.iterator_func_p->private_iter_func->iter_increment(p_it);
    return ptr;
}

static void *decrement(void)
{

    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    void *ptr = p_it->val;
    p_it->__inner.iterator_func_p->private_iter_func->iter_decrement(p_it);
    return ptr;
}

static void *front_increment(void)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    p_it->__inner.iterator_func_p->private_iter_func->iter_increment(p_it);
    return p_it->val;
}

static void *front_decrement(void)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    p_it->__inner.iterator_func_p->private_iter_func->iter_decrement(p_it);
    return p_it->val;
}

static void add(int x)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    p_it->__inner.iterator_func_p->private_iter_func->iter_add(p_it, x);
}

static void sub(int x)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    p_it->__inner.iterator_func_p->private_iter_func->iter_sub(p_it, x);
}

static long long diff(const __iterator *iter)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    return p_it->__inner.iterator_func_p->private_iter_func->iter_diff(p_it, iter);
}

static bool equal(const __iterator *iter)
{
    __iterator *p_it = pop_this();
    push_this(p_it->__inner.obj_this);
    return p_it->__inner.iterator_func_p->private_iter_func->iter_equal(p_it, iter);
}

void copy(const __iterator *iter)
{
    __iterator *p_it = pop_this();
    if (p_it->__inner.obj_iter_size != iter->__inner.obj_iter_size)
        return;
    memcpy(p_it, iter, p_it->__inner.obj_iter_size);
}

const __public_iterator_func def_pub_iter_func = {
        .at = at,
        .increment = increment,
        .decrement = decrement,
        .front_increment = front_increment,
        .front_decrement = front_decrement,
        .add = add,
        .sub = sub,
        .diff = diff,
        .copy = copy,
        .equal = equal
};

struct __inner_iterator __creat_iter(size_t obj_iter_size, void *obj_this, size_t memb_size, const iterator_func *iter_func)
{
    struct __inner_iterator iter = {iter_func, obj_this, obj_iter_size, memb_size};
    return iter;
}

__iterator *__constructor_iter(__iterator *iter)
{
    size_t iter_size = sizeof(struct __inner_iterator) + iter->__inner.obj_iter_size;
    __iterator *res = allocate(iter_size);
    memcpy(res, iter, iter_size);
    return res;
}

void __destructor_iter(void *p)
{
    __iterator *iter = *(void **) p;
    if (iter)
        deallocate(iter, iter->__inner.obj_iter_size);
}