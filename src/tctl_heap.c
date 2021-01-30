//
// Created by xjs on 2020/9/23.
//

#include "../include/tctl_heap.h"
#include "include/_tctl_metaclass.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#define Import ITERATOR
//private
static void adjust_heap(const Iterator first, long long holo_index, long long len, const FormWO_t x,  Compare cmp)
{
    assert(first->rank >= RandomAccessIter);
    long long top_index = holo_index;
    Form_t t = THIS(first).type();
    for (long long i = holo_index * 2 + 1; i < len; i = i * 2 + 1) {
        if (i + 1 < len &&
            cmp(FORM_WITH_OBJ(t, THIS(first).brackets(VA(i + 1))), FORM_WITH_OBJ(t, THIS(first).brackets(VA(i)))) > 0)
            i++;
        if (cmp(x, FORM_WITH_OBJ(t, THIS(first).brackets(VA(i)))) >= 0)
            break;
        if (t.f == POD)
            memcpy(THIS(first).brackets(VA(top_index)), THIS(first).brackets(VA(i)), t.size);
        else if (t.f == OBJ) {
            Object obj = THIS(first).brackets(VA(top_index));
            THIS(obj).assign(FORM_WITH_OBJ(t, THIS(first).brackets(VA(i))));
        }
        top_index = i;
    }
    if (t.f == POD)
        memcpy(THIS(first).brackets(VA(top_index)), x.mem, t.size);
    else if (t.f == OBJ) {
        Object obj = THIS(first).brackets(VA(top_index));
        THIS(obj).assign(x);
    }
}

//public
void make_heap(const Iterator _first, const Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    Form_t t = THIS(first).type();
    size_t memb_size = t.f == POD ? t.size : classSz(t.class);
    void *x = malloc(memb_size);
    if (t.f == OBJ)
        construct(t, x, VAEND);
    long long dist = distance(first, last);
    for (int i = dist / 2 - 1; i>= 0; i--) {
        if (t.f == POD)
            memcpy(x, THIS(first).brackets(VA(i)), memb_size);
        else {
            Object obj = x;
            THIS(obj).assign(FORM_WITH_OBJ(t, THIS(first).brackets(VA(i))));
        }
        adjust_heap(first, i, dist, FORM_WITH_OBJ(t, x), cmp);
    }
    if (t.f == OBJ)
        destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void push_heap(const Iterator _first, const Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    Form_t t = THIS(first).type();
    size_t memb_size = t.f == POD ? t.size : classSz(t.class);
    void *x = malloc(memb_size);
    if (t.f == OBJ)
        construct(t, x, VAEND);
    long long cur_index = distance(first, last) - 1;
    long long father = cur_index / 2 - (cur_index + 1) % 2;
    if (t.f == POD)
        memcpy(x, THIS(first).brackets(VA(cur_index)), memb_size);
    else {
        Object obj = x;
        THIS(obj).assign(FORM_WITH_OBJ(t, THIS(first).brackets(VA(cur_index))));
    }
    while (father >= 0 &&
           cmp(FORM_WITH_OBJ(t, x), FORM_WITH_OBJ(t, THIS(first).brackets(VA(father)))) >= 0)
    {
        if (t.f == POD)
            memcpy(THIS(first).brackets(VA(cur_index)), THIS(first).brackets(VA(father)), memb_size);
        else {
            Object obj = THIS(first).brackets(VA(cur_index));
            THIS(obj).assign(FORM_WITH_OBJ(t, THIS(first).brackets(VA(father))));
        }
        cur_index = father;
        father = cur_index / 2 - (cur_index + 1) % 2;
    }
    if (cur_index != distance(first, last) - 1) {
        if (t.f == POD)
            memcpy(THIS(first).brackets(VA(cur_index)), x, memb_size);
        else {
            Object obj = THIS(first).brackets(VA(cur_index));
            THIS(obj).assign(FORM_WITH_OBJ(t, x));
        }
    }
    if (t.f == OBJ)
        destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void pop_heap(const Iterator _first, const Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    THIS(last).dec();

    Form_t t = THIS(first).type();
    size_t memb_size = t.f == POD ? t.size : classSz(t.class);
    void *x = malloc(memb_size);
    if (t.f == OBJ)
        construct(t, x, VAEND);
    if (t.f == POD)
        memcpy(x, THIS(last).derefer(), memb_size);
    else {
        Object obj = x;
        THIS(obj).assign(FORM_WITH_OBJ(t, THIS(last).derefer()));
    }
    if (t.f == POD)
        memcpy(THIS(last).derefer(), THIS(first).derefer(), memb_size);
    else {
        Object obj = THIS(last).derefer();
        THIS(obj).assign(FORM_WITH_OBJ(t, THIS(first).derefer()));
    }
    long long dist = distance(first, last);
    adjust_heap(first, 0, dist, FORM_WITH_OBJ(t, x), cmp);
    if (t.f == OBJ)
        destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void sort_heap(const Iterator _first, const Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    make_heap(first, last, cmp);
    while (!THIS(last).equal(VA(first)))
    {
        pop_heap(first, last, cmp);
        THIS(last).dec();
    }
    delete(first);
    delete(last);
}

