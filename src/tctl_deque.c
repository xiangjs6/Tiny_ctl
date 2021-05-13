//
// Created by xjs on 2020/9/12.
//
#include "../include/tctl_allocator.h"
#include "../include/tctl_deque.h"
#include "../include/tctl_algobase.h"
#include "../include/tctl_int.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include "include/_tctl_deque.h"
#include "include/_tctl_metaclass.h"
#include "include/_tctl_iterator.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#define Import CLASS, ITERATOR, OBJECT, METACLASS, ANY, INT
struct DequeIter {
    void *cur;
    void *first;
    void *last;
    void **map_node;
};

struct Deque {
    void *class;
    void **map;
    size_t map_size;
    size_t buf_size;
    struct DequeIter start;
    struct DequeIter finish;
};

struct DequeClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    void* (*front)(const void *_self);
    void* (*back)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*push_back)(void *_self, const void *x);
    void (*push_front)(void *_self, const void *x);
    void (*pop_back)(void *_self);
    void (*pop_front)(void *_self);
    Iterator (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Iterator iter, const void *x);
    void (*resize)(void *_self, size_t new_size);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Deque _d);
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static void* _front(void);
static void* _back(void);
static size_t _size(void);
static bool _empty(void);
static void _push_back(const void *x);
static void _push_front(const void *x);
static void _pop_back(void);
static void _pop_front(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, const void *x);
static void _resize(size_t new_size);
static void _clear(void);
static void _swap(Deque _d);
//dequeclass
static void *_dequeclass_ctor(void *_self, va_list *app);
//deque
static void *_deque_ctor(void *_self, va_list *app);
static void *_deque_dtor(void *_self);
static void _deque_swap(void *_self, Deque _d);
static void _deque_clear(void *_self);
static void _deque_resize(void *_self, size_t new_size);
static Iterator _deque_insert(void *_self, Iterator _iter, const void *x);
static Iterator _deque_erase(void *_self, Iterator _iter);
static void _deque_pop_front(void *_self);
static void _deque_pop_back(void *_self);
static void _deque_push_front(void *_self, const void *x);
static void _deque_push_back(void *_self, const void *x);
static bool _deque_empty(const void *_self);
static size_t _deque_size(const void *_self);
static const void *_deque_back(const void *_self);
static const void *_deque_front(const void *_self);
static Iterator _deque_end(const void *_self);
static Iterator _deque_begin(const void *_self);
static void *_deque_brackets(const void *_self, const void *_x);
//iterator
static void *_iter_sub(const void *_self, const void *_x);
static void *_iter_add(const void *_self, const void *_x);
static void _iter_assign(void *_self, const void *_x);
static void _iter_self_sub(void *_self, const void *_x);
static void _iter_self_add(void *_self, const void *_x);
static void _iter_dec(void *_self);
static void _iter_inc(void *_self);
static void *_iter_brackets(const void *_self, const void *_x);
static int _iter_cmp(const void *_self, const void *_x);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
static long long _iter_dist(const void *_self, Iterator _it);
static long long _riter_dist(const void *_self, Iterator _it);
static Iterator _iter_reverse_iterator(const void *_self);
//init
static const void *__DequeIter = NULL;
static const void *__RDequeIter = NULL;
static const void *__Deque = NULL;
static const void *__DequeClass = NULL;
volatile static struct DequeSelector DequeS = {
        { 0 },
        _begin,
        _end,
        _front,
        _back,
        _size,
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

static void initDeque(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!_DequeS) {
        _DequeS = (void*)&DequeS;
        memcpy((void*)&DequeS, _ClassS, sizeof(*_ClassS));
    }
    if (!__DequeIter) {
        __DequeIter = new(_IteratorClass(), "DequeIter",
                           T(Iterator), sizeof(struct DequeIter) + classSz(T(Iterator)),
                           _MetaClassS->ctor, _iter_ctor,
                           _ClassS->equal, _iter_equal,
                           _ClassS->cmp, _iter_cmp,
                           _ClassS->brackets, _iter_brackets,
                           _ClassS->inc, _iter_inc,
                           _ClassS->dec, _iter_dec,
                           _ClassS->self_add, _iter_self_add,
                           _ClassS->self_sub, _iter_self_sub,
                           _ClassS->assign, _iter_assign,
                           _ClassS->add, _iter_add,
                           _ClassS->sub, _iter_sub,
                           _IteratorS->derefer, _iter_derefer,
                           _IteratorS->dist, _iter_dist,
                           _IteratorS->reverse_iterator, _iter_reverse_iterator,
                           Selector, _IteratorS, NULL);
    }
    if (!__RDequeIter) {
        __RDequeIter = new(_IteratorClass(), "RDequeIter",
                           T(Iterator), sizeof(struct DequeIter) + classSz(T(Iterator)),
                           _MetaClassS->ctor, _iter_ctor,
                           _ClassS->equal, _iter_equal,
                           _ClassS->cmp, _iter_cmp,
                           _ClassS->brackets, _iter_brackets,
                           _ClassS->inc, _iter_dec,
                           _ClassS->dec, _iter_inc,
                           _ClassS->self_add, _iter_self_sub,
                           _ClassS->self_sub, _iter_self_add,
                           _ClassS->assign, _iter_assign,
                           _ClassS->add, _iter_sub,
                           _ClassS->sub, _iter_add,
                           _IteratorS->derefer, _iter_derefer,
                           _IteratorS->dist, _riter_dist,
                           _IteratorS->reverse_iterator, _iter_reverse_iterator,
                           Selector, _IteratorS, NULL);
    }
    if (!__DequeClass) {
        __DequeClass = new(T(MetaClass), "DequeClass",
                            T(Class), sizeof(struct DequeClass) + classSz(T(Class)),
                            _MetaClassS->ctor, _dequeclass_ctor, NULL);
    }
    if (!__Deque) {
        __Deque = new(_DequeClass(), "Deque",
                       T(Object), sizeof(struct Deque) + classSz(T(Object)),
                       _MetaClassS->ctor, _deque_ctor,
                       _MetaClassS->dtor, _deque_dtor,
                       _ClassS->brackets, _deque_brackets,
                       DequeS.begin, _deque_begin,
                       DequeS.end, _deque_end,
                       DequeS.front, _deque_front,
                       DequeS.back, _deque_back,
                       DequeS.size, _deque_size,
                       DequeS.empty, _deque_empty,
                       DequeS.push_back, _deque_push_back,
                       DequeS.push_front, _deque_push_front,
                       DequeS.pop_back, _deque_pop_back,
                       DequeS.pop_front, _deque_pop_front,
                       DequeS.erase, _deque_erase,
                       DequeS.insert, _deque_insert,
                       DequeS.resize, _deque_resize,
                       DequeS.clear, _deque_clear,
                       DequeS.swap, _deque_swap,
                       Selector, _DequeS, NULL);
    }
}

