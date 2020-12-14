//
// Created by xjs on 2020/11/20.
//

#ifndef TINY_CTL_TCTL_POINT_ITERATOR_H
#define TINY_CTL_TCTL_POINT_ITERATOR_H
#include "tctl_iterator.h"
#define POINT_ITER(A) __construct_array_iter(A, sizeof(*A))
IterType __construct_point_iter(void *obj_this, size_t memb_size);
#endif //TINY_CTL_TCTL_POINT_ITERATOR_H
