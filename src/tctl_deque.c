//
// Created by xjs on 2020/9/12.
//
#include "../include/tctl_allocator.h"
#include "../include/tctl_deque.h"
#include "../include/tctl_algobase.h"
#include "include/_tctl_deque.h"
#include "../include/auto_release_pool.h"
#include "include/_tctl_metaclass.h"
#include <assert.h>
#include <cstdarg>
#include <cstddef>
#include <memory.h>
#include <pthread.h>
#include <string.h>

#define Import CLASS, ITERATOR, OBJECT, METACLASS
struct DequeIter {
    void *cur;
    void *first;
    void *last;
    void **map_node;
};

struct Deque {
    Form_t _t;
    void **map;
    size_t map_size;
    size_t buf_size;
    struct DequeIter start;
    struct DequeIter finish;
};

struct DequeClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    void* (*front)(const void *_this);
    void* (*back)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*push_back)(void *_this, FormWO_t x);
    void (*push_front)(void *_this, FormWO_t x);
    void (*pop_back)(void *_this);
    void (*pop_front)(void *_this);
    Iterator (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Iterator iter, FormWO_t x);
    void (*resize)(void *_this, size_t new_size);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Deque _d);
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static void* _front(void);
static void* _back(void);
static size_t _size(void);
static bool _empty(void);
static void _push_back(FormWO_t x);
static void _push_front(FormWO_t x);
static void _pop_back(void);
static void _pop_front(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, FormWO_t x);
static void _resize(size_t new_size);
static void _clear(void);
static void _swap(Deque _d);
//dequeclass
static void *_dequeclass_ctor(void *_this, va_list *app);
//deque
static void *_deque_ctor(void *_this, va_list *app);
static void *_deque_dtor(void *_this);
static void _deque_swap(void *_this, Deque _d);
static void _deque_clear(void *_this);
static void _deque_resize(void *_this, size_t new_size);
static Iterator _deque_insert(void *_this, Iterator _iter, FormWO_t x);
static Iterator _deque_erase(void *_this, Iterator _iter);
static void _deque_pop_front(void *_this);
static void _deque_pop_back(void *_this);
static void _deque_push_front(void *_this, FormWO_t x);
static void _deque_push_back(void *_this, FormWO_t x);
static bool _deque_empty(const void *_this);
static size_t _deque_max_size(const void *_this);
static size_t _deque_size(const void *_this);
static const void *_deque_back(const void *_this);
static const void *_deque_front(const void *_this);
static Iterator _deque_end(const void *_this);
static Iterator _deque_begin(const void *_this);
static void *_deque_brackets(const void *_this, FormWO_t _x);
//iterator
static void *_iter_sub(const void *_this, FormWO_t _x);
static void *_iter_add(const void *_this, FormWO_t _x);
static void _iter_asign(void *_this, FormWO_t _x);
static void _iter_self_sub(void *_this, FormWO_t _x);
static void _iter_self_add(void *_this, FormWO_t _x);
static void _iter_dec(void *_this);
static void _iter_inc(void *_this);
static void *_iter_brackets(const void *_this, FormWO_t _x);
static int _iter_cmp(const void *_this, FormWO_t _x);
static bool _iter_equal(const void *_this, FormWO_t _x);
static void *_iter_ctor(void *_this, va_list *app);
static void *_iter_derefer(const void *_this);
static long long _iter_dist(const void *_this, Iterator _it);
//init
static const void *__DequeIter = NULL;
static const void *__Deque = NULL;
static const void *__DequeClass = NULL;
volatile static struct DequeSelector DequeS = {
        {},
        _begin,
        _end,
        _front,
        _back,
        _size,
        _max_size,
        _empty,
        _push_back,
        _push_front,
        _pop_back,
        _pop_front,
        _erase,
        _insert,
        _resize,
        _clear,
        _swap
};
const struct DequeSelector *_DequeS= NULL;

void initdeque(void){}

Form_t _Deque(void)
{
    Form_t t = {OBJ, .class = __Deque};
    return t;
}

