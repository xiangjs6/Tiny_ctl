//
// Created by xjs on 2020/9/23.
//

#include "tctl_heap.h"
#include "tctl_common.h"
#include <memory.h>
//private
void adjust_heap(const __iterator *first, long long holo_index, long long len, const void *x,  bool (*cmp_func)(const void *, const void *))
{
    long long top_index = holo_index;
    for ( long long i = holo_index * 2 + 1; i < len; i = i * 2 + 1) {
        if (i + 1 < len && cmp_func(ITER(first).at(i + 1), ITER(first).at(i)))
            i++;
        if (cmp_func(x, ITER(first).at(i)))
            break;
        memcpy(ITER(first).at(top_index), ITER(first).at(i), first->__inner.memb_size);
        top_index = i;
    }
    memcpy(ITER(first).at(top_index), x, first->__inner.memb_size);
}

//public
void make_heap(const __iterator *first, const __iterator *last, bool (*cmp_func)(const void *, const void *))
{
    iterator *_first = NEW_ITER(first);
    iterator *_last = NEW_ITER(last);
    byte x[_first->__inner.memb_size];
    long long dist = ITER(_last).diff(_first);
    for (int i = dist / 2 - 1; i>= 0; i--) {
        memcpy(x, ITER(_first).at(i), sizeof(x));
        adjust_heap(_first, i, dist, x, cmp_func);
    }
}

void push_heap(const __iterator *first, const __iterator *last, bool (*cmp_func)(const void *, const void *))
{
    iterator *_first = NEW_ITER(first);
    iterator *_last = NEW_ITER(last);
    byte x[_first->__inner.memb_size];
    long long cur_index = ITER(_last).diff(_first) - 1;
    long long father = cur_index / 2 - (cur_index + 1) % 2;
    memcpy(x, ITER(_first).at(cur_index), sizeof(x));
    while (father >= 0 && cmp_func(x, ITER(_first).at(father)))
    {
        memcpy(ITER(_first).at(cur_index), ITER(_first).at(father), _first->__inner.memb_size);
        cur_index = father;
        father = cur_index / 2 - (cur_index + 1) % 2;
    }
    if (cur_index != ITER(_last).diff(_first) - 1)
        memcpy(ITER(_first).at(cur_index), x, _first->__inner.memb_size);
}

void pop_heap(const __iterator *first, const __iterator *last, bool (*cmp_func)(const void *, const void *))
{
    iterator *_first = NEW_ITER(first);
    iterator *_last = NEW_ITER(last);
    byte x[_first->__inner.memb_size];
    ITER(_last).decrement();
    memcpy(x, _last->val, _last->__inner.memb_size);
    memcpy(_last->val, _first->val, _first->__inner.memb_size);
    long long dist = ITER(_last).diff(_first);
    adjust_heap(_first, 0, dist, x, cmp_func);
}

void sort_heap(const __iterator *first, const __iterator *last, bool (*cmp_func)(const void *, const void *))
{
    iterator *_first = NEW_ITER(first);
    iterator *_last = NEW_ITER(last);
    make_heap(_first, _last, cmp_func);
    while (_last->val != _first->val)
    {
        pop_heap(_first, _last, cmp_func);
        ITER(_last).decrement();
    }
}