const void *_Deque(void)
{
    if (!__Deque)
        initDeque();
    return __Deque;
}

const void *_DequeClass(void)
{
    if (!__DequeClass)
        initDeque();
    return __DequeClass;
}

//private
static void extend_map(struct Deque *self)
{
    size_t old_len = self->map_size;
    size_t start_offset = self->start.map_node - self->map;
    size_t finish_offset = self->finish.map_node - self->start.map_node;
    self->map_size *= 2;
    self->map = reallocate(self->map, old_len * sizeof(void*), self->map_size * sizeof(void*));
    memmove(self->map + old_len / 2, self->map + start_offset, old_len * sizeof(void*));

    self->start.map_node = self->map + old_len / 2;
    self->finish.map_node = self->start.map_node + finish_offset;
}

static long long dist_aux(const struct DequeIter *it1, const struct DequeIter *it2, size_t memb_size, size_t buf_size)
{
    if (it1->map_node == it2->map_node)
        return ((char*)it2->cur - (char*)it1->cur) / memb_size;
    long long node_dist = it2->map_node - it1->map_node;
    long long res = node_dist < 0 ?
                    -((char*)it2->last - (char*)it2->cur + (char*)it1->cur - (char*)it1->first) :
                    (char*)it2->cur - (char*)it2->first + (char*)it1->last - (char*)it1->cur;
    node_dist = node_dist > 0 ? node_dist - 1 : node_dist + 1;
    return res / (long long)memb_size + node_dist * buf_size;
}

