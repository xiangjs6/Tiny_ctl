//
// Created by xjs on 2020/9/12.
//
#include "tctl_allocator.h"
#include "tctl_deque.h"
#include "auto_release_pool/auto_release_pool.h"
#include <memory.h>
//private
static void *iter_at(__iterator *iter, int pos)
{
    deque *this = pop_this();
    long long dist = ITER(iter).diff(THIS(this).begin());
    dist += pos;
    return THIS(this).at(dist);
}

static void iter_increment(__iterator *iter)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *_iter = (__deque_iter*)iter->__inner.__address;
    _iter->cur += p_private->memb_size;
    if (_iter->cur == _iter->last) {
        _iter->map_node++;
        _iter->first = *_iter->map_node;
        _iter->last = _iter->first + p_private->block_nmemb * p_private->memb_size;
        _iter->cur = _iter->first;
    }
}

static void iter_decrement(__iterator *iter)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *_iter = (__deque_iter*)iter->__inner.__address;
    if (_iter->cur == _iter->first) {
        _iter->map_node--;
        _iter->first = *_iter->map_node;
        _iter->last = _iter->first + p_private->block_nmemb * p_private->memb_size;
        _iter->cur = _iter->last;
    }
    _iter->cur -= p_private->memb_size;
}

static void iter_add(__iterator *iter, int v)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *_iter = (__deque_iter*)iter->__inner.__address;
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

static void iter_sub(__iterator *iter, int v)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *_iter = (__deque_iter*)iter->__inner.__address;
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

static long long iter_diff(const __iterator *minuend, const __iterator *subtraction)
{
    __deque_iter *_minuend = (__deque_iter*)minuend->__inner.__address;
    __deque_iter *_subtraction = (__deque_iter*)subtraction->__inner.__address;
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (_minuend->map_node == _subtraction->map_node)
        return (_minuend->cur - _subtraction->cur) / p_private->memb_size;
    long long block_diff = _minuend->map_node - _subtraction->map_node;
    long long res = block_diff < 0 ?
                    -(_minuend->last - _minuend->cur + _subtraction->cur - _subtraction->first) :
                    _minuend->cur - _minuend->first + _subtraction->last - _subtraction->cur;
    block_diff = block_diff > 0 ? block_diff - 1 : block_diff + 1;
    return res / (long long)p_private->memb_size + block_diff * p_private->block_nmemb;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    pop_this();
    return it1->val == it2->val;
}

static const __iterator_obj_func  __def_deque_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff,
        iter_equal
};

static const iterator_func __def_deque_iter_func = INIT_ITER_FUNC(&__def_deque_iter);

