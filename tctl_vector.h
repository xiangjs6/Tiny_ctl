//
// Created by xjs on 2020/9/4.
//

#ifndef TINY_CTL_TCTL_VECTOR_H
#define TINY_CTL_TCTL_VECTOR_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_common.h"

typedef void* vector_iter;

typedef struct {
    const size_t memb_size;
    size_t nmemb;
    size_t total_storage_memb;
    struct {
        iterator const start;
        vector_iter start_iter;
    } BYTE_ALIGNED;
    struct {
        iterator const finish;
        vector_iter finish_iter;
    } BYTE_ALIGNED;
    //iterator end_of_storage;
} __private_vector;

typedef struct {
    void *(*at)(int);
    void const * const * (*begin)(void);
    void const * const * (*end)(void);
    void *(*front)(void);
    void *(*back)(void);
    size_t (*size)(void);
    size_t (*capacity)(void);
    bool (*empty)(void);
    void (*push_back)(void *x);
    void (*pop_back)(void);
    vector_iter *(*erase)(vector_iter *iter);
    vector_iter *(*insert)(vector_iter *iter, void *x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    byte __obj_private[sizeof(__private_vector)];
} vector;

vector creat_vector(size_t nmemb, size_t memb_size, void *init_array);
void init_vector(vector *p_vector, size_t nmemb, size_t memb_size, void *init_array);
void destory_vector(vector *p_vector);
#endif //TINY_CTL_TCTL_VECTOR_H
