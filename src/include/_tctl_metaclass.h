//
// Created by xjs on 2020/12/29.
//

#ifndef TINY_CTL__TCTL_METACLASS_H
#define TINY_CTL__TCTL_METACLASS_H
#include "../../include/tctl_metaclass.h"
typedef void (*voidf)();	/* generic function pointer */

void *super_ctor(const void *class, void *this, va_list *app);
void *super_dtor(const void *class, void *this);
int super_differ(const void *class, const void *this, const void *b);
int super_puto(const void *class, const void *this, FILE *fp);
struct MetaClassSelector {
    void *(*ctor)(void *mem, ...);
    void *(*dtor)(void);
    int (*differ)(const void *b);
    int (*puto)(FILE *fp);
};
extern const struct MetaClassSelector *_MetaClassS;
extern const void *Selector;
#endif //TINY_CTL__TCTL_METACLASS_H
