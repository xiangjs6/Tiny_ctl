//
// Created by xjs on 2020/9/1.
//

#include "iterator.h"
#include "tctl_object.h"
#include "allocator.h"
#include "tctl_common.h"

static void *increment(void)
{
    iterator *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = ((__iterator_obj_func*)PRIVATE_THIS(ptr))->iter_increment(p_it->ptr);
    return ptr;
}

static void *decrement(void)
{
    iterator *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = ((__iterator_obj_func*)PRIVATE_THIS(ptr))->iter_decrement(p_it->ptr);
    return ptr;
}

static void *front_increment(void)
{
    iterator *p_it = *pthis();
    p_it->ptr = ((__iterator_obj_func*)PRIVATE_THIS(p_it->ptr))->iter_increment(p_it->ptr);
    return p_it->ptr;
}

static void *front_decrement(void)
{
    iterator *p_it = *pthis();
    p_it->ptr = ((__iterator_obj_func*)PRIVATE_THIS(p_it->ptr))->iter_decrement(p_it->ptr);
    return p_it->ptr;
}

static void *add(int x)
{
    iterator *p_it = *pthis();
    return ((__iterator_obj_func*)PRIVATE_THIS(p_it->ptr))->iter_add(p_it->ptr, x);
}

static void *sub(int x)
{
    iterator *p_it = *pthis();
    return ((__iterator_obj_func*)PRIVATE_THIS(p_it->ptr))->iter_sub(p_it->ptr, x);
}

static typeof(*((iterator*)(0))->Public_memb) def_obj_func = {increment, decrement, front_increment, front_decrement, add, sub};

iterator init_iter(void *obj_ptr, void *p)
{
    iterator iter;
    iter.Public_memb = &def_obj_func;
    iter.obj_this = obj_ptr;
    iter.ptr = p;
    return iter;
}

iterator *__constructor_iter(iterator iter)
{
    iterator *res = allocate(sizeof(iterator));
    *res = iter;
    return res;
}

void __destructor_iter(void *p)
{
    iterator *iter = container_of(*(void**)p, iterator, ptr);
    deallocate(iter, sizeof(iterator));
}

void destory_iter(void *iter)
{
    deallocate(iter, sizeof(iterator));
}