//
// Created by xjs on 2020/12/29.
//

#ifndef TINY_CTL__TCTL_METACLASS_H
#define TINY_CTL__TCTL_METACLASS_H
#include "../../include/tctl_metaclass.h"

void *super_ctor(const void *class, void *this, va_list *app);
void *super_dtor(const void *class, void *this);
//因为继承的时候想要隐藏父类的函数，所以不使用宏
struct MetaClassSelector {
    void *(*ctor)(void *mem, ...);
    void *(*dtor)(void);
    int (*differ)(const void *b);
    int (*puto)(FILE *fp);
    void *(*cast)(const void *class);
};
extern const struct MetaClassSelector *_MetaClassS;
extern const void *Selector;
#endif //TINY_CTL__TCTL_METACLASS_H
