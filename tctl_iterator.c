//
// Created by xjs on 2020/9/1.
//

#include "tctl_iterator.h"
#include "tctl_allocator.h"
#include "tctl_common.h"

static void *increment(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    void *ptr = p_it->ptr;
    push_this(private_it->obj_this);
    p_it->ptr = private_it->obj_iter_func->iter_increment(p_it->ptr);
    return ptr;
}

static void *decrement(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    void *ptr = p_it->ptr;
    push_this(private_it->obj_this);
    p_it->ptr = private_it->obj_iter_func->iter_decrement(p_it->ptr);
    return ptr;
}

static void *front_increment(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(private_it->obj_this);
    p_it->ptr = private_it->obj_iter_func->iter_increment(p_it->ptr);
    return p_it->ptr;
}

static void *front_decrement(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(private_it->obj_this);
    p_it->ptr = private_it->obj_iter_func->iter_decrement(p_it->ptr);
    return p_it->ptr;
}

static void *add(int x)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(private_it->obj_this);
    return private_it->obj_iter_func->iter_add(p_it->ptr, x);
}

static void *sub(int x)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(private_it->obj_this);
    return private_it->obj_iter_func->iter_sub(p_it->ptr, x);
}

static iterator def_obj_func = {.increment = increment, .decrement = decrement, .front_increment = front_increment, .front_decrement = front_decrement, .add = add, .sub = sub};

iterator init_iter(void *obj_ptr, void *p, __iterator_obj_func *func)
{
    iterator iter = def_obj_func;
    iter.ptr = p;
    __private_iterator *p_private = (__private_iterator*)iter.__obj_private;
    p_private->obj_this = obj_ptr;
    p_private->obj_iter_func = func;
    return iter;
}

iterator *__constructor_iter(iterator *iter)
{
    iterator *res = allocate(sizeof(iterator));
    *res = *iter;
    return res;
}

void __destructor_iter(void *p)
{
    iterator *iter = container_of(*(void**)p, iterator, ptr);
    deallocate(iter, sizeof(iterator));
}
