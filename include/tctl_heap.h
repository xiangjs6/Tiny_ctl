//
// Created by xjs on 2020/9/23.
//

#ifndef TINY_CTL_TCTL_HEAP_H
#define TINY_CTL_TCTL_HEAP_H
#include "tctl_iterator.h"
#include "tctl_def.h"
void push_heap(Iterator first, Iterator last, Compare);
void pop_heap(Iterator first, Iterator last, Compare);
void sort_heap(Iterator first, Iterator last, Compare);
void make_heap(Iterator first, Iterator last, Compare);

#endif //TINY_CTL_TCTL_HEAP_H
