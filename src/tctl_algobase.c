//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algobase.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_class.h"
#include "../include/tctl_utility.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#define Import ITERATOR, OBJECT, PAIR
#define ALLOC(size) ARP_MallocARelDtor(size, destroy)
//copy
inline static Iterator copy_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Form_t dst_t = THIS(result).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    char *src_p = THIS(first).derefer();
    char *dst_p = THIS(result).derefer();
    if (dst_t.f == ADDR) { //并且输出迭代器的类型为ADDR
        size_t n = (char*)THIS(last).derefer() - src_p;
        memmove(dst_p, src_p, n);
        THIS(out).self_add(VA(n / dst_t.size));
    } else { //输出迭代器类型为OBJ
        Form_t src_t = THIS(first).type();
        size_t src_memb_size = src_t.f == ADDR ? src_t.size : classSz(src_t.class);
        size_t dst_memb_size = classSz(dst_t.class);
        size_t dist = distance(first, last);
        for (size_t i = 0; i < dist; i++) {
            construct(dst_t, dst_p, FORM_WITH_OBJ(src_t, VA(src_p).mem));
            src_p += src_memb_size;
            dst_p += dst_memb_size;
        }
        THIS(out).self_add(VA(dist));
    }
    return out;
}

inline static Iterator copy_2S(Iterator first, Iterator last, Iterator result) //first和last为SequenceIter，result为其他迭代器类型
{
    Form_t dst_t = THIS(result).type();
    Form_t src_t = THIS(first).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    char *src_p = THIS(first).derefer();
    size_t src_memb_size = src_t.f == ADDR ? src_t.size : classSz(src_t.class);
    size_t dist = distance(first, last);
    if (dst_t.f == ADDR) {
        while (dist--)
        {
            memcpy(THIS(out).derefer(), src_p, src_memb_size);
            src_p += src_memb_size;
            THIS(out).inc();
        }
    } else { //OBJ
        while (dist--)
        {
            construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, src_p));
            src_p += src_memb_size;
            THIS(out).inc();
        }
    }
    return out;
}

inline static Iterator copy_2R(Iterator first, Iterator last, Iterator result) //first和last为RandomAccessIter
{
    Form_t dst_t = THIS(result).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    char tmp[sizeOf(first)];
    Iterator src_it = THIS(first).ctor(tmp, VA(first), VAEND);
    Form_t src_t = THIS(first).type();
    size_t dist = distance(first, last);
    if (result->rank == SequenceIter) {
        char *dst_p = THIS(result).derefer();
        if (dst_t.f == ADDR) {
            size_t dst_memb_size = src_t.size;
            for (size_t i = 0; i < dist; i++) {
                memcpy(dst_p, THIS(src_it).derefer(), dst_t.size);
                dst_p += dst_memb_size;
                THIS(src_it).inc();
            }
        } else {
            size_t dst_memb_size = classSz(src_t.class);
            for (size_t i = 0; i < dist; i++) {
                construct(dst_t, dst_p, FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
                dst_p += dst_memb_size;
                THIS(src_it).inc();
            }
        }
        THIS(out).self_add(VA(dist));
    } else {
        if (dst_t.f == ADDR) {
            while (dist--)
            {
                memcpy(THIS(out).derefer(), THIS(src_it).derefer(), dst_t.size);
                THIS(out).inc(), THIS(src_it).inc();
            }
        } else {
            while (dist--)
            {
                construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
                THIS(out).inc(), THIS(src_it).inc();
            }
        }
    }
    destroy(src_it);
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
        Form_t dst_t = THIS(result).type();
        Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
        char tmp[sizeOf(first)];
        Iterator src_it = THIS(first).ctor(tmp, VA(first), VAEND);
        Form_t src_t = THIS(first).type();
        if (dst_t.f == ADDR) {
            for (; !THIS(src_it).equal(VA(last)); THIS(src_it).inc(), THIS(out).inc())
                memcpy(THIS(out).derefer(), THIS(src_it).derefer(), dst_t.size);
        } else {
            for (; !THIS(src_it).equal(VA(last)); THIS(src_it).inc(), THIS(out).inc())
                construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
        }
        return ARP_Return(out);
    }
}

