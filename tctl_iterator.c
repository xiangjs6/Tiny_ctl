//
// Created by xjs on 2020/9/1.
//

#include "tctl_iterator.h"
#include "tctl_allocator.h"
#include "tctl_common.h"
#include <memory.h>

static void *at(int x)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    return private_it->obj_iter_func->iter_at(private_it->obj_iter, x);
}
static void *increment(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    void *ptr = *(void**)private_it->obj_iter;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_increment(private_it->obj_iter);
    return ptr;
}

static void *decrement(void)
{

    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    void *ptr = *(void**)private_it->obj_iter;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_decrement(private_it->obj_iter);
    return ptr;
}

static void *front_increment(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_increment(private_it->obj_iter);
    return p_it->iter_ptr;
}

static void *front_decrement(void)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_decrement(private_it->obj_iter);
    return p_it->iter_ptr;
}

static void add(int x)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_add(private_it->obj_iter, x);
}

static void sub(int x)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(p_it->obj_this);
    private_it->obj_iter_func->iter_sub(private_it->obj_iter, x);
}

static long long diff(obj_iter iter)
{
    iterator *p_it = pop_this();
    __private_iterator *private_it = (__private_iterator*)p_it->__obj_private;
    push_this(p_it->obj_this);
    return private_it->obj_iter_func->iter_diff(p_it->iter_ptr, iter);
}

static iterator def_obj_func = {.at = at, .increment = increment, .decrement = decrement, .front_increment = front_increment, .front_decrement = front_decrement, .add = add, .sub = sub, .diff = diff};

void __init_iter(iterator *iter, void *obj_ptr, size_t obj_iter_size, size_t memb_size, __iterator_obj_func *func)
{
    memcpy(iter, &def_obj_func, sizeof(iterator));
    iter->obj_this = obj_ptr;
    *(size_t*)&iter->memb_size = memb_size;
    //*iter = def_obj_func;
    __private_iterator *p_private = (__private_iterator*)iter->__obj_private;
    p_private->obj_iter_func = func;
    p_private->obj_iter_size = obj_iter_size;

    //void *obj_iter = p_private->obj_iter;
    //memcpy((void*)&iter->ptr, &obj_iter, sizeof(void*));
    *(void**)&iter->iter_ptr = p_private->obj_iter;
}

iterator *__constructor_iter(iterator *iter)
{
    __private_iterator *p_private = (__private_iterator *)iter->__obj_private;
    size_t iter_size = sizeof(iterator) + p_private->obj_iter_size;
    iterator *res = allocate(iter_size);
    memcpy(res, iter, iter_size);
    //void *obj_iter = p_private->obj_iter;
    //memcpy((void*)&iter->ptr, &obj_iter, sizeof(void*));
    *(void**)&res->iter_ptr = (void*)res + sizeof(iterator);
    return res;
}

void __destructor_iter(void *p)
{
    iterator *iter = container_of(container_of(*(void**)p, __private_iterator, obj_iter), iterator, __obj_private);
    __private_iterator *p_private = (__private_iterator*)iter->__obj_private;
    deallocate(iter, sizeof(iterator) + p_private->obj_iter_size);
}
