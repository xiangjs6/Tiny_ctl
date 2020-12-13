//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_common.h"
#include "tctl_portable.h"

typedef void *__vector_iter;


typedef struct {
    const size_t memb_size;
    size_t nmemb;
    size_t total_storage_memb;
    __vector_iter start_ptr;
    __vector_iter finish_ptr;
} __private_vector;

typedef struct {
    void *(*at)(int);
    IterType (*begin)(void);
    IterType (*end)(void);
    void const *(*front)(void);
    void const *(*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(void *x);
    void (*pop_back)(void);
    IterType (*erase)(IterType iter);
    IterType (*insert)(IterType iter, void *x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_vector)];
} vector;

void init_vector(vector *p_vector, size_t memb_size);
void destory_vector(vector *p_vector);
vector creat_vector(size_t memb_size);
#endif //TINY_CTL_TCTL_VECTOR_H
