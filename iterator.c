//
// Created by xjs on 2020/9/1.
//

#include "iterator.h"
#include "tctl_object.h"

static void *increment(void)
{
    iter *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = PRIVATE_THIS(p_it->obj_this).iter_increment(p_it->ptr);
    return ptr;
}

static void *decrement(void)
{
    iter *p_it = *pthis();
    void *ptr = p_it->ptr;
    p_it->ptr = PRIVATE_THIS(p_it->obj_this).iter_decrement(p_it->ptr);
    return ptr;
}
typeof(*((iter*)(0))->obj_func) def_obj_func = {increment, decrement};

iter creat_iter(void *p)
{
    iter it;
    it.obj_func = &def_obj_func;
    it.ptr = p;
    it.obj_this = pthis();
    return it;
}