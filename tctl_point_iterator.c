//
// Created by xjs on 2020/11/20.
//

#include "tctl_point_iterator.h"
#include "tctl_allocator.h"
#include "auto_release_pool/auto_release_pool.h"
#include <memory.h>
//private
typedef void *__point_iter;

static void *iter_at(__iterator *iter, int pos)
{
    __point_iter *__iter = (__point_iter*)iter->__inner.__address;
    return *__iter + pos * iter->__inner.memb_size;
}

static void iter_increment(__iterator *iter)
{
    __point_iter *__iter = (__point_iter*)iter->__inner.__address;
    *__iter += iter->__inner.memb_size;
}

static void iter_decrement(__iterator *iter)
{
    __point_iter *__iter = (__point_iter*)iter->__inner.__address;
    *__iter -= iter->__inner.memb_size;
}

static void iter_add(__iterator *iter, int x)
{
    __point_iter *__iter = (__point_iter*)iter->__inner.__address;
    *__iter += x * iter->__inner.memb_size;
}

static void iter_sub(__iterator *iter, int x)
{
    __point_iter *__iter = (__point_iter*)iter->__inner.__address;
    *__iter -= x * iter->__inner.memb_size;
}

static long long iter_diff(const __iterator *minuend, const __iterator *subtraction)
{
    __point_iter *__minuend = (__point_iter*)minuend->__inner.__address;
    __point_iter *__subtraction = (__point_iter*)subtraction->__inner.__address;
    return (*__minuend - *__subtraction) / (long long)minuend->__inner.memb_size;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    __point_iter *__it1 = (__point_iter*)it1->__inner.__address;
    __point_iter *__it2 = (__point_iter*)it2->__inner.__address;
    return *__it1 == *__it2;
}

static const __iterator_obj_func  __def_point_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff,
        iter_equal
};

static const iterator_func __def_point_iter_func = INIT_ITER_FUNC(&__def_point_iter);

IterType __construct_point_iter(void *p, void *obj_this, size_t memb_size)
{
    struct __inner_iterator *point = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__point_iter));
    *point = __creat_iter(sizeof(__point_iter), obj_this, memb_size, &__def_point_iter_func);
    memcpy(point->__address, &p, sizeof(__point_iter));
    return point;
}