//copy_backward
inline static Iterator copy_backward_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Form_t dst_t = THIS(result).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    char *dst_p = THIS(result).derefer();
    if (dst_t.f == ADDR) { //并且输出迭代器的类型为ADDR
        char *src_p = THIS(first).derefer();
        size_t n = (char*)THIS(last).derefer() - src_p;
        dst_p -= n;
        memmove(dst_p, src_p, n);
        THIS(out).self_sub(VA(n / dst_t.size));
    } else { //输出迭代器类型为OBJ
        Form_t src_t = THIS(first).type();
        size_t src_memb_size = src_t.f == ADDR ? src_t.size : classSz(src_t.class);
        size_t dst_memb_size = classSz(dst_t.class);
        char *src_p = (char*)THIS(last).derefer() - src_memb_size; //从后往前
        dst_p -= dst_memb_size;
        size_t dist = distance(first, last);
        for (size_t i = 0; i < dist; i++) {
            construct(dst_t, dst_p, FORM_WITH_OBJ(src_t, src_p));
            src_p -= src_memb_size;
            dst_p -= dst_memb_size;
        }
        THIS(out).self_sub(VA(dist));
    }
    return out;
}

inline static Iterator copy_backward_2S(Iterator first, Iterator last, Iterator result) //first和last为SequenceIter，result为其他迭代器类型
{
    Form_t dst_t = THIS(result).type();
    Form_t src_t = THIS(first).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    THIS(out).dec();

    size_t src_memb_size = src_t.f == ADDR ? src_t.size : classSz(src_t.class);
    char *src_p = (char*)THIS(last).derefer() - src_memb_size;
    size_t dist = distance(first, last);
    if (dst_t.f == ADDR) {
        while (dist--)
        {
            memcpy(THIS(out).derefer(), src_p, src_memb_size);
            src_p -= src_memb_size;
            THIS(out).dec();
        }
    } else { //OBJ
        while (dist--)
        {
            construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, src_p));
            src_p -= src_memb_size;
            THIS(out).dec();
        }
    }
    return out;
}

