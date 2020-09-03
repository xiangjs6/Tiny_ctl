//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_ITERATOR_H
#define TINY_CTL_ITERATOR_H

typedef struct {
    void *(*iter_increment)(void *);
    void *(*iter_decrement)(void *);
    void *(*iter_add)(void *, int);
    void *(*iter_sub)(void *, int);
} __iterator_obj_func;

typedef struct {
    struct {
        void *(*increment)(void);
        void *(*decrement)(void);
        void *(*front_increment)(void);
        void *(*front_decrement)(void);
        void *(*add)(int);
        void *(*sub)(int);
    } *Public_memb;
    void *obj_this;
    void *ptr;
} iterator;

#define ITER(p) THIS(container_of((p), iterator, ptr))
#define ITER_TYPE(type) type**
#define ITER_VALUE(p) *p
iterator creat_iter(void *p, void *obj_this);

#endif //TINY_CTL_ITERATOR_H