static void _deal_Deque_Args(void *_self, MetaObject *args, int n)
{
    if (classOf(*args) == __Deque) { //复制一个Deque
        struct Deque *d = offsetOf(*args, __Deque);
        size_t d_memb_size = classSz(d->class);
        struct DequeIter it = d->start;
        while (it.cur != d->finish.cur)
        {
            _deque_push_back(_self, it.cur);
            it.cur = (char*)it.cur + d_memb_size;
            if (it.cur == it.last) {
                it.map_node++;
                it.first = it.cur = *it.map_node;
                it.last = (char*)*it.map_node + d_memb_size * d->buf_size;
            }
        }
    } else if (classOf(*args) == T(Any)) { //size_type n, T value = T() 构造方法
        Any any = (Any)*args;
        Int ii = THIS(any).cast(T(Int));
        unsigned long long nmemb = ii->val;
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _deque_push_back(_self, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _deque_push_back(_self, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(class_check(args[0], T(Iterator)));
        assert(class_check(args[1], T(Iterator)));
        Iterator first = (Iterator)args[0];
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, first, VAEND);
        Iterator last = (Iterator)args[1];
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, last, VAEND);
        while (!THIS(first).equal(last))
        {
            void *obj = THIS(first).derefer();
            _deque_push_back(_self, obj);
            THIS(first).inc();
        }
        destroy(first);
        destroy(last);
    }
}

//public
//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__DequeIter, _self, app);
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    self->cur = NULL;
    self->first = NULL;
    self->last = NULL;
    self->map_node = NULL;
    void *t = va_arg(*app, void*);
    if (classOf(t) == __DequeIter || classOf(t) == __RDequeIter)
        _iter_assign(_self, t);
    else {
        assert(classOf(t) == T(Any));
        Any any = t;
        memcpy(self, THIS(any).value(), sizeof(struct DequeIter));
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    assert(classOf(_x) == __DequeIter || classOf(_x) == __RDequeIter);
    struct DequeIter *x = offsetOf(_x, __DequeIter);
    return self->cur == x->cur;
}

static int _iter_cmp(const void *_self, const void *_x)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    assert(classOf(_x) == __DequeIter || classOf(_x) == __RDequeIter);
    struct DequeIter *x = offsetOf(_x, __DequeIter);
    if (self->map_node < x->map_node)
        return -1;
    else if (self->map_node > x->map_node)
        return 1;
    else {
        if (self->cur < x->cur)
            return -1;
        else if (self->cur > x->cur)
            return 1;
        return 0;
    }
}

static void *_iter_brackets(const void *_self, const void *_x)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    Int x = THIS((MetaObject)_x).cast(T(Int));
    size_t front_len = ((char*)self->cur - (char*)self->first) / memb_size;
    size_t back_len = ((char*)self->last - (char*)self->cur) / memb_size;
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    if (x->val >= 0) {
        if (x->val < back_len)
            return (char*)self->cur + memb_size * x->val;
        x->val -= back_len;
        long long node_index = x->val / buf_size + 1;
        void **node = self->map_node + node_index;
        x->val %= buf_size;
        return *node + x->val * memb_size;
    } else {
        if (-x->val < front_len)
            return (char*)self->cur + memb_size * x->val;
        x->val += front_len;
        long long node_index = x->val / buf_size - 1; //这是负数
        void **node = self->map_node + node_index;
        x->val %= buf_size;
        return (char*)*node + x->val * memb_size + buf_size; //负数要从后向前
    }
}

static void _iter_inc(void *_self)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    self->cur = (char*)self->cur + memb_size;
    if (self->cur == self->last) {
        self->map_node++;
        self->cur = self->first = *self->map_node;
        self->last = (char*)*self->map_node + buf_size * memb_size;
    }
}

static void _iter_dec(void *_self)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    if (self->cur == self->first) {
        self->map_node--;
        self->first = *self->map_node;
        self->last = self->cur = (char*)*self->map_node + buf_size * memb_size;
    }
    self->cur = (char*)self->cur - memb_size;
}

static void _iter_assign(void *_self, const void *_x)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    assert(classOf(_x) == __DequeIter || classOf(_x) == __RDequeIter);
    struct DequeIter *x = offsetOf(_x, __DequeIter);
    *self = *x;
}

