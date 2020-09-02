//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stddef.h>

void **pthis(void);
#define THIS(p) (*((*(typeof(p))((*pthis()) = p)).Public_memb))
#define PRIVATE_THIS(p) ((*pthis()) = p)

#endif //TINY_CTL_TCTL_OBJECT_H
