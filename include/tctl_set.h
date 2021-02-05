//
// Created by xjs on 2020/11/16.
//

#ifndef TINY_CTL_TCTL_SET_H
#define TINY_CTL_TCTL_SET_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"

#define SET_FUNC                    \
struct {                            \
    CLASS_FUNC;                     \
    Iterator (*begin)(void);        \
    Iterator (*end)(void);          \
    size_t (*size)(void);           \
    bool (*empty)(void);            \
    void (*erase)(Iterator iter);   \
    Iterator (*insert)(FormWO_t x); \
    size_t (*count)(FormWO_t x);    \
    Iterator (*find)(FormWO_t x);   \
    void (*clear)(void);            \
    void (*swap)(Set);              \
}

typedef struct _Set *Set;
struct _Set {
    METAOBJECT_HEAD(SET_FUNC);
};

void initSet(void) __attribute__((constructor));
Form_t _Set(void);
#define SET Set : _Set()
#endif //TINY_CTL_TCTL_SET_H
