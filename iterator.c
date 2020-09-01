//
// Created by xjs on 2020/9/1.
//

#include "iterator.h"
#include "tctl_object.h"

static void *increment(void)
{
    iterator *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = PRIVATE_THIS(p_it->obj_this).iter_increment(p_it->ptr);
    return ptr;
}

static void *decrement(void)
{
    iterator *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = PRIVATE_THIS(p_it->obj_this).iter_decrement(p_it->ptr);
    return ptr;
}
typeof(*((iterator*)(0))->obj_func) def_obj_func = {increment, decrement};

iterator creat_iter(void *p)
{
    iterator it;
    it.obj_func = &def_obj_func;
    it.ptr = p;
    it.obj_this = pthis();
    return it;
}