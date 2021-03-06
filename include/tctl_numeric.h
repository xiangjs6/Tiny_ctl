//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_NUMERIC_H
#define TINY_CTL_TCTL_NUMERIC_H

#include "tctl_iterator.h"
#include "tctl_def.h"
#include "tctl_metaclass.h"
FormWO_t accumulate(const Iterator first, const Iterator last, FormWO_t init, .../*Add*/);
Iterator adjacent_difference(const Iterator first, const Iterator last, const Iterator result, .../*Sub*/);
FormWO_t inner_product(const Iterator _first1, const Iterator _last1, const Iterator _first2, FormWO_t init, .../*Add, Mul*/);
Iterator partial_sum(const Iterator _first, const Iterator _last, const Iterator _result, .../*Add*/);
void iota(const Iterator _first, const Iterator _last, FormWO_t x, .../*Add*/);
#endif //TINY_CTL_TCTL_NUMERIC_H
