//
// Created by xjs on 2020/11/20.
//

#ifndef TINY_CTL_TCTL_POINTER_ITERATOR_H
#define TINY_CTL_TCTL_POINTER_ITERATOR_H
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
Iterator _oriPointIter_aux(void *class, void *p, size_t x);
#define oriPointIter(x, ...) _oriPointIter_aux(T(*(x)), x, ##__VA_ARGS__, 0)
#endif //TINY_CTL_TCTL_POINTER_ITERATOR_H