static void extend_map(__private_deque *p_private)
{
    size_t old_len = p_private->mmap_len;
    size_t start_off = p_private->start_ptr.map_node - p_private->mmap;
    size_t finish_off = p_private->finish_ptr.map_node - p_private->start_ptr.map_node;
    p_private->mmap_len = 2 * p_private->mmap_len;
    p_private->mmap = reallocate(p_private->mmap, old_len * sizeof(void*), p_private->mmap_len * sizeof(void*));
    memmove(p_private->mmap + old_len / 2, p_private->mmap + start_off, old_len * sizeof(void*));

    p_private->start_ptr.map_node = p_private->mmap + old_len / 2;
    p_private->start_ptr.cur = *p_private->start_ptr.map_node + (p_private->start_ptr.cur - p_private->start_ptr.first);
    p_private->start_ptr.first = *p_private->start_ptr.map_node;
    p_private->start_ptr.last = *p_private->start_ptr.map_node + p_private->memb_size * p_private->block_nmemb;

    p_private->finish_ptr.map_node = p_private->start_ptr.map_node + finish_off;
    p_private->finish_ptr.cur = *p_private->finish_ptr.map_node + (p_private->finish_ptr.cur - p_private->finish_ptr.first);
    p_private->finish_ptr.first = *p_private->finish_ptr.map_node;
    p_private->finish_ptr.last = *p_private->finish_ptr.map_node + p_private->memb_size * p_private->block_nmemb;
}
//public
static IterType begin(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    struct __inner_iterator *start = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__deque_iter));
    *start = __creat_iter(sizeof(__deque_iter), this, p_private->memb_size, &__def_deque_iter_func);
    memcpy(start->__address, &p_private->start_ptr, sizeof(__deque_iter));
    return start;
}
static IterType end(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    struct __inner_iterator *finish = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__deque_iter));
    *finish = __creat_iter(sizeof(__deque_iter), this, p_private->memb_size, &__def_deque_iter_func);
    memcpy(finish->__address, &p_private->finish_ptr, sizeof(__deque_iter));
    return finish;
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
    const __deque_iter *start_ptr = &p_private->start_ptr;
    //__deque_iter *finish_iter = p_private->finish.ptr;
    size_t start_len = (start_ptr->last - start_ptr->cur) / p_private->memb_size;
    //size_t finish_len = finish_iter->cur - finish_iter->first;
    if (pos < start_len)
        return start_ptr->cur + pos * p_private->memb_size;
    pos -= start_len;
    int block_index = pos / (int)p_private->block_nmemb + 1;
    void **p_block = start_ptr->map_node + block_index;
    pos %= p_private->block_nmemb;
    return *p_block + pos * p_private->memb_size;
}
static void const *front(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    return p_private->start_ptr.cur;
}
static void const *back(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *finish_iter = &p_private->finish_ptr;
    return finish_iter->cur - p_private->memb_size;
}
static void push_back(void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *finish_ptr = &p_private->finish_ptr;
    if (finish_ptr->cur == finish_ptr->last) {
        if (finish_ptr->map_node == p_private->mmap + p_private->mmap_len - 1)
            extend_map(p_private);
        finish_ptr->map_node++;
        *finish_ptr->map_node = allocate(p_private->block_nmemb * p_private->memb_size);
        finish_ptr->cur = finish_ptr->first = *finish_ptr->map_node;
        finish_ptr->last = *finish_ptr->map_node + p_private->memb_size * p_private->block_nmemb;
    }
    memcpy(finish_ptr->cur, x, p_private->memb_size);
    finish_ptr->cur += p_private->memb_size;
    p_private->nmemb++;
}
static void push_front(void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __deque_iter *start_ptr = &p_private->start_ptr;
    if (start_ptr->cur == start_ptr->first) {
        if (start_ptr->map_node == p_private->mmap)
            extend_map(p_private);
        start_ptr->map_node--;
        *start_ptr->map_node = allocate(p_private->block_nmemb * p_private->memb_size);
        start_ptr->cur = start_ptr->last = *start_ptr->map_node + p_private->memb_size * p_private->block_nmemb;
        start_ptr->first = *start_ptr->map_node;
    }
    start_ptr->cur -= p_private->memb_size;
    memcpy(start_ptr->cur, x, p_private->memb_size);
    p_private->nmemb++;
}
static void pop_back(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (!p_private->nmemb)
        return;
    __deque_iter *finish_ptr = &p_private->finish_ptr;
    finish_ptr->cur -= p_private->memb_size;
    if (finish_ptr->cur == finish_ptr->first) {
        deallocate(*finish_ptr->map_node, p_private->block_nmemb * p_private->memb_size);
        finish_ptr->map_node--;
        finish_ptr->cur = finish_ptr->last = *finish_ptr->map_node + p_private->memb_size * p_private->block_nmemb;
        finish_ptr->first = *finish_ptr->map_node;
    }
    p_private->nmemb--;
}
static void pop_front(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (!p_private->nmemb)
        return;
    __deque_iter *start_ptr = &p_private->start_ptr;
    start_ptr->cur += p_private->memb_size;
    if (start_ptr->cur == start_ptr->last) {
        deallocate(*start_ptr->map_node, p_private->block_nmemb * p_private->memb_size);
        start_ptr->map_node++;
        start_ptr->cur = start_ptr->first = *start_ptr->map_node;
        start_ptr->last = *start_ptr->map_node + p_private->memb_size * p_private->block_nmemb;
    }
    p_private->nmemb--;
//    if (--p_private)
}
static IterType insert(IterType iter, void *x)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    __iterator *__iter = iter;
    const __deque_iter *_d_iter = (__deque_iter*)__iter->__inner.__address;
    if (_d_iter->map_node - p_private->start_ptr.map_node < p_private->finish_ptr.map_node - _d_iter->map_node) {
        THIS(this).push_front(x);
        __deque_iter in_iter = p_private->start_ptr;
        while (in_iter.map_node != _d_iter->map_node)
        {
            __deque_iter next_iter;
            next_iter.map_node = in_iter.map_node + 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->block_nmemb * p_private->memb_size;
            next_iter.cur = next_iter.first;
            memmove(in_iter.cur, in_iter.cur + p_private->memb_size, in_iter.last - in_iter.cur - p_private->memb_size);
            memcpy(in_iter.last - p_private->memb_size, next_iter.first, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(in_iter.first, in_iter.first + p_private->memb_size, _d_iter->cur - in_iter.first - p_private->memb_size);
        memcpy(_d_iter->cur - p_private->memb_size, x, p_private->memb_size);
    } else {
        THIS(this).push_back(x);
        __deque_iter in_iter = p_private->finish_ptr;
        while (in_iter.map_node != _d_iter->map_node)
        {
            __deque_iter next_iter;
            next_iter.map_node = in_iter.map_node - 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->block_nmemb * p_private->memb_size;
            next_iter.cur = next_iter.last;
            memmove(in_iter.first + p_private->memb_size, in_iter.first, in_iter.cur - in_iter.first - p_private->memb_size);
            memcpy(in_iter.first, next_iter.last - p_private->memb_size, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(_d_iter->cur + p_private->memb_size, _d_iter->cur, _d_iter->last - _d_iter->cur - p_private->memb_size);
        memcpy(_d_iter->cur, x, p_private->memb_size);
    }
    return iter;
}
static IterType erase(IterType iter)
{
    __iterator *__iter = iter;
    __deque_iter in_iter = *(__deque_iter*)__iter->__inner.__address;
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    if (in_iter.map_node - p_private->start_ptr.map_node < p_private->finish_ptr.map_node - in_iter.map_node) {
        //复制内存块，移动内存
        while(in_iter.map_node != p_private->start_ptr.map_node)
        {
            __deque_iter next_iter;
            next_iter.map_node = in_iter.map_node - 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->memb_size * p_private->block_nmemb;
            next_iter.cur = next_iter.last - p_private->memb_size;
            memmove(in_iter.first + p_private->memb_size, in_iter.first, in_iter.cur - in_iter.first);
            memcpy(in_iter.first, next_iter.cur, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(p_private->start_ptr.cur + p_private->memb_size, p_private->start_ptr.cur, in_iter.cur - p_private->start_ptr.cur);
        THIS(this).pop_front();
    } else {
        while(in_iter.map_node != p_private->finish_ptr.map_node)
        {
            __deque_iter next_iter;
            next_iter.map_node = in_iter.map_node + 1;
            next_iter.first = *next_iter.map_node;
            next_iter.last = *next_iter.map_node + p_private->memb_size * p_private->block_nmemb;
            next_iter.cur = next_iter.first;
            memmove(in_iter.cur, in_iter.cur + p_private->memb_size, in_iter.last - in_iter.cur - p_private->memb_size);
            memcpy(in_iter.last - p_private->memb_size, next_iter.cur, p_private->memb_size);
            in_iter = next_iter;
        }
        memmove(in_iter.cur, in_iter.cur + p_private->memb_size, p_private->finish_ptr.cur - in_iter.cur - p_private->memb_size);
        THIS(this).pop_back();
    }
    return iter;
}
static void clear(void)
{
    deque *this = pop_this();
    __private_deque *p_private = (__private_deque*)this->__obj_private;
    void **mid = p_private->mmap + p_private->mmap_len / 2;
    while (p_private->start_ptr.map_node != mid)
    {
        deallocate(*p_private->start_ptr.map_node, p_private->memb_size * p_private->block_nmemb);
        p_private->start_ptr.map_node++;
    }
    while (p_private->finish_ptr.map_node != mid)
    {
        deallocate(*p_private->finish_ptr.map_node, p_private->memb_size * p_private->block_nmemb);
        p_private->finish_ptr.map_node--;
    }
    p_private->start_ptr.first = p_private->finish_ptr.first = *mid;
    p_private->start_ptr.cur = p_private->finish_ptr.cur = *mid;
    p_private->start_ptr.last = p_private->finish_ptr.last = *mid + p_private->block_nmemb * p_private->memb_size;
    p_private->nmemb = 0;
}



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

   p_private->start_ptr.map_node = p_private->mmap;
   p_private->start_ptr.first = p_private->start_ptr.cur = *p_private->start_ptr.map_node;
   p_private->start_ptr.last = *p_private->start_ptr.map_node + memb_size * block_nmemb;
   p_private->finish_ptr = p_private->start_ptr;
}

void destory_deque(deque *p_deque)
{
    __private_deque *p_private = (__private_deque*)p_deque->__obj_private;
    THIS(p_deque).clear();
    deallocate(*p_private->start_ptr.map_node, p_private->memb_size * p_private->block_nmemb);
    deallocate(p_private->mmap, p_private->mmap_len);
}

deque creat_deque(size_t memb_size, size_t block_nmemb)
{
    deque deq;
    init_deque(&deq, memb_size, block_nmemb);
    return deq;
}
