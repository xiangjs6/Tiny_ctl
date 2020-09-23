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

typedef void* obj_iter;
typedef struct {
    void *(*iter_at)(obj_iter, int);
    void (*iter_increment)(obj_iter);
    void (*iter_decrement)(obj_iter);
    void (*iter_add)(obj_iter, int);
    void (*iter_sub)(obj_iter, int);
    long long (*iter_diff)(obj_iter, obj_iter);
} __iterator_obj_func;

typedef struct {
    __iterator_obj_func *obj_iter_func;
    size_t obj_iter_size;
    byte obj_iter[0];
} __private_iterator;

typedef struct {
    obj_iter const iter_ptr;
    void *obj_this;
    const size_t memb_size;
    void *(*at)(int);
    void *(*increment)(void);
    void *(*decrement)(void);
    void *(*front_increment)(void);
    void *(*front_decrement)(void);
    void (*add)(int);
    void (*sub)(int);
    long long (*diff)(obj_iter);
    byte __obj_private[sizeof(__private_iterator)];
} iterator;

#define ITER(p) THIS(container_of(container_of(p, __private_iterator, obj_iter), iterator, __obj_private))
#define ITER_TYPE(type) autofree(__destructor_iter) type**
#define NEW_ITER(p) (void*)((__constructor_iter((iterator*)container_of(container_of(p, __private_iterator, obj_iter), iterator, __obj_private)))->iter_ptr)

void __init_iter(iterator *iter, void *obj_ptr, size_t obj_iter_size, size_t memb_size, __iterator_obj_func *func);
iterator *__constructor_iter(iterator *iter);
void __destructor_iter(void *p);
#endif //TINY_CTL_TCTL_ITERATOR_H