static void _iter_self_add(void *_self, const void *_x)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    long long x;
    if (classOf(_x) == T(Int))
        x = ((Int)_x)->val;
    else
        x = ((Int)THIS((MetaObject)_x).cast(T(Int)))->val;
    long long offset = x + (self->cur - self->first) / memb_size;
    if (offset >= 0 && offset < buf_size) {
        self->cur = (char*)self->cur + memb_size * x;
    } else {
        long long node_offset = offset > 0 ?
                                offset / buf_size :
                                -(-offset - 1) / buf_size - 1;
        void **node = self->map_node + node_offset;
        self->map_node = node;
        self->first = *node;
        self->last = (char*)*node + buf_size * memb_size;
        self->cur = (char*)self->first + (offset - node_offset * buf_size) * memb_size;
    }
}

static void _iter_self_sub(void *_self, const void *_x)
{
    if (classOf(_x) != T(Int))
        _x = THIS((MetaObject)_x).cast(T(Int));
    Int x = (void*)_x;
    x->val = -x->val;
    _iter_self_add(_self, x);
}

static void *_iter_add(const void *_self, const void *_x)
{
    Iterator it = (void*)_self;
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Iterator new_it = construct(__DequeIter, mem, it, VAEND);
    _iter_self_add(new_it, _x);
    return new_it;
}

static void *_iter_sub(const void *_self, const void *_x)
{
    Iterator it = (void*)_self;
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Iterator new_it = construct(__DequeIter, mem, it, VAEND);
    _iter_self_sub(new_it, _x);
    return new_it;
}

static void *_iter_derefer(const void *_self)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    return self->cur;
}

static long long _iter_dist(const void *_self, Iterator _it)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    assert(classOf(_it) == __DequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    struct DequeIter *it = offsetOf(_it, __DequeIter);
    return dist_aux(self, it, memb_size, buf_size);
}

static long long _riter_dist(const void *_self, Iterator _it)
{
    struct DequeIter *self = offsetOf(_self, __DequeIter);
    assert(classOf(_it) == __RDequeIter);
    const void *class = ((Iterator)_self)->class;
    size_t memb_size = classSz(class);
    size_t buf_size = ((char*)self->last - (char*)self->first) / memb_size;
    struct DequeIter *it = offsetOf(_it, __DequeIter);
    return dist_aux(it, self, memb_size, buf_size);
}

static Iterator _iter_reverse_iterator(const void *_self)
{
    Iterator it = (void*)_self;
    if (classOf(_self) == __RDequeIter) {
        void *mem = ARP_MallocARelDtor(classSz(__RDequeIter), destroy);
        return construct(__DequeIter, mem, it, VAEND);
    } else {
        void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
        return construct(__RDequeIter, mem, it, VAEND);
    }
}

//DequeClass
static void *_dequeclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__DequeClass, _self, app);
    struct DequeClass *self = offsetOf(_self, __DequeClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&DequeS + sizeof(DequeS._);
    voidf *end = (void*)&DequeS + sizeof(DequeS);
    voidf *self_begin = (void*)self;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(self_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _self;
}

//Deque
static void *_deque_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Deque, _self, app);
    struct Deque *self = offsetOf(_self, __Deque);
    self->class = va_arg(*app, void*);
    self->buf_size = 512;
    self->map_size = 2; //必须为2的倍数，不然expand_map无法正确放置start地址
    self->map = allocate(self->map_size * sizeof(void*));

    size_t memb_size = classSz(self->class);
    *self->map = allocate(memb_size * self->buf_size);

    self->start.map_node = self->map;
    self->start.first = self->start.cur = *self->map;
    self->start.last = (char*)(*self->map) + memb_size * self->buf_size;
    self->finish = self->start;

    void *t;
    MetaObject args[2];
    int i = 0;
    while ((t = va_arg(*app, void*)) != VAEND)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _deal_Deque_Args(_self, args, i);
    return _self;
}

static void *_deque_dtor(void *_self)
{
    _self = super_dtor(__Deque, _self);
    struct Deque *self = offsetOf(_self, __Deque);
    _deque_clear(_self);
    size_t memb_size = classSz(self->class);
    deallocate(*self->start.map_node, memb_size * self->buf_size);
    deallocate(self->map, self->map_size * sizeof(void*));
    return _self;
}

static Iterator _deque_begin(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    return construct(__DequeIter, mem, VA(RandomAccessIter), self->class, VA_ANY(self->start, NULL), VAEND);
}

static Iterator _deque_end(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    return construct(__DequeIter, mem, VA(RandomAccessIter), self->class, VA_ANY(self->finish, NULL), VAEND);
}

static const void *_deque_front(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    return self->start.cur;
}