Form_t _DequeClass(void)
{
    Form_t t = {OBJ, .class = __DequeClass};
    return t;
}

static Form_t _DequeIter(void)
{
    Form_t t = {OBJ, .class = __DequeIter};
    return t;
}
//private
static void extend_map(struct Deque *this)
{
    size_t old_len = this->map_size;
    size_t start_offset = this->start.map_node - this->map;
    size_t finish_offset = this->finish.map_node - this->start.map_node;
    this->map_size *= 2;
    this->map = reallocate(this->map, old_len, this->map_size);
    memmove(this->map + old_len / 2, this->map + start_offset, old_len * sizeof(void*));

    this->start.map_node = this->map + old_len / 2;
    this->finish.map_node = this->start.map_node + finish_offset;
}

//public
//Iterator
//DequeClass
//Deque
static void *_deque_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Deque, _this, app);
    struct Deque *this = offsetOf(_this, __Deque);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f >= FORM);
    t._.f -= FORM;
    this->_t = t._;
    this->buf_size = 512;
    this->map_size = 1;
    this->map = allocate(this->map_size * sizeof(void*));

    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    *this->map = allocate(memb_size * this->buf_size);

    this->start.map_node = this->map;
    this->start.first = this->start.cur = *this->map;
    this->start.last = (char*)(this->map) + memb_size * this->buf_size;
    this->finish = this->start;
    return _this;
}

static void *_deque_dtor(void *_this)
{
    _this = super_dtor(__Deque, _this);
    struct Deque *this = offsetOf(_this, __Deque);
    _deque_clear(_this);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    deallocate(*this->start.map_node, memb_size * this->buf_size);
    deallocate(this->map, this->map_size * sizeof(void*));
    return _this;
}

static Iterator _deque_begin(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    void *mem = ARP_MallocDtor(classSz(__DequeIter), destroy);
    return new(compose(_DequeIter(), mem), VA(this->_t, VA_ADDR(this->start)));
}

static Iterator _deque_end(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    void *mem = ARP_MallocDtor(classSz(__DequeIter), destroy);
    return new(compose(_DequeIter(), mem), VA(this->_t, VA_ADDR(this->finish)));
}

static const void *_deque_front(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    return this->start.cur;
}

static const void *_deque_back(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    return this->finish.cur;
}

static size_t _deque_size(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    struct DequeIter *start = &this->start;
    struct DequeIter *finish = &this->finish;
    if (start->map_node == finish->map_node)
        return ((char*)finish->cur - (char*)start->cur) / memb_size;
    size_t block_diff = finish->map_node - start->map_node - 1;
    long long res = finish->cur - finish->first + start->last - start->cur;
    return res / memb_size + block_diff * this->buf_size;
}

static bool _deque_empty(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    return this->start.map_node == this->finish.map_node && 
           this->start.cur == this->finish.cur;
}

static void _deque_push_back(void *_this, FormWO_t _x)
{
    struct Deque *this = offsetOf(_this, __Deque);
    struct DequeIter *finish = &this->finish;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (finish->cur == finish->last) {
        if (finish->map_node == this->map + this->map_size - 1)
            extend_map(this);
        finish->map_node++;
        *finish->map_node = allocate(this->buf_size * memb_size);
        finish->cur = finish->first = *finish->map_node;
        finish->last = (char*)*finish->map_node + memb_size * this->buf_size;
    }
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(finish->cur, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(finish->cur, &_x.mem, memb_size);
    } else {
        construct(this->_t, finish->cur, _x);
    }
    finish->cur = (char*)finish->cur + memb_size;
}

static void _deque_push_front(void *_this, FormWO_t _x)
{
    struct Deque *this = offsetOf(_this, __Deque);
    struct DequeIter *start = &this->start;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (start->cur == start->first) {
        if (start->map_node == this->map)
            extend_map(this);
        start->map_node--;
        *start->map_node = allocate(this->buf_size * memb_size);
        start->cur = start->last = (char*)*start->map_node + memb_size * this->buf_size;
        start->first = *start->map_node;
    }
    start->cur = (char*)start->cur - memb_size;
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(start->cur, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(start->cur, &_x.mem, memb_size);
    } else {
        construct(this->_t, start->cur, _x);
    }
}

