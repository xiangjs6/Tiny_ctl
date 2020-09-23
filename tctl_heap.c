//
// Created by xjs on 2020/9/23.
//

#include "tctl_heap.h"
#include "tctl_common.h"

void make_heap(obj_iter first, obj_iter last, bool (*cmp_func)(const void *, const void *))
{
    ITER_TYPE(void) _first = NEW_ITER(first);
    ITER_TYPE(void) _last = NEW_ITER(last);
    ITER(_first).memb_size;
}