static const void *_deque_back(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    size_t memb_size = classSz(self->class);
    if (self->finish.cur == self->finish.first)
        return *(self->finish.map_node - 1) + (self->buf_size - 1) * memb_size;
    return self->finish.cur - memb_size;
}

static size_t _deque_size(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    size_t memb_size = classSz(self->class);
    struct DequeIter *start = &self->start;
    struct DequeIter *finish = &self->finish;
    if (start->map_node == finish->map_node)
        return ((char*)finish->cur - (char*)start->cur) / memb_size;
    size_t block_diff = finish->map_node - start->map_node - 1;
    long long res = finish->cur - finish->first + start->last - start->cur;
    return res / memb_size + block_diff * self->buf_size;
}

static bool _deque_empty(const void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    return self->start.map_node == self->finish.map_node &&
           self->start.cur == self->finish.cur;
}

static void _deque_push_back(void *_self, const void *_x)
{
    struct Deque *self = offsetOf(_self, __Deque);
    struct DequeIter *finish = &self->finish;
    size_t memb_size = classSz(self->class);
    construct(self->class, finish->cur, _x, VAEND);
    finish->cur = (char*)finish->cur + memb_size;
    if (finish->cur == finish->last) {
        if (finish->map_node == self->map + self->map_size - 1)
            extend_map(self);
        finish->map_node++;
        *finish->map_node = allocate(self->buf_size * memb_size);
        finish->cur = finish->first = *finish->map_node;
        finish->last = (char*)*finish->map_node + memb_size * self->buf_size;
    }
}

static void _deque_push_front(void *_self, const void *_x)
{
    struct Deque *self = offsetOf(_self, __Deque);
    struct DequeIter *start = &self->start;
    size_t memb_size = classSz(self->class);
    if (start->cur == start->first) {
        if (start->map_node == self->map)
            extend_map(self);
        start->map_node--;
        *start->map_node = allocate(self->buf_size * memb_size);
        start->cur = start->last = (char*)*start->map_node + memb_size * self->buf_size;
        start->first = *start->map_node;
    }
    start->cur = (char*)start->cur - memb_size;
    construct(self->class, start->cur, _x, VAEND);
}

static void _deque_pop_back(void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    assert(!_deque_empty(self));
    struct DequeIter *finish = &self->finish;
    size_t memb_size = classSz(self->class);
    if (finish->cur == finish->first) {
        deallocate(*finish->map_node, self->buf_size * memb_size);
        finish->map_node--;
        finish->cur = finish->last = (char*)*finish->map_node + self->buf_size * memb_size;
        finish->first = *finish->map_node;
    }
    finish->cur = (char*)finish->cur - memb_size;
    destroy(finish->cur);
}

static void _deque_pop_front(void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    assert(!_deque_empty(self));
    struct DequeIter *start = &self->start;
    size_t memb_size = classSz(self->class);
    destroy(start->cur);
    start->cur = (char*)start->cur + memb_size;
    if (start->cur == start->last) {
        deallocate(*start->map_node, self->buf_size * memb_size);
        start->map_node++;
        start->cur = start->first = *start->map_node;
        start->last = (char*)*start->map_node + self->buf_size * memb_size;
    }
}

static Iterator _deque_erase(void *_self, Iterator _iter)
{
    assert(classOf(_iter) == __DequeIter && !_deque_empty(_self));
    struct Deque *self = offsetOf(_self, __Deque);
    struct DequeIter *it = offsetOf(_iter, __DequeIter);
    size_t memb_size = classSz(self->class);
    Iterator next = _iter_add(_iter, VA(1));
    offset_t index = dist_aux(&self->start, it, memb_size, self->buf_size);
    if (index < _deque_size(_self) / 2) {
        Iterator front = _deque_begin(_self);
        copy_backward(front, _iter, next);
        _deque_pop_front(_self);
    } else {
        Iterator back = _deque_end(_self);
        copy(next, back, _iter);
        _deque_pop_back(_self);
    }
    *it = self->start;
    _iter_self_add(_iter, VA(index));
    return _iter;
}

