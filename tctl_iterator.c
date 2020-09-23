//
// Created by xjs on 2020/9/1.
//

#include "tctl_iterator.h"
#include "tctl_allocator.h"
#include <memory.h>

static void *at(int x)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    return p_private_func->obj_iter_func->iter_at(p_it, x);
}
static void *increment(void)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    void *ptr = p_it->val;
    p_private_func->obj_iter_func->iter_increment(p_it);
    return ptr;
}

static void *decrement(void)
{

    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    void *ptr = p_it->val;
    p_private_func->obj_iter_func->iter_decrement(p_it);
    return ptr;
}

static void *front_increment(void)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    p_private_func->obj_iter_func->iter_increment(p_it);
    return p_it->val;
}

static void *front_decrement(void)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    p_private_func->obj_iter_func->iter_decrement(p_it);
    return p_it->val;
}

static void add(int x)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    p_private_func->obj_iter_func->iter_add(p_it, x);
}

static void sub(int x)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    p_private_func->obj_iter_func->iter_sub(p_it, x);
}

static long long diff(__iterator * iter)
{
    __iterator *p_it = pop_this();
    __private_iterator_func *p_private_func = (__private_iterator_func*)p_it->iterator_func_p->__obj_private;
    return p_private_func->obj_iter_func->iter_diff(p_it, iter);
}

void copy(__iterator *iter)
{
    __iterator *p_it = pop_this();
    if (p_it->obj_iter_size != iter->obj_iter_size)
        return;
    memcpy(p_it, iter, p_it->obj_iter_size);
}

static iterator_func def_obj_func = {.at = at, .increment = increment, .decrement = decrement, .front_increment = front_increment, .front_decrement = front_decrement, .add = add, .sub = sub, .diff = diff, .copy = copy};

__iterator __creat_iter(size_t obj_iter_size, size_t memb_size, iterator_func const *iterator_func_p)
{
    __iterator iter = {NULL, iterator_func_p, obj_iter_size, memb_size};
    return iter;
}

void __init_iter_func(iterator_func *iter_f, __iterator_obj_func *func)
{
    *iter_f = def_obj_func;
    __private_iterator_func *p_private = (__private_iterator_func*)iter_f->__obj_private;
    p_private->obj_iter_func = func;
}

__iterator *__constructor_iter(__iterator *iter)
{
    __iterator *res = allocate(iter->obj_iter_size);
    memcpy(res, iter, iter->obj_iter_size);
    return res;
}

void __destructor_iter(void *p)
{
    __iterator *iter = *(void **) p;
    deallocate(iter, iter->obj_iter_size);
}