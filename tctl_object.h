//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stddef.h>

void **pthis(void);
#define THIS(p) (*((*(typeof(p))((*pthis()) = p)).obj_func))
#define PRIVATE_THIS(p) ((*(Obj_private_func*)((*pthis()) = p)))

#endif //TINY_CTL_TCTL_OBJECT_H
