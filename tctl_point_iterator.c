//
// Created by xjs on 2020/11/20.
//

#include "tctl_point_iterator.h"
#include "tctl_allocator.h"
//private
typedef void *__point_iter;

static void *iter_at(__iterator *iter, int pos)
{
    __point_iter *_iter = (__point_iter*)iter->__inner.iterator_func_p;
    pop_this();
    return *_iter + pos * iter->__inner.memb_size;
}

static void iter_increment(__iterator *iter)
{
    struct __point_iter *_iter = (struct __point_iter*)iter->__inner.iterator_func_p;
    pop_this();
    _iter->v += _iter->memb_size;
}

static void iter_decrement(__iterator *iter)
{
    struct __point_iter *_iter = (struct __point_iter*)iter->__inner.iterator_func_p;
    pop_this();
    _iter->v -= _iter->memb_size;
}

static void iter_add(__iterator *iter, int x)
{
    struct __point_iter *_iter = (struct __point_iter*)iter->__inner.__address;
    pop_this();
    _iter->v += x * _iter->memb_size;
}

static void iter_sub(__iterator *iter, int x)
{
    struct __point_iter *_iter = (struct __point_iter*)iter->__inner.__address;
    pop_this();
    _iter->v -= x * _iter->memb_size;
}

static long long iter_diff(const __iterator *minuend, const __iterator *subtraction)
{
    pop_this();
    struct __point_iter *__minuend = (struct __point_iter*)minuend->__inner.__address;
    struct __point_iter *__subtraction = (struct __point_iter*)subtraction->__inner.__address;
    return (__minuend->v - __subtraction->v) / (long long)__minuend->memb_size;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    pop_this();
    struct __point_iter *__it1 = (struct __point_iter*)it1->__inner.__address;
    struct __point_iter *__it2 = (struct __point_iter*)it2->__inner.__address;
    return __it1->v == __it2->v;
}

static const __iterator_obj_func  __def_array_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff,
        iter_equal
};

static const iterator_func __def_array_iter_func = INIT_ITER_FUNC(&__def_array_iter);

IterType __construct_array_iter(void *obj_this, size_t memb_size)
{
}
int freea(void)
{

}