inline static Iterator copy_backward_2R(Iterator first, Iterator last, Iterator result) //first和last为RandomAccessIter
{
    Form_t dst_t = THIS(result).type();
    Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
    char tmp[sizeOf(first)];
    Iterator src_it = THIS(first).ctor(tmp, VA(last), VAEND);
    THIS(src_it).dec();

    Form_t src_t = THIS(first).type();
    size_t dist = distance(first, last);
    if (result->rank == SequenceIter) {
        char *dst_p = THIS(result).derefer();
        if (dst_t.f == ADDR) {
            size_t dst_memb_size = src_t.size;
            dst_p -= dst_memb_size;
            for (size_t i = 0; i < dist; i++) {
                memcpy(dst_p, THIS(src_it).derefer(), dst_t.size);
                dst_p += dst_memb_size;
                THIS(src_it).dec();
            }
        } else {
            size_t dst_memb_size = classSz(src_t.class);
            dst_p -= dst_memb_size;
            for (size_t i = 0; i < dist; i++) {
                construct(dst_t, dst_p, FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
                dst_p -= dst_memb_size;
                THIS(src_it).dec();
            }
        }
        THIS(out).self_sub(VA(dist));
    } else {
        THIS(out).dec();
        if (dst_t.f == ADDR) {
            while (dist--)
            {
                memcpy(THIS(out).derefer(), THIS(src_it).derefer(), dst_t.size);
                THIS(out).dec(), THIS(src_it).dec();
            }
        } else {
            while (dist--)
            {
                construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
                THIS(out).dec(), THIS(src_it).dec();
            }
        }
    }
    destroy(src_it);
    return out;
}

Iterator copy_backward(Iterator first, Iterator last, Iterator result)
{
    assert(classOf(first) == classOf(last));
    ARP_CreatePool();
    if (first->rank == SequenceIter && result->rank == SequenceIter) { //都为顺序迭代器
        return ARP_Return(copy_backward_3S(first, last, result));
    } else if (first->rank == SequenceIter) { //first和last为SequenceIter
        return ARP_Return(copy_backward_2S(first, last, result));
    } else if (first->rank == RandomAccessIter) { //first和last为RandomAccessIter
        return ARP_Return(copy_backward_2R(first, last, result));
    } else { //first和last都为BidirectionalIter或ForwardIter迭代器
        Form_t dst_t = THIS(result).type();
        Iterator out = THIS(result).ctor(ALLOC(sizeOf(result)), VA(result), VAEND);
        THIS(out).dec();

        char tmp[sizeOf(first)];
        Iterator src_it = THIS(first).ctor(tmp, VA(last), VAEND);
        THIS(src_it).dec();
        Form_t src_t = THIS(first).type();
        if (dst_t.f == ADDR) {
            do
            {
                memcpy(THIS(out).derefer(), THIS(src_it).derefer(), dst_t.size);
                THIS(out).dec();
                THIS(src_it).dec();
            } while (!THIS(src_it).equal(VA(first)));
        } else {
            do
            {
                construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
                THIS(out).dec();
                THIS(src_it).dec();
            } while (!THIS(src_it).equal(VA(first)));
        }
        return ARP_Return(out);
    }
}

inline static int CompareOpt(FormWO_t a, FormWO_t b, FormWO_t op)
{
    if (a._.f != OBJ && b._.f == OBJ)
        return -CompareOpt(b, a, op);
    if (op._.f != FUNC) {
        if (a._.f == OBJ) {
            Object obj = a.mem;
            return THIS(obj).cmp(b);
        } else {
            assert(b._.f != OBJ);
            assert(a._.size == b._.size);
            assert(!(a._.f == POD && a._.size > sizeof(a.mem)));
            assert(!(b._.f == POD && b._.size > sizeof(b.mem)));
            if (a._.f == ADDR && b._.f == ADDR)
                return memcmp(a.mem, b.mem, a._.size);
            else if (a._.f == ADDR && b._.f == POD)
                return memcmp(a.mem, &b.mem, a._.size);
            else if (a._.f == POD && b._.f == ADDR)
                return memcmp(&a.mem, b.mem, a._.size);
            else
                return memcmp(&a.mem, &b.mem, a._.size);
        }
    } else {
        Compare cmp = op.mem;
        return cmp(a, b);
    }
}

static inline void AssignOpt(FormWO_t left, FormWO_t right)
{
    if (left._.f == OBJ) {
        Object obj = left.mem;
        THIS(obj).assign(right);
    } else {
        assert(right._.f != OBJ);
        assert(left._.size == right._.size);
        assert(!(left._.f == POD && left._.size > sizeof(left.mem)));
        assert(!(right._.f == POD && right._.size > sizeof(right.mem)));
        if (left._.f == ADDR && right._.f == ADDR)
            memcpy(left.mem, right.mem, left._.size);
        else if (left._.f == ADDR && right._.f == POD)
            memcpy(left.mem, &right.mem, left._.size);
        else if (left._.f == POD && right._.f == POD)
            memcpy(&left.mem, &right.mem, left._.size);
        else
            memcpy(&left.mem, right.mem, left._.size);
    }
}

//equal
bool equal(Iterator _first1, Iterator _last1, Iterator _first2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), VA(_first2), VAEND);
    va_list ap;
    va_start(ap, _first2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc(), THIS(first2).inc())
        if (CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                       FORM_WITH_OBJ(f2, THIS(first2).derefer()), op)) {
            ARP_FreePool();
            return false;
        }
    ARP_FreePool();
    return true;
}

//fill
void fill(Iterator _first, Iterator _last, FormWO_t x)
{
    ARP_CreatePool();
    Form_t f = THIS(_first).type();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), VA(_first), VAEND);
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), x);
    ARP_FreePool();
}

//fill_n
void fill_n(Iterator _first, size_t n, FormWO_t x)
{
    ARP_CreatePool();
    Form_t f = THIS(_first).type();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), VA(_first), VAEND);
    for (; n--; THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), x);
    ARP_FreePool();
}

