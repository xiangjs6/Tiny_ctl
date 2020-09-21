//
// Created by xjs on 2020/9/12.
//
#include "tctl_allocator.h"
#include "tctl_deque.h"
#include <memory.h>

static void extend_map(__private_deque *p_private)
{
    size_t old_len = p_private->mmap_len;
    size_t finish_off = p_private->finish_iter.map_node - p_private->start_iter.map_node;
    p_private->mmap_len = 2 * p_private->mmap_len;
    p_private->mmap = reallocate(p_private->mmap, old_len * sizeof(void*), p_private->mmap_len * sizeof(void*));
    memmove(p_private->mmap + old_len / 2, p_private->mmap, old_len * sizeof(void*));

    p_private->start_iter.map_node = p_private->mmap + old_len / 2;
    p_private->start_iter.cur = *p_private->start_iter.map_node + (p_private->start_iter.cur - p_private->start_iter.first);
    p_private->start_iter.first = *p_private->start_iter.map_node;
    p_private->start_iter.last = *p_private->start_iter.map_node + p_private->memb_size * p_private->block_nmemb;

    p_private->finish_iter.map_node = p_private->start_iter.map_node + finish_off;
    p_private->finish_iter.cur = *p_private->finish_iter.map_node + (p_private->finish_iter.cur - p_private->finish_iter.first);
    p_private->finish_iter.first = *p_private->finish_iter.map_node;
    p_private->finish_iter.last = *p_private->finish_iter.map_node + p_private->memb_size * p_private->block_nmemb;
}
static void const * const * begin(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return (void*)&p_private->start_iter;
}
static void const * const * end(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return (void*)&p_private->finish_iter;
}
static size_t size(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->nmemb;
}

