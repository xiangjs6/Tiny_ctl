//
// Created by xjs on 2020/9/23.
//

#include "../include/tctl_heap.h"
#include "../include/tctl_arg.h"
#include <assert.h>
#include <stdlib.h>
#define Import ITERATOR
//private
static void adjust_heap(Iterator first, long long holo_index, long long len, const void *x,  Compare cmp)
{
    assert(first->rank >= RandomAccessIter);
    long long top_index = holo_index;
    for (long long i = holo_index * 2 + 1; i < len; i = i * 2 + 1) {
        if (i + 1 < len && cmp(THIS(first).brackets(VA(i + 1)), THIS(first).brackets(VA(i))) > 0)
            i++;
        if (cmp(x, THIS(first).brackets(VA(i))) >= 0)
            break;
        Object obj = THIS(first).brackets(VA(top_index));
        THIS(obj).assign(THIS(first).brackets(VA(i)));
        top_index = i;
    }
    Object obj = THIS(first).brackets(VA(top_index));
    THIS(obj).assign(x);
}

//public
void make_heap(Iterator _first, Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, _first, VAEND);
    Iterator last = THIS(_last).ctor(NULL, _last, VAEND);
    size_t memb_size = classSz(first->class);
    void *x = malloc(memb_size);
    construct(first->class, x, VAEND);
    long long dist = distance(first, last);
    for (long long i = dist / 2 - 1; i>= 0; i--) {
        Object obj = x;
        THIS(obj).assign(THIS(first).brackets(VA(i)));
        adjust_heap(first, i, dist, x, cmp);
    }
    destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void push_heap(Iterator _first, Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, _first, VAEND);
    Iterator last = THIS(_last).ctor(NULL, _last, VAEND);
    size_t memb_size = classSz(first->class);
    void *x = malloc(memb_size);
    construct(first->class, x, VAEND);
    long long cur_index = distance(first, last) - 1;
    long long father = cur_index / 2 - (cur_index + 1) % 2;
    THIS((Object)x).assign(THIS(first).brackets(VA(cur_index)));
    while (father >= 0 && cmp(x, THIS(first).brackets(VA(father))) >= 0)
    {
        Object obj = THIS(first).brackets(VA(cur_index));
        THIS(obj).assign(THIS(first).brackets(VA(father)));
        cur_index = father;
        father = cur_index / 2 - (cur_index + 1) % 2;
    }
    if (cur_index != distance(first, last) - 1) {
        Object obj = THIS(first).brackets(VA(cur_index));
        THIS(obj).assign( x);
    }
    destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void pop_heap(Iterator _first, Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, _first, VAEND);
    Iterator last = THIS(_last).ctor(NULL, _last, VAEND);
    THIS(last).dec();

    size_t memb_size = classSz(first->class);
    void *x = malloc(memb_size);
    construct(first->class, x, VAEND);
    THIS((Object)x).assign(THIS(last).derefer());
    Object obj = THIS(last).derefer();
    THIS(obj).assign(THIS(first).derefer());
    long long dist = distance(first, last);
    adjust_heap(first, 0, dist, x, cmp);
    destroy(x);
    free(x);
    delete(first);
    delete(last);
}

void sort_heap(Iterator _first, Iterator _last, Compare cmp)
{
    Iterator first = THIS(_first).ctor(NULL, _first, VAEND);
    Iterator last = THIS(_last).ctor(NULL, _last, VAEND);
    make_heap(first, last, cmp);
    while (!THIS(last).equal(first))
    {
        pop_heap(first, last, cmp);
        THIS(last).dec();
    }
    delete(first);
    delete(last);
}