static Iterator _deque_insert(void *_self, Iterator _iter, const void *x)
{
    assert(classOf(_iter) == __DequeIter);
    struct Deque *self = offsetOf(_self, __Deque);
    struct DequeIter *it = offsetOf(_iter, __DequeIter);
    if (it->cur == self->start.cur) {
        _deque_push_front(_self, x);
        *it = self->start;
        return _iter;
    } else if (it->cur == self->finish.cur) {
        _deque_push_back(_self, x);
        *it = self->finish;
        _iter_dec(_iter);
        return _iter;
    }
    size_t memb_size = classSz(self->class);
    offset_t index = dist_aux(&self->start, it, memb_size, self->buf_size);
    if (index < _deque_size(_self) / 2) {
        _deque_push_front(_self, VAEND);
        Iterator front1 = _deque_begin(_self);
        Iterator front2 = _iter_add(front1, VA(1));
        *it = self->start;
        _iter_self_add(_iter, VA(index + 1));
        copy(front2, _iter, front1);
        _iter_dec(_iter);
    } else {
        _deque_push_back(_self, VAEND);
        Iterator back1 = _deque_end(_self);
        Iterator back2 = _iter_sub(back1, VA(1));
        *it = self->start;
        _iter_self_add(_iter, VA(index));
        copy_backward(_iter, back2, back1);
    }
    //赋值
    Object obj = _iter_derefer(_iter);
    THIS(obj).assign(x);
    return _iter;
}

static void _deque_swap(void *_self, Deque _d)
{
    struct Deque *self = offsetOf(_self, __Deque);
    struct Deque *D = offsetOf(_d, __Deque);
    assert(self->class == D->class);
    struct Deque tmp = *self;
    *self = *D;
    *D = tmp;
}

static void _deque_clear(void *_self)
{
    struct Deque *self = offsetOf(_self, __Deque);
    size_t memb_size = classSz(self->class);
    for (void **node = self->start.map_node + 1; node < self->finish.map_node; node++) {
        char *end = (char*)*node + self->buf_size;
            for (char *ptr = *node; ptr < end; ptr += memb_size)
                destroy(ptr);
        deallocate(*node, self->buf_size);
    }
    if (self->start.map_node != self->finish.map_node) {
        for (char *ptr = self->finish.first; ptr < (char *) self->finish.cur; ptr += memb_size)
            destroy(ptr);
        for (char *ptr = self->start.cur; ptr < (char *) self->start.last; ptr += memb_size)
            destroy(ptr);
        deallocate(*self->finish.map_node, self->buf_size);
    } else {
        for (char *ptr = self->start.cur; ptr < (char *) self->finish.cur; ptr += memb_size)
            destroy(ptr);
    }
    self->finish = self->start;
}

static void _deque_resize(void *_self, size_t new_size)
{
    size_t n = _deque_size(_self);
    while (n > new_size)
        _deque_pop_back(_self), n--;
    while (n < new_size)
        _deque_push_back(_self, VAEND), n++;
}

static void *_deque_brackets(const void *_self, const void *_x)
{
    struct Deque *self = offsetOf(_self, __Deque);
    size_t memb_size = classSz(self->class);
    long long x;
    if (classOf(_x) == T(Int))
        x = ((Int)_x)->val;
    else
        x = ((Int)(THIS((MetaObject)_x).cast(T(Int))))->val;
    assert(x >= 0 && x < _deque_size(_self));
    struct DequeIter *start = &self->start;
    size_t start_len = ((char*)start->last - (char*)start->cur) /  memb_size;
    if (x < start_len)
        return (char*)start->cur + x * memb_size;
    x -= start_len;
    long long node_index = x / self->buf_size + 1;
    void **node = start->map_node + node_index;
    x %= self->buf_size;
    return *node + x * memb_size;
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->end);
    return class->end(_self);
}

static void* _front(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->front);
    return class->front(_self);
}

static void* _back(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->back);
    return class->back(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _push_back(const void *x)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->push_back);
    return class->push_back(_self, x);
}

static void _push_front(const void *x)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->push_front);
    return class->push_front(_self, x);
}

static void _pop_back(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->pop_back);
    class->pop_back(_self);
}

static void _pop_front(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->pop_front);
    class->pop_front(_self);
}

static Iterator _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->erase);
    return class->erase(_self, iter);
}

static Iterator _insert(Iterator iter, const void *x)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->insert);
    return class->insert(_self, iter, x);
}

static void _resize(size_t new_size)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->resize);
    class->resize(_self, new_size);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Deque _d)
{
    void *_self = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_self), __DequeClass);
    assert(class->swap);
    class->swap(_self, _d);
}