static bool empty(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->nmemb;
}
static void *at(int pos)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (pos >= p_private->nmemb || pos < 0)
        return NULL;
    deque_iter *start_iter = &p_private->start_iter;
    //__deque_iter *finish_iter = p_private->finish.ptr;
    size_t start_len = (start_iter->last - start_iter->cur) / p_private->memb_size;
    //size_t finish_len = finish_iter->cur - finish_iter->first;
    if (pos < start_len)
        return start_iter->cur + pos * p_private->memb_size;
    pos -= start_len;
    int block_index = pos / (int)p_private->block_nmemb + 1;
    void **p_block = start_iter->map_node + block_index;
    pos %= p_private->block_nmemb;
    return *p_block + pos * p_private->memb_size;
}
static void const *front(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->start_iter.cur;
}
static void const *back(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *finish_iter = p_private->finish.iter_ptr;
    return finish_iter->cur - p_private->memb_size;
}
static void push_back(void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *finish_iter = p_private->finish.iter_ptr;
    if (finish_iter->cur == finish_iter->last) {
        if (finish_iter->map_node == p_private->mmap + p_private->mmap_len - 1)
            extend_map(p_private);
        finish_iter->map_node++;
        *finish_iter->map_node = allocate(p_private->block_nmemb * p_private->memb_size);
        finish_iter->cur = finish_iter->first = *finish_iter->map_node;
        finish_iter->last = *finish_iter->map_node + p_private->memb_size * p_private->block_nmemb;
    }
    memcpy(finish_iter->cur, x, p_private->memb_size);
    finish_iter->cur += p_private->memb_size;
    p_private->nmemb++;
}
static void push_front(void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *start_iter = &p_private->start_iter;
    if (start_iter->cur == start_iter->first) {
        if (start_iter->map_node == p_private->mmap)
            extend_map(p_private);
        start_iter->map_node--;
        *start_iter->map_node = allocate(p_private->block_nmemb * p_private->memb_size);
        start_iter->cur = start_iter->last = *start_iter->map_node + p_private->memb_size * p_private->block_nmemb;
        start_iter->first = *start_iter->map_node;
    }
    start_iter->cur -= p_private->memb_size;
    memcpy(start_iter->cur, x, p_private->memb_size);
    p_private->nmemb++;
}
static void pop_back(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (!p_private->nmemb)
        return;
    deque_iter *finish_iter = &p_private->finish_iter;
    finish_iter->cur -= p_private->memb_size;
    if (finish_iter->cur == finish_iter->first) {
        deallocate(*finish_iter->map_node, p_private->block_nmemb * p_private->memb_size);
        finish_iter->map_node--;
        finish_iter->cur = finish_iter->last = *finish_iter->map_node + p_private->memb_size * p_private->block_nmemb;
        finish_iter->first = *finish_iter->map_node;
    }
    p_private->nmemb--;
/*
    if (--p_private->nmemb){
        p_private->start_iter.cur = p_private->start_iter.first;
        p_private->finish_iter.cur = p_private->finish_iter.first;
    }
*/
}
static void pop_front(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (!p_private->nmemb)
        return;
    deque_iter *start_iter = &p_private->start_iter;
    start_iter->cur += p_private->memb_size;
    if (start_iter->cur == start_iter->last) {
        deallocate(*start_iter->map_node, p_private->block_nmemb * p_private->memb_size);
        start_iter->map_node++;
        start_iter->cur = start_iter->first = *start_iter->map_node;
        start_iter->last = *start_iter->map_node + p_private->memb_size * p_private->block_nmemb;
    }
    p_private->nmemb--;
//    if (--p_private)
}
static deque_iter *insert(deque_iter *iter, void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (iter->map_node - p_private->start_iter.map_node < p_private->finish_iter.map_node - iter->map_node) {
        THIS(this).push_front(x);
        deque_iter in_iter = p_private->start_iter;
        while (in_iter.map_node != iter->map_node)
        {
            deque_iter next_iter;
            next_iter.map_node = in_iter.map_node + 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->block_nmemb * p_private->memb_size;
            next_iter.cur = next_iter.first;
            memmove(in_iter.cur, in_iter.cur + p_private->memb_size, in_iter.last - in_iter.cur - p_private->memb_size);
            memcpy(in_iter.last - p_private->memb_size, next_iter.first, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(in_iter.first, in_iter.first + p_private->memb_size, iter->cur - in_iter.first - p_private->memb_size);
        memcpy(iter->cur - p_private->memb_size, x, p_private->memb_size);
    } else {
        THIS(this).push_back(x);
        deque_iter in_iter = p_private->finish_iter;
        while (in_iter.map_node != iter->map_node)
        {
            deque_iter next_iter;
            next_iter.map_node = in_iter.map_node - 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->block_nmemb * p_private->memb_size;
            next_iter.cur = next_iter.last;
            memmove(in_iter.first + p_private->memb_size, in_iter.first, in_iter.cur - in_iter.first - p_private->memb_size);
            memcpy(in_iter.first, next_iter.last - p_private->memb_size, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(iter->cur + p_private->memb_size, iter->cur, iter->last - iter->cur - p_private->memb_size);
        memcpy(iter->cur, x, p_private->memb_size);
    }
    return iter;
}
static deque_iter *erase(deque_iter *iter)
{
    deque_iter in_iter = *iter;
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (in_iter.map_node - p_private->start_iter.map_node < p_private->finish_iter.map_node - in_iter.map_node) {
        while(in_iter.map_node != p_private->start_iter.map_node)
        {
            deque_iter next_iter;
            next_iter.map_node = in_iter.map_node - 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->memb_size * p_private->block_nmemb;
            next_iter.cur = next_iter.last - p_private->memb_size;
            memmove(in_iter.first + p_private->memb_size, in_iter.first, in_iter.cur - in_iter.first);
            memcpy(in_iter.first, next_iter.cur, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(p_private->start_iter.cur + p_private->memb_size, p_private->start_iter.cur, in_iter.cur - p_private->start_iter.cur);
        THIS(this).pop_front();
    } else {
        while(in_iter.map_node != p_private->finish_iter.map_node)
        {
            deque_iter next_iter;
            next_iter.map_node = in_iter.map_node + 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->memb_size * p_private->block_nmemb;
            next_iter.cur = next_iter.first;
            memmove(in_iter.cur, in_iter.cur + p_private->memb_size, in_iter.last - in_iter.cur - p_private->memb_size);
            memcpy(in_iter.last - p_private->memb_size, next_iter.cur, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(in_iter.cur, in_iter.cur + p_private->memb_size, p_private->finish_iter.cur - in_iter.cur - p_private->memb_size);
        THIS(this).pop_back();
    }
    return iter;
}
static void clear(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    void **mid = p_private->mmap + p_private->mmap_len / 2;
    while (p_private->start_iter.map_node != mid)
    {
        deallocate(*p_private->start_iter.map_node, p_private->memb_size * p_private->block_nmemb);
        p_private->start_iter.map_node++;
    }
    while (p_private->finish_iter.map_node != mid)
    {
        deallocate(*p_private->finish_iter.map_node, p_private->memb_size * p_private->block_nmemb);
        p_private->finish_iter.map_node--;
    }
    p_private->start_iter.first = p_private->finish_iter.first = *mid;
    p_private->start_iter.cur = p_private->finish_iter.cur = *mid;
    p_private->start_iter.last = p_private->finish_iter.last = *mid + p_private->block_nmemb * p_private->memb_size;
    p_private->nmemb = 0;
}

static void *iter_at(obj_iter iter, int pos)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *_iter = iter;
    size_t block_len = _iter->last - _iter->cur;
    if (pos < block_len)
        return _iter->cur + p_private->memb_size * pos;
    pos -= block_len;
    int block_index = pos / (int)p_private->block_nmemb + 1;
    return *(p_private->mmap + block_index) + p_private->memb_size * (pos % p_private->block_nmemb);
}
static void iter_increment(obj_iter iter)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *_iter = iter;
    _iter->cur += p_private->memb_size;
    if (_iter->cur == _iter->last) {
        _iter->map_node++;
        _iter->first = *_iter->map_node;
        _iter->last = _iter->first + p_private->block_nmemb * p_private->memb_size;
        _iter->cur = _iter->first;
    }
}
static void iter_decrement(obj_iter iter)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *_iter = iter;
    if (_iter->cur == _iter->first) {
        _iter->map_node--;
        _iter->first = *_iter->map_node;
        _iter->last = _iter->first + p_private->block_nmemb * p_private->memb_size;
        _iter->cur = _iter->last;
    }
    _iter->cur -= p_private->memb_size;
}
static void iter_add(obj_iter iter, int v)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *_iter = iter;
    size_t block_len = _iter->last - _iter->cur;
    if (v < block_len) {
        _iter->cur += p_private->memb_size * v;
        return;
    }
    v -= block_len;
    int block_index = v / (int)p_private->block_nmemb + 1;
    void **node = _iter->map_node + block_index;
    _iter->map_node = node;
    _iter->first = *node;
    _iter->last = *node + p_private->memb_size * p_private->block_nmemb;
    _iter->cur = *node + (v % p_private->block_nmemb) * p_private->block_nmemb;
}
static void iter_sub(obj_iter iter, int v)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    deque_iter *_iter = iter;
    size_t block_len = _iter->cur - _iter->first;
    if (v <= block_len) {
        _iter->cur -= p_private->memb_size * v;
        return;
    }
    v -= block_len;
    int block_index = v / (int)p_private->block_nmemb + 1;
    void **node = _iter->map_node - block_index;
    _iter->map_node = node;
    _iter->first = *node;
    _iter->last = *node + p_private->memb_size * p_private->block_nmemb;
    _iter->cur = _iter->last - (v % p_private->block_nmemb) * p_private->block_nmemb;
}


static __iterator_obj_func  __def_deque_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub
};

static const deque __def_deque = {
        begin,
        end,
        size,
        empty,
        at,
        front,
        back,
        push_back,
        push_front,
        pop_back,
        pop_front,
        insert,
        erase,
        clear
};

void init_deque(deque *p_deque, size_t memb_size, size_t block_nmemb)
{
   *p_deque = __def_deque;
   __private_deque *p_private = (__private_deque*)p_deque->__obj_private;
   p_private->nmemb = 0;
   p_private->block_nmemb = block_nmemb;
   p_private->memb_size = memb_size;
   p_private->mmap_len = 1;
   p_private->mmap = reallocate(NULL, 0, p_private->mmap_len * sizeof(void*));
   *p_private->mmap = allocate(memb_size * block_nmemb);
   __init_iter((void*)&p_private->start, p_deque, sizeof(deque_iter), memb_size, &__def_deque_iter);
   __init_iter((void*)&p_private->finish, p_deque, sizeof(deque_iter), memb_size, &__def_deque_iter);

   p_private->start_iter.map_node = p_private->mmap;
   p_private->start_iter.first = p_private->start_iter.cur = *p_private->start_iter.map_node;
   p_private->start_iter.last = *p_private->start_iter.map_node + memb_size * block_nmemb;
   p_private->finish_iter = p_private->start_iter;
}

void destory_deque(deque *p_deque)
{
    __private_deque *p_private = (__private_deque*)p_deque->__obj_private;
    THIS(p_deque).clear();
    deallocate(*p_private->start_iter.map_node, p_private->memb_size * p_private->block_nmemb);
    deallocate(p_private->mmap, p_private->mmap_len);
}