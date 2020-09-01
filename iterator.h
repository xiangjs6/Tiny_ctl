//
// Created by xjs on 2020/9/1.
//

#ifndef TINY_CTL_ITERATOR_H
#define TINY_CTL_ITERATOR_H

typedef struct {
    struct {
        void *(*increment)(void);
        void *(*decrement)(void);
    } *obj_func;
    void *obj_this;
    void *ptr;
} iter;

iter creat_iter(void *p);

#endif //TINY_CTL_ITERATOR_H
