//
// Created by xjs on 2020/11/20.
//

#ifndef TINY_CTL_TCTL_POINT_ITERATOR_H
#define TINY_CTL_TCTL_POINT_ITERATOR_H
#include "tctl_iterator.h"
#define POINT_ITER(P, OBJ) __construct_point_iter(P, OBJ, sizeof(*(OBJ)))
IterType __construct_point_iter(void *p, void *obj_this, size_t memb_size);
#endif //TINY_CTL_TCTL_POINT_ITERATOR_H
