//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_def.h"
#include "iterator.h"

typedef struct {
    const size_t memb_size;
    size_t nmemb;
    size_t total_storage_memb;
    iterator start;
    iterator finish;
    //iterator end_of_storage;
} __private_vector;

typedef struct {
    void *(*at)(int);
    iter_ptr const *(*begin)(void);
    iter_ptr const *(*end)(void);
    void *(*front)(void);
    void *(*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(void *x);
    void (*pop_back)(void);
    iter_ptr (*erase)(iter_ptr iter);
    iter_ptr (*insert)(iter_ptr iter, void *x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    byte OBJECT_PRIVATE[sizeof(__private_vector)];
} vector;

void init_vector(vector *p_vector, size_t nmemb, size_t memb_size, void *init_array);
void destory_vector(vector *p_vector);
#endif //TINY_CTL_TCTL_VECTOR_H
