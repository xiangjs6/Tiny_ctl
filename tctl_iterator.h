//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_TCTL_ITERATOR_H
#define TINY_CTL_TCTL_ITERATOR_H
#include "tctl_object.h"

/*
 * 迭代器规则
 * 1、用户应该使用iterator定义迭代器变量，并使用NEW_ITER创建迭代器
 * 2、使用迭代器的对象应该在结构体中加入__iterator_obj_func成员
 * 3、容器的begin和end应该返回__iterator类型，NEW_ITER传入__iterator类型
 * 4、容器要使用struct __inner_iterator作为迭代器的壳，并定义容器自己的迭代器，放在struct __inner_iterator后面，并且第一个成员为指向值的地址
 * */
typedef struct __iterator __iterator;
typedef struct {
    void *(*iter_at)(__iterator*, int);
    void (*iter_increment)(__iterator*);
    void (*iter_decrement)(__iterator*);
    void (*iter_add)(__iterator*, int);
    void (*iter_sub)(__iterator*, int);
    long long (*iter_diff)(__iterator*, __iterator*);
} __iterator_obj_func;

typedef struct {
    void *(*at)(int);
    void *(*increment)(void);
    void *(*decrement)(void);
    void *(*front_increment)(void);
    void *(*front_decrement)(void);
    void (*add)(int);
    void (*sub)(int);
    void (*copy)(__iterator*);
    long long (*diff)(__iterator*);
} __public_iterator_func;

typedef struct {
    const __public_iterator_func *iter_func;
    const __iterator_obj_func  *private_iter_func;
} iterator_func;

struct __inner_iterator {
    const iterator_func *iterator_func_p;
    void *obj_this;
    size_t obj_iter_size;
    size_t memb_size;
    byte __address[0];
};

struct __iterator {
    struct __inner_iterator __inner;
    void *val;
};

#define iterator autofree(__destructor_iter) struct __iterator
#define ITER(p) (*THIS(((__iterator*)p)).__inner.iterator_func_p->iter_func)
#define NEW_ITER(p) (void*)(__constructor_iter((__iterator*)p))

extern const __public_iterator_func def_pub_iter_func;
#define INIT_ITER_FUNC(private_iter_func) {&def_pub_iter_func, private_iter_func}

struct __inner_iterator __creat_iter(size_t obj_iter_size, void *obj_this, size_t memb_size, const iterator_func *iter_func);
__iterator *__constructor_iter(__iterator *iter);
void __destructor_iter(void *p);
#endif //TINY_CTL_TCTL_ITERATOR_H
