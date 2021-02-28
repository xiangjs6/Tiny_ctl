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
void fill(Iterator first, Iterator last, FormWO_t x, .../*Assign*/);
void fill_n(Iterator first, size_t n, FormWO_t x, .../*Assign*/);
void iter_swap(Iterator a, Iterator b, .../*Assign*/);
bool lexicographical_compare(Iterator first1, Iterator last1, Iterator first2, Iterator last2, .../*Compare*/);
FormWO_t max(FormWO_t a, FormWO_t b, .../*Compare*/);
FormWO_t min(FormWO_t a, FormWO_t b, .../*Compare*/);
Pair mismatch(Iterator first1, Iterator last1, Iterator first2, .../*Compare*/);
void swap(FormWO_t a, FormWO_t b, .../*Assign*/);
#endif //TINY_CTL_TCTL_ALGOBASE_H
