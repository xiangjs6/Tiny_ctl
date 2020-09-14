//
// Created by xjs on 2020/9/12.
//
#include "tctl_allocator.h"
#include "tctl_deque.h"
#include <memory.h>

static void extend_map(__private_deque *p_private)
{
    size_t old_len = p_private->mmap_len;
    p_private->mmap_len = p_private->mmap_len ? 2 * p_private->mmap_len : 1;
    p_private->mmap = reallocate(p_private->mmap, old_len, p_private->mmap_len);
    memmove(p_private->mmap + sizeof(void*) * old_len / 2, p_private->mmap, old_len * sizeof(void*));
    __deque_iter *start_iter = p_private->start.ptr;
    __deque_iter *finish_iter = p_private->finish.ptr;
    start_iter->map_node = p_private->mmap + sizeof(void*) * old_len / 2;
    finish_iter->map_node = start_iter->map_node + sizeof(void*) * old_len;
}
iterator begin(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->start;
}
iterator end(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->finish;
}
size_t size(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->nmemb;
}
bool empty(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->nmemb;
}
void *at(int pos)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (pos >= p_private->nmemb)
        return NULL;
    __deque_iter *start_iter = p_private->start.ptr;
    //__deque_iter *finish_iter = p_private->finish.ptr;
    size_t start_len = start_iter->last - start_iter->cur;
    //size_t finish_len = finish_iter->cur - finish_iter->first;
    if (pos < start_len)
        return start_iter->cur + pos * p_private->memb_size;
    pos -= start_len;
    int block_index = pos % (int)p_private->block_nmemb;
    void **p_block = start_iter->map_node + block_index;
    pos %= p_private->block_nmemb;
    return *p_block + pos;
}
void const *front(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *start_iter = p_private->start.ptr;
    return start_iter->cur + p_private->memb_size;
}
void const *back(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *finish_iter = p_private->finish.ptr;
    return finish_iter->cur - p_private->memb_size;
}
void push_back(void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *finish_iter = p_private->finish.ptr;
    if (finish_iter->cur == finish_iter->last) {
        extend_map(p_private);
    }
    memcpy(finish_iter->cur, x, p_private->memb_size);
    finish_iter->cur += p_private->memb_size;
}
void (*push_front)(void *x);
void (*pop_back)(void);
void (*pop_front)(void);
void (*insert)(iter_ptr iter, void *x);
void (*erase)(iter_ptr iter);
void (*clear)(void);
