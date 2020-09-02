//
// Created by xjs on 2020/9/1.
//

#include "iterator.h"
#include "tctl_object.h"

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

typeof(*((iterator*)(0))->Public_memb) def_obj_func = {increment, decrement, front_increment, front_decrement, add, sub};

iterator creat_iter(void *p)
{
    iterator it;
    it.Public_memb = &def_obj_func;
    it.ptr = p;
    it.obj_this = pthis();
    return it;
}