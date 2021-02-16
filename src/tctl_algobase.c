//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algobase.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_class.h"
#include "../include/tctl_utility.h"
#include <assert.h>
#include <stdarg.h>
#include <stdarg.h>
#include <string.h>
#define Import ITERATOR, OBJECT, PAIR
//copy
inline static Iterator copy_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Form_t dst_t = THIS(result).type();
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
            construct(dst_t, dst_p, FORM_WITH_OBJ(src_t, src_p));
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
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
    if (first->rank == SequenceIter && result->rank == SequenceIter) { //都为顺序迭代器
        return copy_3S(first, last, result);
    } else if (first->rank == SequenceIter) { //first和last为SequenceIter
        return copy_2S(first, last, result);
    } else if (first->rank == RandomAccessIter) { //first和last为RandomAccessIter
        return copy_2R(first, last, result);
    } else { //first和last都为BidirectionalIter或ForwardIter迭代器
        Form_t dst_t = THIS(result).type();
        void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
        Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
        return out;
    }
}

//copy_backward
inline static Iterator copy_backward_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Form_t dst_t = THIS(result).type();
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
    if (first->rank == SequenceIter && result->rank == SequenceIter) { //都为顺序迭代器
        return copy_backward_3S(first, last, result);
    } else if (first->rank == SequenceIter) { //first和last为SequenceIter
        return copy_backward_2S(first, last, result);
    } else if (first->rank == RandomAccessIter) { //first和last为RandomAccessIter
        return copy_backward_2R(first, last, result);
    } else { //first和last都为BidirectionalIter或ForwardIter迭代器
        Form_t dst_t = THIS(result).type();
        void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
        Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
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
        return out;
    }
}

inline static int CompareOpt(FormWO_t a, FormWO_t b, FormWO_t op)
{
    if (op._.f != FUNC) {
        if (a._.f == OBJ) {
            Object obj = a.mem;
            return THIS(obj).cmp(b);
        } else {
            if (op._.f == ADDR)
                return memcmp(a.mem, b.mem, a._.size);
            else
                return memcmp(&a.mem, &b.mem, a._.size);
        }
    } else {
        Compare cmp = op.mem;
        return cmp(a, b);
    }
}

static inline void AssignOpt(FormWO_t left, FormWO_t right, FormWO_t op)
{
    if (op._.f != FUNC) {
        if (left._.f == OBJ) {
            Object obj = left.mem;
            THIS(obj).assign(right);
        } else {
            if (op._.f == ADDR)
                memcpy(left.mem, right.mem, left._.size);
            else
                memcpy(&left.mem, &right.mem, left._.size);
        }
    } else {
        AssignOperation func = op.mem;
        func(left, right);
    }
}

//equal
bool equal(Iterator _first1, Iterator _last1, Iterator _first2, .../*Compare*/)
{
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    va_list ap;
    va_start(ap, _first2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc(), THIS(first2).inc())
        if (!CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()), 
                        FORM_WITH_OBJ(f2, THIS(first2).derefer()), op)) {
            delete(first1);
            delete(first2);
            return false;
        }
    delete(first1);
    delete(first2);
    return true;
}

//fill
void fill(Iterator _first, Iterator _last, FormWO_t x, .../*Assign*/)
{
    va_list ap;
    va_start(ap, x);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    Form_t f = THIS(_first).type();
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), x, op);
}

//fill_n
void fill_n(Iterator _first, size_t n, FormWO_t x, .../*Assign*/)
{
    va_list ap;
    va_start(ap, x);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    Form_t f = THIS(_first).type();
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    for (; n--; THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), x, op);
}

//iter_swap
void iter_swap(Iterator a, Iterator b, .../*Swap*/)
{
    Form_t f = THIS(a).type();
    assert(f.f == THIS(b).type().f && f.class == THIS(b).type().class);
    va_list ap;
    va_start(ap, b);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    if (op._.f != FUNC) {
        if (f.f == OBJ) {
            Object obj_a = THIS(a).derefer();
            Object obj_b = THIS(b).derefer();
            char mem[sizeOf(obj_a)];
            Object tmp = construct(f, mem, VAEND);
            THIS(tmp).assign(FORM_WITH_OBJ(f, obj_a));
            THIS(obj_a).assign(FORM_WITH_OBJ(f, obj_b));
            THIS(obj_b).assign(FORM_WITH_OBJ(f, tmp));
            destroy(tmp);
        } else {
            char mem[f.size];
            void *p_a = THIS(a).derefer();
            void *p_b = THIS(b).derefer();
            memcpy(mem, p_a, f.size);
            memcpy(p_a, p_b, f.size);
            memcpy(p_b, mem, f.size);
        }
    } else {
        SwapFunc func = op.mem;
        func(FORM_WITH_OBJ(f, THIS(a).derefer()),
             FORM_WITH_OBJ(f, THIS(b).derefer));
    }
}

//lexicographical_compare
bool lexicographical_compare(Iterator _first1, Iterator _last1, Iterator _first2, Iterator _last2, .../*Compare*/)
{
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);

    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    for (; !THIS(first1).equal(VA(first2)) &&
           !THIS(first2).equal(VA(_last2));
           THIS(first1).inc(), THIS(first2).inc()) {
        int res = CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                             FORM_WITH_OBJ(f2, THIS(first2).derefer()), op);
        if (res < 0)
            return true;
        else if (res > 0)
            return false;
    }
    return THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2));
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
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();

    va_list ap;
    va_start(ap, _first2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc()) {
        int res = CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                             FORM_WITH_OBJ(f2, THIS(first2).derefer()), op);
        if (res)
            break;
    }
    
    Pair pair = tmpPair(f1, f2, VA(first1, first2));
    delete(first1);
    delete(first2);
    return pair;
}

//swap
void swap(FormWO_t a, FormWO_t b, .../*Swap*/)
{
    assert(a._.f == ADDR || a._.f == OBJ);
    assert(b._.f == ADDR || b._.f == OBJ);
    assert(a._.class == b._.class);
    va_list ap;
    va_start(ap, b);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    if (op._.f != FUNC) {
        if (a._.f == OBJ) {
            Object obj_a = a.mem;
            Object obj_b = b.mem;
            char mem[sizeOf(obj_a)];
            Object tmp = construct(a._, mem, VAEND);
            THIS(tmp).assign(FORM_WITH_OBJ(a._, obj_a));
            THIS(obj_a).assign(FORM_WITH_OBJ(a._, obj_b));
            THIS(obj_b).assign(FORM_WITH_OBJ(a._, tmp));
            destroy(tmp);
        } else {
            char mem[a._.size];
            void *p_a = a.mem;
            void *p_b = b.mem;
            memcpy(mem, p_a, a._.size);
            memcpy(p_a, p_b, a._.size);
            memcpy(p_b, mem, a._.size);
        }
    } else {
        SwapFunc func = op.mem;
        func(a, b);
    }
}
