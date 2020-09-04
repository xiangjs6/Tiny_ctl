//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_ITERATOR_H
#define TINY_CTL_ITERATOR_H
#include "tctl_object.h"

/*
 * 迭代器规则
 * 1、对用户所见的都应该是type**类型
 * 2、使用迭代器的对象应该在结构体中加入__iterator_obj_func成员，并且使用宏__DEF_ITER_FUNC_NAME为名字
 * 3、声明得到__iterator_obj_func成员地址的函数，并且放在public域中
 * */
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
    byte OBJECT_PRIVATE[sizeof(__private_iterator)];
} iterator;

#define __GET_ITER_FUNC_NAME __get_iter_func
#define __GET_ITER_FUNC __iterator_obj_func *(*__GET_ITER_FUNC_NAME)(void)

#define ITER(p) THIS(container_of((p), iterator, ptr))
#define ITER_TYPE(type) autofree(__destructor_iter) type**
#define ITER_VALUE(p) *p
#define NEW_ITER(obj, p) (&(__constructor_iter(init_iter(obj, p, (THIS(obj)).OBJECT_INNER.__GET_ITER_FUNC_NAME())))->ptr);

iterator init_iter(void *obj_ptr, void *p, __iterator_obj_func *func);
iterator *__constructor_iter(iterator iter);
void __destructor_iter(void *p);
#endif //TINY_CTL_ITERATOR_H
