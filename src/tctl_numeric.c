//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_numeric.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include <stdarg.h>
#define Import ITERATOR

#define ALLOC(size) ARP_MallocARelDtor(size, destroy)

static inline void *AddOpt(void *first, void *second, BinaryOperation op)
{
    if (op == *(BinaryOperation*)&VAEND) {
        Object obj = first;
        return THIS(obj).add(second);
    } else {
        return op(first, second);
    }
}

static inline void *SubOpt(void *first, void *second, BinaryOperation op)
{
if (op == *(BinaryOperation*)&VAEND) {
        Object obj = first;
        return THIS(obj).sub(second);
    } else {
        return op(first, second);
    }
}

static inline void *MulOpt(void *first, void *second, BinaryOperation op)
{
    if (op == *(BinaryOperation*)&VAEND) {
        Object obj = first;
        return THIS(obj).mul(second);
    } else {
        return op(first, second);
    }
}

static inline void AssignOpt(void *left, const void *right)
{
    Object obj = left;
    THIS(obj).assign(right);
}

void *accumulate(Iterator _first, Iterator _last, const void *init, ...)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);
    Object ret = THIS((Object)init).ctor(ALLOC(sizeOf(init)), init, VAEND);
    va_list ap;
    va_start(ap, init);
    void *t = va_arg(ap, void*);
    va_end(ap);
    for (; !THIS(first).equal(last); THIS(first).inc()) {
        void *val = THIS(first).derefer();
        ret = AddOpt(ret, val, *(BinaryOperation*)&t);
    }
    return ARP_Return(ret);
}

Iterator adjacent_difference(Iterator _first, Iterator _last, Iterator _result, ...)
{
    if (THIS(_first).equal(_last))
        return THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    ARP_CreatePool();
    Iterator res = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);

    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    AssignOpt(THIS(res).derefer(),
              THIS(first).derefer());
    void *pre_val = THIS(first).derefer(); //相当于指向first的内容
    for(THIS(first).inc(), THIS(res).inc(); !THIS(first).equal(last); THIS(first).inc(), THIS(res).inc()) {
        void *tmp = SubOpt(THIS(first).derefer(), pre_val, *(BinaryOperation*)&op);
        AssignOpt(THIS(res).derefer(), tmp);
        pre_val = THIS(first).derefer();
    }
    return ARP_Return(res);
}

void *inner_product(Iterator _first1, Iterator _last1, Iterator _first2, const void *init, ...)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator last1 = THIS(_last1).ctor(ALLOC(sizeOf(_last1)), _last1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    void *ret = THIS((Object)init).ctor(ALLOC(sizeOf(init)), init, VAEND);

    enum {Add, Mul};
    void *vargs[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, init);
    for (int i = 0; i < 2; i++)
        if ((vargs[i] = va_arg(ap, void*)) == VAEND)
            break;
    va_end(ap);

    for (; !THIS(first1).equal(last1); THIS(first1).inc(), THIS(first2).inc()) {
        void *tmp = MulOpt(THIS(first1).derefer(),
                           THIS(first2).derefer(), *(BinaryOperation*)&vargs[Mul]);
        ret = AddOpt(ret, tmp, *(BinaryOperation*)&vargs[Add]);
    }

    return ARP_Return(ret);
}

Iterator partial_sum(Iterator _first, Iterator _last, Iterator _result, ...)
{
    if (THIS(_first).equal(_last))
        return THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    ARP_CreatePool();
    Iterator res = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);

    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    AssignOpt(THIS(res).derefer(), THIS(first).derefer());
    void *pre_val = THIS(first).derefer();
    for(THIS(first).inc(), THIS(res).inc(); !THIS(first).equal(last); THIS(first).inc(), THIS(res).inc()) {
        void *tmp = AddOpt(THIS(first).derefer(), pre_val, *(BinaryOperation*)&op);
        AssignOpt(THIS(res).derefer(), tmp);
        pre_val = THIS(first).derefer();
    }

    return ARP_Return(res);
}

void iota(Iterator _first, Iterator _last, const void *_x, ...)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_x)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_x)), _last, VAEND);
    void *x = THIS((Object)_x).ctor(ALLOC(sizeOf(_x)), _x, VAEND);

    void *op;
    va_list ap;
    va_start(ap, _x);
    op = va_arg(ap, void*);
    va_end(ap);

    while (!THIS(first).equal(last))
    {
        AssignOpt(THIS(first).derefer(), x);
        x = AddOpt(x, VA(1), *(BinaryOperation*)&op);
    }
    ARP_FreePool();
}