//iter_swap
void iter_swap(Iterator a, Iterator b)
{
    Form_t f = THIS(a).type();
    assert(f.f == THIS(b).type().f && f.class == THIS(b).type().class);

    if (f.f == OBJ) {
        Object obj_a = THIS(a).derefer();
        Object obj_b = THIS(b).derefer();
        char mem[sizeOf(obj_a)];
        Object tmp = construct(f, mem, VAEND);
        AssignOpt(FORM_WITH_OBJ(f, tmp), FORM_WITH_OBJ(f, obj_a));
        AssignOpt(FORM_WITH_OBJ(f, obj_a), FORM_WITH_OBJ(f, obj_b));
        AssignOpt(FORM_WITH_OBJ(f, obj_b), FORM_WITH_OBJ(f, tmp));
        destroy(tmp);
    } else {
        char mem[f.size];
        void *p_a = THIS(a).derefer();
        void *p_b = THIS(b).derefer();
        AssignOpt(FORM_WITH_OBJ(f, mem), FORM_WITH_OBJ(f, p_a));
        AssignOpt(FORM_WITH_OBJ(f, p_a), FORM_WITH_OBJ(f, p_b));
        AssignOpt(FORM_WITH_OBJ(f, p_b), FORM_WITH_OBJ(f, mem));
    }
}

//lexicographical_compare
bool lexicographical_compare(Iterator _first1, Iterator _last1, Iterator _first2, Iterator _last2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), VA(_first2), VAEND);

    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    for (; !THIS(first1).equal(VA(_last1)) &&
           !THIS(first2).equal(VA(_last2));
           THIS(first1).inc(), THIS(first2).inc()) {
        int res = CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                             FORM_WITH_OBJ(f2, THIS(first2).derefer()), op);
        if (res < 0)
            return true;
        else if (res > 0)
            return false;
    }
    bool res = THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2));
    ARP_FreePool();
    return res;
}

//max
FormWO_t max(FormWO_t a, FormWO_t b, .../*Compare*/)
{
    va_list ap;
    va_start(ap, b);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    int res = CompareOpt(a, b, op);
    if (res > 0)
        return a;
    else
        return b;
}

//min
FormWO_t min(FormWO_t a, FormWO_t b, .../*Compare*/)
{
    va_list ap;
    va_start(ap, b);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    int res = CompareOpt(a, b, op);
    if (res < 0)
        return a;
    else
        return b;
}

//mismatch
Pair mismatch(Iterator _first1, Iterator _last1, Iterator _first2, .../*Compare*/)
{
    ARP_CreatePool();
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();

    va_list ap;
    va_start(ap, _first2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc(), THIS(first2).inc()) {
        int res = CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                             FORM_WITH_OBJ(f2, THIS(first2).derefer()), op);
        if (res)
            break;
    }
    
    Pair pair = tmpPair((Form_t){OBJ, {.class = classOf(first1)}},
                        (Form_t){OBJ, {.class = classOf(first2)}},
                        VA(first1, first2), VAEND);
    return ARP_Return(pair);
}

//swap
void swap(FormWO_t a, FormWO_t b)
{
    assert(a._.f == ADDR || a._.f == OBJ);
    assert(b._.f == ADDR || b._.f == OBJ);
    assert(a._.class == b._.class);
    Form_t f = a._;

    if (a._.f == OBJ) {
        Object obj_a = a.mem;
        Object obj_b = b.mem;
        char mem[sizeOf(obj_a)];
        Object tmp = construct(a._, mem, VAEND);
        AssignOpt(FORM_WITH_OBJ(f, tmp), FORM_WITH_OBJ(f, obj_a));
        AssignOpt(FORM_WITH_OBJ(f, obj_a), FORM_WITH_OBJ(f, obj_b));
        AssignOpt(FORM_WITH_OBJ(f, obj_b), FORM_WITH_OBJ(f, tmp));
        destroy(tmp);
    } else {
        char mem[a._.size];
        void *p_a = a.mem;
        void *p_b = b.mem;
        AssignOpt(FORM_WITH_OBJ(f, mem), FORM_WITH_OBJ(f, p_a));
        AssignOpt(FORM_WITH_OBJ(f, p_a), FORM_WITH_OBJ(f, p_b));
        AssignOpt(FORM_WITH_OBJ(f, p_b), FORM_WITH_OBJ(f, mem));
    }
}
