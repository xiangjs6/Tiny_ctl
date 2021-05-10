//
// Created by xjs on 2020/11/20.
//

#ifndef TINY_CTL_TCTL_POINTER_ITERATOR_H
#define TINY_CTL_TCTL_POINTER_ITERATOR_H
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
Iterator _oriPointerIter_aux(const void *class, void *p, size_t x, ...);
#define oriPointerIter(x, ...) _oriPointerIter_aux(_Generic(*(x), Import), x, ##__VA_ARGS__, 0)
#endif //TINY_CTL_TCTL_POINTER_ITERATOR_H
