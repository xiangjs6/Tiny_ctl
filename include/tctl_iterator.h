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
typedef void *IterType;
typedef struct {
    void *(*iter_at)(__iterator*, int);
    void (*iter_inc)(__iterator*);
    void (*iter_dec)(__iterator*);
    void (*iter_add)(__iterator*, int);
    void (*iter_sub)(__iterator*, int);
    long long (*iter_dist)(const __iterator*, const __iterator*);
    bool (*iter_equal)(const __iterator*, const __iterator*);
} __iterator_obj_func;

typedef struct {
    void *(*at)(int);
    void (*inc)(void);
    void (*dec)(void);
    void (*self_add)(int);
    void (*self_sub)(int);
    IterType (*add)(int);
    IterType (*sub)(int);
    void (*copy)(const IterType);
    long long (*dist)(const IterType);
    bool (*equal)(const IterType);
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

//#define iterator autofree(__destructor_iter) struct __iterator * const
#define iterator(T) struct {const struct __inner_iterator __inner; T *val;} * const

#define ITER(p) (*(**(__iterator**)push_this((void*)&p)).__inner.iterator_func_p->iter_func)
//#define ITER(p) (*THIS(((__iterator**)&p)).__inner.iterator_func_p->iter_func)
//#define NEW_ITER(p) (void*)(__constructor_iter((__iterator*)p))

extern const __public_iterator_func def_pub_iter_func;
extern const __iterator def_init_iter;
#define INIT_ITER_FUNC(private_iter_func) {&def_pub_iter_func, private_iter_func}
#define INIT_ITERATOR (IterType)&def_init_iter;

void init_iter(struct __inner_iterator *iter, size_t obj_iter_size, void *obj_this, size_t memb_size, const iterator_func *iter_func);
__iterator *copy_iter(__iterator const *iter);
IterType retain_iter(IterType iter);
void free_iter(__iterator *iter);
#endif //TINY_CTL_TCTL_ITERATOR_H
