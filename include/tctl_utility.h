//
// Created by xjs on 2021/2/6.
//

#ifndef TINY_CTL_TCTL_UTILITY_H
#define TINY_CTL_TCTL_UTILITY_H
#include "tctl_metaclass.h"

//Pair
#define PAIR_OBJ struct { \
    const void *f_t;      \
    void *first;          \
    const void *s_t;      \
    void *second;         \
}
typedef struct _Pair *Pair;
struct _Pair {
    METAOBJECT_HEAD(METACLASS_FUNC);
    PAIR_OBJ;
};

const void *_Pair(void);
Pair tmpPair(const void *first_t, const void *second_t, ...);
#define PAIR Pair : _Pair()

#endif //TINY_CTL_TCTL_UTILITY_H
