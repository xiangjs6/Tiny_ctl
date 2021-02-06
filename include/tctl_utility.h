//
// Created by xjs on 2021/2/6.
//

#ifndef TINY_CTL_TCTL_UTILITY_H
#define TINY_CTL_TCTL_UTILITY_H
#include "tctl_metaclass.h"

//Pair
#define PAIR_OBJ struct { \
    Form_t f_t;           \
    void *first;          \
    Form_t s_t;           \
    void *second;         \
}
typedef struct _Pair *Pair;
struct _Pair {
    METAOBJECT_HEAD(METACLASS_FUNC);
    PAIR_OBJ;
};
void initPair(void) __attribute__((constructor));
Form_t _Pair(void);
Pair tmpPair(Form_t key_t, Form_t val_t, ...);
#define PAIR Pair : _Pair()


#endif //TINY_CTL_TCTL_UTILITY_H
