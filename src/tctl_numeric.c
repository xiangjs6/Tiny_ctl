//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_numeric.h"
#include "../include/auto_release_pool.h"

void *accumulate(const IterType first, const IterType last, void *init, BinaryOperation binary_op)
{
    __iterator *__first = copy_iter(first);
    __iterator *__last = copy_iter(last);
    for (; !ITER(__first).equal(__last); ITER(__first).inc())
        binary_op(init, __first->val, init);
    free_iter(__first);
    free_iter(__last);
    return init;
}

IterType adjacent_difference(const IterType first, const IterType last, const IterType result, BinaryOperation binary_op)
{
    __iterator *__first = copy_iter(first);
    __iterator *__last = copy_iter(last);
    __iterator *__res = copy_iter(result);
    ARP_JoinARel(__res);
    binary_op(__first->val, NULL, __res->val);
    void *pre_val = __first->val;
    for(ITER(__first).inc(), ITER(__res).inc(); !ITER(__first).equal(__last); ITER(__first).inc(), ITER(__res).inc()) {
        binary_op(__first->val, pre_val, __res->val);
        pre_val = __first->val;
    }
    free_iter(__first);
    free_iter(__last);
    return __res;
}

void *inner_product(const IterType first1, const IterType last1, const IterType first2, void *init, BinaryOperation binary_op1, BinaryOperation binary_op2)
{
    ARP_CreatePool();
    __iterator *__first1 = copy_iter(first1);
    __iterator *__last1 = copy_iter(last1);
    __iterator *__first2 = copy_iter(first2);
    void *val_type = NULL;
    for (; !ITER(__first1).equal(__last1); ITER(__first1).inc(), ITER(__first2).inc()) {
        val_type = binary_op2(__first1->val, __first2->val, val_type);
        binary_op1(init, val_type, init);
    }
    free_iter(__first1);
    free_iter(__first2);
    free_iter(__last1);
    ARP_FreePool();
    return init;
}

IterType partial_sum(const IterType first, const IterType last, const IterType result, BinaryOperation binary_op)
{
    __iterator *__first = copy_iter(first);
    __iterator *__last = copy_iter(last);
    __iterator *__res = copy_iter(result);
    ARP_JoinARel(__res);
    binary_op(__first->val, NULL, __res->val);
    void *sum_val = __res->val;
    for(ITER(__first).inc(), ITER(__res).inc(); !ITER(__first).equal(__last); ITER(__first).inc(), ITER(__res).inc()) {
        binary_op(__first->val, sum_val, __res->val);
        sum_val = __res->val;
    }
    free_iter(__first);
    free_iter(__last);
    return __res;
}
