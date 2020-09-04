//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_def.h"
#include "iterator.h"

typedef struct {
    size_t nmemb;
    size_t memb_size;
    void *start;
    void *finish;
    void *end_of_storage;
    __iterator_obj_func *iter_func;
} __private_vector;

typedef struct {
    void *(*at)(int);
    void *(*begin)(void);
    void *(*end)(void);
    void *(*front)(void);
    void *(*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(void *x);
    void (*pop_back)(void);
    void *(*erase)(void *iter);
    void (*resize)(size_t new_size, void *x);
    void (*clear)(void);
    struct {
        __GET_ITER_FUNC;
    } OBJECT_INNER;
    byte OBJECT_PRIVATE[sizeof(__private_vector)];
} vector;
#endif //TINY_CTL_TCTL_VECTOR_H
