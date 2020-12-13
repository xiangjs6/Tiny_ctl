//
// Created by xjs on 2020/9/1.
//

#include "tctl_iterator.h"
#include "tctl_allocator.h"
#include <memory.h>

static void *at(int x)
{
    __iterator *p_it = *(__iterator**)pop_this();
    return p_it->__inner.iterator_func_p->private_iter_func->iter_at(p_it, x);
}
static void *increment(void)
{
    __iterator *p_it = *(__iterator**)pop_this();
    void *ptr = p_it->val;
    p_it->__inner.iterator_func_p->private_iter_func->iter_increment(p_it);
    return ptr;
}

static void *decrement(void)
{
    __iterator *p_it = *(__iterator**)pop_this();
    void *ptr = p_it->val;
    p_it->__inner.iterator_func_p->private_iter_func->iter_decrement(p_it);
    return ptr;
}

static void *front_increment(void)
{
    __iterator *p_it = *(__iterator**)pop_this();
    p_it->__inner.iterator_func_p->private_iter_func->iter_increment(p_it);
    return p_it->val;
}

static void *front_decrement(void)
{
    __iterator *p_it = *(__iterator**)pop_this();
    p_it->__inner.iterator_func_p->private_iter_func->iter_decrement(p_it);
    return p_it->val;
}

static void add(int x)
{
    __iterator *p_it = *(__iterator**)pop_this();
    p_it->__inner.iterator_func_p->private_iter_func->iter_add(p_it, x);
}

static void sub(int x)
{
    __iterator *p_it = *(__iterator**)pop_this();
    p_it->__inner.iterator_func_p->private_iter_func->iter_sub(p_it, x);
}

static long long diff(const IterType iter)
{
    __iterator *p_it = *(__iterator**)pop_this();
    return p_it->__inner.iterator_func_p->private_iter_func->iter_diff(p_it, iter);
}

static bool equal(const IterType iter)
{
    __iterator *p_it = *(__iterator**)pop_this();
    return p_it->__inner.iterator_func_p->private_iter_func->iter_equal(p_it, iter);
}

void copy(const IterType iter)
{
    __iterator **pp_it = pop_this();
    __iterator *p_it = *pp_it;
    const __iterator *__iter = iter;
    if (p_it == &def_init_iter) {
        *pp_it = __constructor_iter(iter);
        return;
    }
    else if (p_it->__inner.obj_iter_size != __iter->__inner.obj_iter_size)
        *pp_it = reallocate(p_it, p_it->__inner.obj_iter_size, __iter->__inner.obj_iter_size);
    memcpy(*pp_it, iter, sizeof(struct __inner_iterator) + __iter->__inner.obj_iter_size);
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

const iterator_func def_init_func = INIT_ITER_FUNC(NULL);

const __iterator def_init_iter = {{&def_init_func, NULL, 0, 0}, NULL};

struct __inner_iterator __creat_iter(size_t obj_iter_size, void *obj_this, size_t memb_size, const iterator_func *iter_func)
{
    struct __inner_iterator iter = {iter_func, obj_this, obj_iter_size, memb_size};
    return iter;
}

__iterator *__constructor_iter(__iterator const *iter)
{
    size_t iter_size = sizeof(struct __inner_iterator) + iter->__inner.obj_iter_size;
    __iterator *res = allocate(iter_size);
    memcpy(res, iter, iter_size);
    return res;
}

void __destructor_iter(void const *p)
{
    __iterator *iter = *(__iterator **)p;
    if (iter && iter != &def_init_iter)
        deallocate(iter, iter->__inner.obj_iter_size);
}