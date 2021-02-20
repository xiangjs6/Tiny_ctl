//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_ALGO_H
#define TINY_CTL_TCTL_ALGO_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
Iterator set_union(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator adjacent_find(Iterator _first, Iterator _last, .../*Compare*/);
size_t count(Iterator _first, Iterator _last, FormWO_t val, .../*Compare*/);
size_t count_if(Iterator _first, Iterator _last, Predicate pred);
Iterator find(Iterator _first, Iterator _last, FormWO_t val, ...);
Iterator find_if(Iterator _first, Iterator _last, Predicate pred);
#endif //TINY_CTL_TCTL_ALGO_H
