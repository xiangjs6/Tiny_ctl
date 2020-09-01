//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stddef.h>

void **pthis(void);
#define THIS(p) ((*(typeof(p))((*pthis()) = p)))

void *tmp_val(void *p, size_t n);
#define TEMP(v) ({typeof(v) __temp = v; (typeof(v)*)tmp_val(&__temp, sizeof(v));})
#endif //TINY_CTL_TCTL_OBJECT_H