static void _deque_pop_back(void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    struct DequeIter *finish = &this->finish;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    finish->cur = (char*)finish->cur - memb_size;
    if (finish->cur == finish->first) {
        deallocate(*finish->map_node, this->buf_size * memb_size);
        finish->map_node--;
        finish->cur = finish->last = (char*)*finish->map_node + this->buf_size * memb_size;
        finish->first = *finish->map_node;
    }
}

static void _deque_pop_front(void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    struct DequeIter *start = &this->start;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (start->cur == start->last) {
        deallocate(*start->map_node, this->buf_size * memb_size);
        start->map_node++;
        start->cur = start->first = *start->map_node;
        start->last = (char*)*start->map_node + this->buf_size * memb_size;
    }
}

static Iterator _deque_erase(void *_this, Iterator _iter)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    Iterator first = _deque_begin(_this);
    Iterator last = _deque_end(_this);
    long long front_dist = _iter_dist(_iter, first);
    long long back_dist = _iter_dist(last, _iter);
    if (front_dist < back_dist) {
        //复制内存块，移动内存
        Iterator iter_pre = _iter_sub(_iter, VA(1));
        Iterator first_next = _iter_add(first, VA(1));
        copy(first, iter_pre, first_next);
        _deque_pop_front(_this);
        _iter_inc(_iter);//传出去的迭代器后前移动
    } else {
        Iterator iter_next = _iter_add(_iter, VA(1));
        copy(iter_next, last, _iter);
        _deque_pop_back(_this);
    }
    return _iter;
}

static Iterator _deque_insert(void *_this, Iterator _iter, FormWO_t x)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    Iterator first = _deque_begin(_this);
    Iterator last = _deque_end(_this);
    long long front_dist = _iter_dist(_iter, first);
    long long back_dist = _iter_dist(last, _iter);
    if (front_dist < back_dist) {
        //复制内存块，移动内存
        _deque_push_front(_this, VAEND);//VAEND填充FormWO_t，让其调用无参构造函数
        Iterator first_pre = _iter_sub(first, VA(1));
        copy(first, _iter, first_pre);
        _iter_dec(_iter);
        Object obj = _iter_derefer(_iter);
        THIS(obj).asign(x);
    } else {
        _deque_push_back(_this, VAEND);
        Iterator iter_next = _iter_add(_iter, VA(1));
        copy(_iter, last, iter_next);
        Object obj = _iter_derefer(_iter);
        THIS(obj).asign(x);
    }
    return _iter;
}

static void _deque_swap(void *_this, Deque _d)
{
    struct Deque *this = offsetOf(_this, __Deque);
    struct Deque *D = offsetOf(_d, __Deque);
    assert(this->_t.f == D->_t.f && this->_t.class == D->_t.class);
    struct Deque tmp = *this;
    *this = *D;
    *D = tmp;
}

static void _deque_clear(void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    for (void **node = this->start.map_node + 1; node < this->finish.map_node; node++) {
        char *end = (char*)*node + this->buf_size;
        for (char *ptr = *node; ptr < end; ptr += memb_size)
            destroy(ptr);
        deallocate(*node, this->buf_size);
    }
    if (this->start.map_node != this->finish.map_node) {
        for (char *ptr = this->finish.cur; ptr >= (char*)this->finish.first; ptr -= memb_size)
            destroy(ptr);
        for (char *ptr = this->start.cur; ptr < (char*)this->start.last; ptr += memb_size)
            destroy(ptr);
        deallocate(*this->finish.map_node, this->buf_size);
    } else {
        for (char *ptr = this->start.cur; ptr < (char*)this->start.last; ptr += memb_size)
            destroy(ptr);
    }
    this->finish = this->start;
}

static void _deque_resize(void *_this, size_t new_size)
{
}
static void *_deque_brackets(const void *_this, FormWO_t _x);
