//
// Created by xjs on 2020/9/23.
//

#ifndef TINY_CTL_TCTL_HEAP_H
#define TINY_CTL_TCTL_HEAP_H
#include "tctl_iterator.h"
void push_heap(const __iterator *first, const __iterator *last, Compare);
void pop_heap(const __iterator *first, const __iterator *last, Compare);
void sort_heap(const __iterator *first, const __iterator *last, Compare);
void make_heap(const __iterator *first, const __iterator *last, Compare);

#endif //TINY_CTL_TCTL_HEAP_H
