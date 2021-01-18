//
// Created by xjs on 2021/1/3.
//

#ifndef TINY_CTL_TCTL_CHAR_H
#define TINY_CTL_TCTL_CHAR_H
#include "tctl_common.h"
#include "tctl_class.h"

#define INHERIT_CHAR \
struct {            \
    INHERIT_CLASS;  \
}

//调整到一个字节对齐
typedef struct {
    union {
        INHERIT_CHAR *_s;
        char _pad[sizeof(*(Object)NULL)];
    };
    char val;
} BYTE_ALIGNED *Char ;

void initChar(void) __attribute__((constructor));
char toChar(FormWO_t t);
Form_t _Char(void);
#define CHAR Char : _Char()
#endif //TINY_CTL_TCTL_CHAR_H
