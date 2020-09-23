//
// Created by xjs on 2020/9/23.
//

#ifndef TINY_CTL_TCTL_HEAP_H
#define TINY_CTL_TCTL_HEAP_H
#include "tctl_iterator.h"
void push_heap(obj_iter first, obj_iter last, bool (*cmp_func)(const void *, const void *));
void pop_heap(obj_iter first, obj_iter last, bool (*cmp_func)(const void *, const void *));
void sort_heap(obj_iter first, obj_iter last, bool (*cmp_func)(const void *, const void *));
void make_heap(obj_iter first, obj_iter last, bool (*cmp_func)(const void *, const void *));

#endif //TINY_CTL_TCTL_HEAP_H
