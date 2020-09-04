//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_ITERATOR_H
#define TINY_CTL_ITERATOR_H
#include "tctl_object.h"

typedef struct {
    void *(*iter_increment)(void *);
    void *(*iter_decrement)(void *);
    void *(*iter_add)(void *, int);
    void *(*iter_sub)(void *, int);
} __iterator_obj_func;

typedef struct {
    void *obj_this;
    __iterator_obj_func *obj_iter_func;
} __private_iterator;

typedef struct {
    void *ptr;
    void *(*increment)(void);
    void *(*decrement)(void);
    void *(*front_increment)(void);
    void *(*front_decrement)(void);
    void *(*add)(int);void *(*sub)(int);
    byte __private_fill[sizeof(__private_iterator)];
} iterator;

#define __DEF_ITER_FUNC_NAME iter_func
#define DEF_ITER_FUNC_NAME *__DEF_ITER_FUNC_NAME
#define ITER(p) THIS(container_of((p), iterator, ptr))
#define ITER_TYPE(type) autofree(__destructor_iter) type**
#define ITER_VALUE(p) *p
#define NEW_ITER(obj, p) (&(__constructor_iter(init_iter(obj, p, (obj)->__DEF_ITER_FUNC_NAME)))->ptr);

iterator init_iter(void *obj_ptr, void *p, __iterator_obj_func *func);
iterator *__constructor_iter(iterator iter);
void __destructor_iter(void *p);
#endif //TINY_CTL_ITERATOR_H
