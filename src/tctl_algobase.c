//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algobase.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_def.h"
#include "../include/tctl_int.h"
#include <assert.h>
#include <stdarg.h>
#define Import ITERATOR, OBJECT, PAIR, INT
#define ALLOC(size) ARP_MallocARelDtor(size, destroy)
//copy
inline static Iterator copy_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    char *src_p = THIS(first).derefer();
    char *dst_p = THIS(result).derefer();
    size_t src_memb_size = classSz(first->class);
    size_t dst_memb_size = classSz(result->class);
    size_t dist = distance(first, last);
    for (size_t i = 0; i < dist; i++) {
        construct(result->class, dst_p, src_p, VAEND);
        src_p += src_memb_size;
        dst_p += dst_memb_size;
    }
    THIS(out).self_add(VA(dist));
    return out;
}

inline static Iterator copy_2S(Iterator first, Iterator last, Iterator result) //first和last为SequenceIter，result为其他迭代器类型
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    char *src_p = THIS(first).derefer();
    size_t src_memb_size = classSz(first->class);
    size_t dist = distance(first, last);
    while (dist--)
    {
        construct(result->class, THIS(out).derefer(), src_p, VAEND);
        src_p += src_memb_size;
        THIS(out).inc();
    }
    return out;
}

inline static Iterator copy_2R(Iterator first, Iterator last, Iterator result) //first和last为RandomAccessIter
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    Iterator src_it = THIS(first).ctor(ALLOC(sizeOf(first)), first, VAEND);
    size_t dist = distance(first, last);
    if (result->rank == SequenceIter) {
        char *dst_p = THIS(result).derefer();
        size_t dst_memb_size = classSz(first->class);
        for (size_t i = 0; i < dist; i++) {
            construct(result->class, dst_p, THIS(src_it).derefer(), VAEND);
            dst_p += dst_memb_size;
            THIS(src_it).inc();
        }
        THIS(out).self_add(VA(dist));
    } else {
        while (dist--)
        {
            construct(result->class, THIS(out).derefer(), THIS(src_it).derefer());
            THIS(out).inc(), THIS(src_it).inc();
        }
    }
    return out;
}

Iterator copy(Iterator first, Iterator last, Iterator result)
{
    assert(classOf(first) == classOf(last));
    ARP_CreatePool();
    if (first->rank == SequenceIter && result->rank == SequenceIter) { //都为顺序迭代器
        return ARP_Return(copy_3S(first, last, result));
    } else if (first->rank == SequenceIter) { //first和last为SequenceIter
        return ARP_Return(copy_2S(first, last, result));
    } else if (first->rank == RandomAccessIter) { //first和last为RandomAccessIter
        return ARP_Return(copy_2R(first, last, result));
    } else { //first和last都为BidirectionalIter或ForwardIter迭代器
        Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
        Iterator src_it = THIS(first).ctor(ALLOC(sizeOf(first)), first, VAEND);
        for (; !THIS(src_it).equal(last); THIS(src_it).inc(), THIS(out).inc())
            construct(result->class, THIS(out).derefer(), THIS(src_it).derefer(), VAEND);
        return ARP_Return(out);
    }
}

//copy_backward
inline static Iterator copy_backward_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    char *dst_p = THIS(result).derefer();
    size_t src_memb_size = classSz(first->class);
    size_t dst_memb_size = classSz(result->class);
    char *src_p = (char*)THIS(last).derefer() - src_memb_size; //从后往前
    dst_p -= dst_memb_size;
    size_t dist = distance(first, last);
    for (size_t i = 0; i < dist; i++) {
        construct(result->class, dst_p, src_p, VAEND);
        src_p -= src_memb_size;
        dst_p -= dst_memb_size;
    }
    THIS(out).self_sub(VA(dist));
    return out;
}

inline static Iterator copy_backward_2S(Iterator first, Iterator last, Iterator result) //first和last为SequenceIter，result为其他迭代器类型
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    THIS(out).dec();

    size_t src_memb_size = classSz(first->class);
    char *src_p = (char*)THIS(last).derefer() - src_memb_size;
    size_t dist = distance(first, last);
    while (dist--)
    {
        construct(result->class, THIS(out).derefer(), src_p, VAEND);
        src_p -= src_memb_size;
        THIS(out).dec();
    }

    return out;
}

inline static Iterator copy_backward_2R(Iterator first, Iterator last, Iterator result) //first和last为RandomAccessIter
{
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
    Iterator src_it = THIS(first).ctor(ALLOC(sizeOf(first)), last, VAEND);
    THIS(src_it).dec();

    size_t dist = distance(first, last);
    if (result->rank == SequenceIter) {
        char *dst_p = THIS(result).derefer();
        size_t dst_memb_size = classSz(first->class);
        dst_p -= dst_memb_size;
        for (size_t i = 0; i < dist; i++) {
            construct(result->class, dst_p, THIS(src_it).derefer(), VAEND);
            dst_p -= dst_memb_size;
            THIS(src_it).dec();
        }
        THIS(out).self_sub(VA(dist));
    } else {
        THIS(out).dec();
        while (dist--)
        {
            construct(result->class, THIS(out).derefer(), THIS(src_it).derefer(), VAEND);
            THIS(out).dec(), THIS(src_it).dec();
        }
    }
    return out;
}

