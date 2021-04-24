//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_CHAR_H
#define TINY_CTL_TCTL_CHAR_H
#include "tctl_common.h"
#include "tctl_class.h"
#include "tctl_metaclass.h"

#define CHAR_FUNC \
struct {          \
    CLASS_FUNC;   \
}

#define CHAR_OBJ \
struct {         \
    char val;    \
}

//调整到一个字节对齐
typedef struct {
    METAOBJECT_HEAD(CHAR_FUNC);
    CHAR_OBJ;
} BYTE_ALIGNED *Char ;

const void *_Char(void);
#define CHAR Char : _Char()
#endif //TINY_CTL_TCTL_CHAR_H
