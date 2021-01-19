//
// Created by xjs on 2021/1/6.
//

#ifndef TINY_CTL__TCTL_VECTOR_H
#define TINY_CTL__TCTL_VECTOR_H
#include "../../include/tctl_vector.h"
#include "_tctl_class.h"
struct VectorSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    void* (*front)(void);
    void* (*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(FormWO_t x);
    void (*pop_back)(void);
    Iterator (*erase)(Iterator iter);
    Iterator (*insert)(Iterator iter, FormWO_t x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    void (*swap)(Vector _v);
};
Form_t _VectorClass(void);
#endif //TINY_CTL__TCTL_VECTOR_H
