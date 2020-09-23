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
typedef struct __iterator __iterator;
typedef void* obj_iter;
typedef struct {
    void *(*iter_at)(__iterator*, int);
    void (*iter_increment)(__iterator*);
    void (*iter_decrement)(__iterator*);
    void (*iter_add)(__iterator*, int);
    void (*iter_sub)(__iterator*, int);
    long long (*iter_diff)(__iterator*, struct __iterator*);
} __iterator_obj_func;

typedef struct {
    __iterator_obj_func *obj_iter_func;
} __private_iterator_func;

typedef struct {
    void *(*at)(int);
    void *(*increment)(void);
    void *(*decrement)(void);
    void *(*front_increment)(void);
    void *(*front_decrement)(void);
    void (*add)(int);
    void (*sub)(int);
    void (*copy)(__iterator*);
    long long (*diff)(__iterator *);
    byte __obj_private[sizeof(__private_iterator_func)];
} iterator_func;

struct __iterator {
    void *val;
    iterator_func const *iterator_func_p;
    size_t obj_iter_size;
    size_t memb_size;
};

#define iterator autofree(__destructor_iter) __iterator *

#define ITER(p) (*THIS(((__iterator*)p)).iterator_func_p)
#define ITER_TYPE(type) autofree(__destructor_iter) type**
#define NEW_ITER(p) (void*)(__constructor_iter((__iterator*)p))

__iterator __creat_iter(size_t obj_iter_size, size_t memb_size, iterator_func const *iterator_func_p);
void __init_iter_func(iterator_func *iter_f, __iterator_obj_func *func);
__iterator *__constructor_iter(__iterator *iter);
void __destructor_iter(void *p);
void __iter_copy(__iterator*, __iterator*);
#endif //TINY_CTL_TCTL_ITERATOR_H
