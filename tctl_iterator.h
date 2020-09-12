//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_object.h"

/*
 * 迭代器规则
 * 1、对用户所见的都应该是type**类型
 * 2、使用迭代器的对象应该在结构体中加入__iterator_obj_func成员
 * 3、容器应该存储自己的begin和end迭代器，并且返回迭代器的函数应该为iter_ptr
 * 4、iter_ptr应该指向容器中的节点的元素的地址，并且！！！可以由这个地址找到这个元素所在的整个节点的地址
 * */
typedef struct {
    void *(*iter_increment)(void *);
    void *(*iter_decrement)(void *);
    void *(*iter_add)(void *, int);
    void *(*iter_sub)(void *, int);
} __iterator_obj_func;

typedef struct {
    void *obj_this;
    const size_t memb_size;
    __iterator_obj_func *obj_iter_func;
} __private_iterator;

typedef struct {
    void *ptr;
    void *(*increment)(void);
    void *(*decrement)(void);
    void *(*front_increment)(void);
    void *(*front_decrement)(void);
    void *(*add)(int);void *(*sub)(int);
    byte __obj_private[sizeof(__private_iterator)];
} iterator;

typedef void* iter_ptr;
#define ITER(p) THIS(container_of((p), iterator, ptr))
#define ITER_TYPE(type) autofree(__destructor_iter) type**
#define NEW_ITER(p) (void*)(&(__constructor_iter((iterator*)p))->ptr)

iterator init_iter(void *obj_ptr, void *p, size_t memb_size, __iterator_obj_func *func);
iterator *__constructor_iter(iterator *iter);
void __destructor_iter(void *p);
#endif //TINY_CTL_TCTL_ITERATOR_H
