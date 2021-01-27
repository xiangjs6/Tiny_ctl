//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algobase.h"
#include "../include/tctl_metaclass.h"
#include "../include/tctl_iterator.h"
#include "../include/auto_release_pool.h"
#include <assert.h>
#include <string.h>
#define Import ITERATOR
//copy
inline static Iterator copy_3S(Iterator first, Iterator last, Iterator result) //迭代器都为SequenceIter
{
    Form_t dst_t = THIS(result).type();
    void *mem = ARP_MallocARelDtor(sizeOf(result), destroy);
    Iterator out = THIS(result).ctor(mem, VA(result), VAEND);
    char *src_p = THIS(first).derefer();
    char *dst_p = THIS(result).derefer();
    if (dst_t.f == POD) { //并且输出迭代器的类型为POD
        size_t n = (char*)THIS(last).derefer() - src_p;
        memmove(dst_p, src_p, n);
        THIS(out).self_add(VA(n / dst_t.size));
    } else { //输出迭代器类型为OBJ
        Form_t src_t = THIS(first).type();
        size_t src_memb_size = src_t.f == POD ? src_t.size : classSz(src_t.class);
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
    size_t src_memb_size = src_t.f == POD ? src_t.size : classSz(src_t.class);
    size_t dist = distance(first, last);
    if (dst_t.f == POD) {
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
        if (dst_t.f == POD) {
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
        if (dst_t.f == POD) {
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
        if (dst_t.f == POD) {
            for (; !THIS(src_it).equal(VA(last)); THIS(src_it).inc(), THIS(out).inc())
                memcpy(THIS(out).derefer(), THIS(src_it).derefer(), dst_t.size);
        } else {
            for (; !THIS(src_it).equal(VA(last)); THIS(src_it).inc(), THIS(out).inc())
                construct(dst_t, THIS(out).derefer(), FORM_WITH_OBJ(src_t, THIS(src_it).derefer()));
        }
        return out;
    }
}

Iterator copy_backward(Iterator first, Iterator last, Iterator result)
{
    char tmp[sizeOf(first)];
    Form_t t = THIS(first).type();
    Form_t res_t = THIS(result).type();
    Iterator it = THIS(last).ctor(tmp, VA(last), VAEND);
    long long dist = distance(first, last);
    THIS(it).dec();
    THIS(result).dec();
    do
    {
        if (res_t.f == POD)
            memcpy(THIS(result).derefer(), THIS(it).derefer(), t.size);
        else 
            construct(res_t, THIS(result).derefer(), FORM_WITH_OBJ(t, THIS(it).derefer()));
        THIS(it).dec();
        THIS(result).dec();
    } while (THIS(it).cmp(VA(first)) >= 0);
    return result;
}
