//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_ALGO_H
#define TINY_CTL_TCTL_ALGO_H
#include "tctl_iterator.h"
Iterator set_union(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator adjacent_find(Iterator _first, Iterator _last, .../*Compare*/);
#endif //TINY_CTL_TCTL_ALGO_H
