//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_common.h"

typedef void *__vector_iter;


typedef struct {
    const size_t memb_size;
    size_t nmemb;
    size_t total_storage_memb;
    struct {
        struct __inner_iterator start_iter;
        __vector_iter start_ptr;
    };
    struct {
        struct __inner_iterator finish_iter;
        __vector_iter finish_ptr;
    };
} __private_vector;

typedef struct {
    void *(*at)(int);
    const __iterator *(*begin)(void);
    const __iterator *(*end)(void);
    void const *(*front)(void);
    void const *(*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(void *x);
    void (*pop_back)(void);
    __iterator *(*erase)(__iterator *iter);
    __iterator *(*insert)(__iterator *iter, void *x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_vector)];
} vector;

void init_vector(vector *p_vector, size_t nmemb, size_t memb_size, void *init_array);
void destory_vector(vector *p_vector);
vector creat_vector(size_t nmemb, size_t memb_size, void *init_array);
#endif //TINY_CTL_TCTL_VECTOR_H