Iterator copy_backward(Iterator first, Iterator last, Iterator result)
{
    assert(classOf(first) == classOf(last));
    assert(first->rank >= BidirectionalIter && result->rank >= BidirectionalIter);
    ARP_CreatePool();
    if (first->rank == SequenceIter && result->rank == SequenceIter) { //都为顺序迭代器
        return ARP_Return(copy_backward_3S(first, last, result));
    } else if (first->rank == SequenceIter) { //first和last为SequenceIter
        return ARP_Return(copy_backward_2S(first, last, result));
    } else if (first->rank == RandomAccessIter) { //first和last为RandomAccessIter
        return ARP_Return(copy_backward_2R(first, last, result));
    } else { //first和last都为BidirectionalIter
        Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), result, VAEND);
        THIS(out).dec();

        Iterator src_it = THIS(first).ctor(ALLOC(sizeOf(first)), last, VAEND);
        THIS(src_it).dec();
        do
        {
            construct(result->class, THIS(out).derefer(), THIS(src_it).derefer(), VAEND);
            THIS(out).dec();
            THIS(src_it).dec();
        } while (!THIS(src_it).equal(first));
        return ARP_Return(out);
    }
}

inline static int CompareOpt(const void *a, const void *b, Compare op)
{
    if (op == *(Compare*)&VAEND) {
        assert(class_check(a, T(Object)));
        Object obj = (void*)a;
        return THIS(obj).cmp(b);
    } else {
        return op(a, b);
    }
}

static inline void AssignOpt(void *left, const void *right)
{
    assert(class_check(left, T(Object)));
    Object obj = left;
    THIS(obj).assign(right);
}

//equal
bool equal(Iterator _first1, Iterator _last1, Iterator _first2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    va_list ap;
    va_start(ap, _first2);
    void *op = va_arg(ap, void*);
    va_end(ap);
    for (; !THIS(first1).equal(_last1); THIS(first1).inc(), THIS(first2).inc())
        if (CompareOpt(THIS(first1).derefer(),
                       THIS(first2).derefer(), *(Compare*)&op)) {
            ARP_FreePool();
            return false;
        }
    ARP_FreePool();
    return true;
}

//fill
void fill(Iterator _first, Iterator _last, const void *x)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc())
        AssignOpt(THIS(first).derefer(), x);
    ARP_FreePool();
}

//fill_n
void fill_n(Iterator _first, const void *_n, const void *x)
{
    ARP_CreatePool();
    Int n = (void*)_n;
    n = THIS(n).cast(T(Int));
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; n->val--; THIS(first).inc())
        AssignOpt(THIS(first).derefer(), x);
    ARP_FreePool();
}

//iter_swap
void iter_swap(Iterator a, Iterator b)
{
    assert(a->class == b->class);

    Object obj_a = THIS(a).derefer();
    Object obj_b = THIS(b).derefer();
    char mem[sizeOf(obj_a)];
    Object tmp = construct(a->class, mem, VAEND);
    AssignOpt(tmp, obj_a);
    AssignOpt(obj_a, obj_b);
    AssignOpt(obj_b, tmp);
    destroy(tmp);
}

//lexicographical_compare
bool lexicographical_compare(Iterator _first1, Iterator _last1, Iterator _first2, Iterator _last2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    va_list ap;
    va_start(ap, _last2);
    void *op = va_arg(ap, void *);
    va_end(ap);

    for (; !THIS(first1).equal(_last1) &&
           !THIS(first2).equal(_last2);
           THIS(first1).inc(), THIS(first2).inc()) {
        int res = CompareOpt(THIS(first1).derefer(),
                             THIS(first2).derefer(), *(Compare*)&op);
        if (res < 0) {
            ARP_FreePool();
            return true;
        } else if (res > 0) {
            ARP_FreePool();
            return false;
        }
    }
    bool res = THIS(first1).equal(_last1) && !THIS(first2).equal(_last2);
    ARP_FreePool();
    return res;
}

//max
void *max(const void *a, const void *b, .../*Compare*/)
{
    va_list ap;
    va_start(ap, b);
    void *op = va_arg(ap, void *);
    va_end(ap);

    int res = CompareOpt(a, b, *(Compare*)&op);
    if (res > 0)
        return (void*)a;
    else
        return (void*)b;
}

//min
void *min(const void *a, const void *b, .../*Compare*/)
{
    va_list ap;
    va_start(ap, b);
    void *op = va_arg(ap, void*);
    va_end(ap);

    int res = CompareOpt(a, b, *(Compare*)&op);
    if (res < 0)
        return (void*)a;
    else
        return (void*)b;
}

//mismatch
Pair mismatch(Iterator _first1, Iterator _last1, Iterator _first2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    va_list ap;
    va_start(ap, _first2);
    void *op = va_arg(ap, void*);
    va_end(ap);

    for (; !THIS(first1).equal(_last1); THIS(first1).inc(), THIS(first2).inc()) {
        int res = CompareOpt(THIS(first1).derefer(),
                             THIS(first2).derefer(), *(Compare*)&op);
        if (res)
            break;
    }
    
    Pair pair = tmpPair(classOf(first1), classOf(first2), first1, first2, VAEND);
    return ARP_Return(pair);
}

//swap
void swap(void *a, void *b)
{
    assert(classOf(a) == classOf(b));

    Object obj_a = a;
    Object obj_b = b;
    char mem[sizeOf(obj_a)];
    Object tmp = construct(classOf(a), mem, VAEND);
    AssignOpt(tmp, obj_a);
    AssignOpt(obj_a, obj_b);
    AssignOpt(obj_b, tmp);
    destroy(tmp);
}
