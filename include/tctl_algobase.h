//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_ALGOBASE_H
#define TINY_CTL_TCTL_ALGOBASE_H
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
#include "tctl_utility.h"
#include <stddef.h>
Iterator copy_backward(Iterator first, Iterator last, Iterator result);
Iterator copy(Iterator first, Iterator last, Iterator result);
bool equal(Iterator first1, Iterator last1, Iterator first2, .../*Compare*/);
void fill(Iterator first, Iterator last, const void *x);
void fill_n(Iterator first, const void *n, const void *x);
void iter_swap(Iterator a, Iterator b);
bool lexicographical_compare(Iterator first1, Iterator last1, Iterator first2, Iterator last2, .../*Compare*/);
void *max(const void *a, const void *b, .../*Compare*/);
void *min(const void *a, const void *b, .../*Compare*/);
Pair mismatch(Iterator first1, Iterator last1, Iterator first2, .../*Compare*/);
void swap(void *a, void *b);
#endif //TINY_CTL_TCTL_ALGOBASE_H
