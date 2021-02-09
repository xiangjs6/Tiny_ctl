//
// Created by xjs on 2020/9/12.
//
#include "../include/tctl_allocator.h"
#include "../include/tctl_deque.h"
#include "../include/tctl_algobase.h"
#include "../include/tctl_int.h"
#include "../include/tctl_uint.h"
#include "../include/auto_release_pool.h"
#include "include/_tctl_deque.h"
#include "include/_tctl_metaclass.h"
#include "include/_tctl_iterator.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
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
static size_t _deque_size(const void *_this);
static const void *_deque_back(const void *_this);
static const void *_deque_front(const void *_this);
static Iterator _deque_end(const void *_this);
static Iterator _deque_begin(const void *_this);
static void *_deque_brackets(const void *_this, FormWO_t _x);
//iterator
static void *_iter_sub(const void *_this, FormWO_t _x);
static void *_iter_add(const void *_this, FormWO_t _x);
static void _iter_assign(void *_this, FormWO_t _x);
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
                           T(Iterator), sizeof(struct DequeIter) + classSz(_Iterator().class),
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
                           Selector, _IteratorS, NULL);
    }
    if (!__DequeClass) {
        __DequeClass = new(T(MetaClass), "DequeClass",
                            T(Class), sizeof(struct DequeClass) + classSz(_Class().class),
                            _MetaClassS->ctor, _dequeclass_ctor, NULL);
    }
    if (!__Deque) {
        __Deque = new(_DequeClass(), "Deque",
                       T(Object), sizeof(struct Deque) + classSz(_Object().class),
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

Form_t _Deque(void)
{
    if (!__Deque)
        initDeque();
    return (Form_t){OBJ, .class = __Deque};
}

Form_t _DequeClass(void)
{
    if (!__DequeClass)
        initDeque();
    return (Form_t){OBJ, .class = __DequeClass};
}

static Form_t _DequeIter(void)
{
    if (!__DequeIter)
        initDeque();
    return (Form_t){OBJ, .class = __DequeIter};
}
//private
static void extend_map(struct Deque *this)
{
    size_t old_len = this->map_size;
    size_t start_offset = this->start.map_node - this->map;
    size_t finish_offset = this->finish.map_node - this->start.map_node;
    this->map_size *= 2;
    this->map = reallocate(this->map, old_len * sizeof(void*), this->map_size * sizeof(void*));
    memmove(this->map + old_len / 2, this->map + start_offset, old_len * sizeof(void*));

    this->start.map_node = this->map + old_len / 2;
    this->finish.map_node = this->start.map_node + finish_offset;
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

static void _dealDequeArgs(void *_this, FormWO_t *args, int n)
{
    struct Deque *this = offsetOf(_this, __Deque);
    if (args->_.class == __Deque) { //复制一个Deque
        struct Deque *d = offsetOf(args->mem, __Deque);
        size_t d_memb_size = d->_t.f == POD ? d->_t.size : classSz(d->_t.class);
        Form_t t = d->_t;
        struct DequeIter it = d->start;
        while (it.cur != d->finish.cur) {
            if (t.f == POD) {
                char (*p)[t.size] = it.cur;
                _deque_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _deque_push_back(_this, FORM_WITH_OBJ(t, it.cur));
            }
            it.cur = (char*)it.cur + d_memb_size;
            if (it.cur == it.last) {
                it.map_node++;
                it.first = it.cur = *it.map_node;
                it.last = (char*)*it.map_node + d_memb_size * d->buf_size;
            }
        }
    } else if (args->_.f == POD || args->_.f == ADDR || args->_.class != _Iterator().class) { //size_type n, T value = T() 构造方法
        unsigned long long nmemb = toUInt(*args);
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _deque_push_back(_this, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _deque_push_back(_this, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(args[1]._.class == _Iterator().class);
        Iterator first = args[0].mem;
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, VA(first), VAEND);
        Iterator last = args[1].mem;
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, VA(last), VAEND);
        Form_t t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            void *obj = THIS(first).derefer();
            if (t.f == POD) {
                char (*p)[t.size] = obj;;
                _deque_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _deque_push_back(_this, FORM_WITH_OBJ(t, obj));
            }
            THIS(first).inc();
        }
        destroy(first);
        destroy(last);
    }
}

//public
//Iterator
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__DequeIter, _this, app);
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    this->cur = NULL;
    this->first = NULL;
    this->last = NULL;
    this->map_node = NULL;
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == ADDR) 
        memcpy(this, t.mem, t._.size);
    else if (t._.f == OBJ)
        _iter_assign(_this, t);
    return _this;
}

static bool _iter_equal(const void *_this, FormWO_t _x)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    assert(classOf(_x.mem) == __DequeIter);
    struct DequeIter *x = offsetOf(_x.mem, __DequeIter);
    return this->cur == x->cur;
}

static int _iter_cmp(const void *_this, FormWO_t _x)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    assert(classOf(_x.mem) == __DequeIter);
    struct DequeIter *x = offsetOf(_x.mem, __DequeIter);
    if (this->map_node < x->map_node)
        return -1;
    else if (this->map_node > x->map_node)
        return 1;
    else {
        if (this->cur < x->cur)
            return -1;
        else if (this->cur > x->cur)
            return 1;
        return 0;
    }
}

static void *_iter_brackets(const void *_this, FormWO_t _x)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    Form_t t = THIS((Iterator)_this).type();
    size_t memb_size = t.f == OBJ ? classSz(t.class) : t.size;
    long long x = toInt(_x);
    size_t front_len = ((char*)this->cur - (char*)this->first) / memb_size;
    size_t back_len = ((char*)this->last - (char*)this->cur) / memb_size;
    size_t buf_size = ((char*)this->last - (char*)this->first) / memb_size;
    if (x >= 0) {
        if (x < back_len)
            return (char*)this->cur + memb_size * x;
        x -= back_len;
        int node_index = x / buf_size + 1;
        void **node = this->map_node + node_index;
        x %= buf_size;
        return *node + x * memb_size;
    } else {
        if (-x < front_len)
            return (char*)this->cur + memb_size * x;
        x += front_len;
        int node_index = x / buf_size - 1; //这是负数
        void **node = this->map_node + node_index;
        x %= buf_size;
        return (char*)*node + x * memb_size + buf_size; //负数要从后向前
    }
}

static void _iter_inc(void *_this)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    Form_t t = THIS((Iterator)_this).type();
    size_t memb_size = t.f == OBJ ? classSz(t.class) : t.size;
    size_t buf_size = ((char*)this->last - (char*)this->first) / memb_size;
    this->cur = (char*)this->cur + memb_size;
    if (this->cur == this->last) {
        this->map_node++;
        this->cur = this->first = *this->map_node;
        this->last = (char*)*this->map_node + buf_size * memb_size;
    }
}

static void _iter_dec(void *_this)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    Form_t t = THIS((Iterator)_this).type();
    size_t memb_size = t.f == OBJ ? classSz(t.class) : t.size;
    size_t buf_size = ((char*)this->last - (char*)this->first) / memb_size;
    if (this->cur == this->first) {
        this->map_node--;
        this->first = *this->map_node;
        this->last = this->cur = (char*)*this->map_node + buf_size * memb_size;
    }
    this->cur = (char*)this->cur - memb_size;
}

static void _iter_assign(void *_this, FormWO_t _x)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    assert(classOf(_x.mem) == __DequeIter);
    struct DequeIter *x = offsetOf(_x.mem, __DequeIter);
    *this = *x;
}

static void _iter_self_add(void *_this, FormWO_t _x)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    Form_t t = THIS((Iterator)_this).type();
    size_t memb_size = t.f == OBJ ? classSz(t.class) : t.size;
    size_t buf_size = ((char*)this->last - (char*)this->first) / memb_size;
    long long x = toInt(_x);
    long long offset = x + (this->cur - this->first) / memb_size;
    if (offset >= 0 && offset < buf_size) {
        this->cur = (char*)this->cur + memb_size * x;
    } else {
        long long node_offset = offset > 0 ?
                                offset / buf_size :
                                -(-offset - 1) / buf_size - 1;
        void **node = this->map_node + node_offset;
        this->map_node = node;
        this->first = *node;
        this->last = (char*)*node + buf_size * memb_size;
        this->cur = (char*)this->first + (offset - node_offset * buf_size) * memb_size;
    }
}

static void _iter_self_sub(void *_this, FormWO_t _x)
{
    long long x = toInt(_x);
    _iter_self_add(_this, VA(-x));
}

static void *_iter_add(const void *_this, FormWO_t _x)
{
    Iterator it = (void*)_this;
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Iterator new_it = new(compose(_DequeIter(), mem), VA(it));
    _iter_self_add(new_it, _x);
    return new_it;
}

static void *_iter_sub(const void *_this, FormWO_t _x)
{
    Iterator it = (void*)_this;
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Iterator new_it = new(compose(_DequeIter(), mem), VA(it));
    _iter_self_sub(new_it, _x);
    return new_it;
}

static void *_iter_derefer(const void *_this)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    return this->cur;
}

static long long _iter_dist(const void *_this, Iterator _it)
{
    struct DequeIter *this = offsetOf(_this, __DequeIter);
    assert(classOf(_it) == __DequeIter);
    Form_t t = THIS((Iterator)_this).type();
    size_t memb_size = t.f == OBJ ? classSz(t.class) : t.size;
    size_t buf_size = ((char*)this->last - (char*)this->first) / memb_size;
    struct DequeIter *it = offsetOf(_it, __DequeIter);
    return dist_aux(this, it, memb_size, buf_size);
}

//DequeClass
static void *_dequeclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__DequeClass, _this, app);
    struct DequeClass *this = offsetOf(_this, __DequeClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&DequeS + sizeof(DequeS._);
    voidf *end = (void*)&DequeS + sizeof(DequeS);
    voidf *this_begin = (void*)this;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(this_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _this;
}

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
    this->map_size = 2; //必须为2的倍数，不然expand_map无法正确放置start地址
    this->map = allocate(this->map_size * sizeof(void*));

    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    *this->map = allocate(memb_size * this->buf_size);

    this->start.map_node = this->map;
    this->start.first = this->start.cur = *this->map;
    this->start.last = (char*)(*this->map) + memb_size * this->buf_size;
    this->finish = this->start;

    FormWO_t args[2];
    int i = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _dealDequeArgs(_this, args, i);
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
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    return new(compose(_DequeIter(), mem), VA(t, RandomAccessIter, VA_ADDR(this->start)));
}

static Iterator _deque_end(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    void *mem = ARP_MallocARelDtor(classSz(__DequeIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    return new(compose(_DequeIter(), mem), VA(t, RandomAccessIter, VA_ADDR(this->finish)));
}

static const void *_deque_front(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    return this->start.cur;
}

static const void *_deque_back(const void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (this->finish.cur == this->finish.first)
        return *(this->finish.map_node - 1) + (this->buf_size - 1) * memb_size;
    return this->finish.cur - memb_size;
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
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(finish->cur, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(finish->cur, &_x.mem, memb_size);
        else if (_x._.f == END)
            memset(finish->cur, 0, memb_size);
    } else {
        construct(this->_t, finish->cur, _x, VAEND);
    }
    finish->cur = (char*)finish->cur + memb_size;
    if (finish->cur == finish->last) {
        if (finish->map_node == this->map + this->map_size - 1)
            extend_map(this);
        finish->map_node++;
        *finish->map_node = allocate(this->buf_size * memb_size);
        finish->cur = finish->first = *finish->map_node;
        finish->last = (char*)*finish->map_node + memb_size * this->buf_size;
    }
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
        else if (_x._.f == END)
            memset(start->cur, 0, memb_size);
    } else {
        construct(this->_t, start->cur, _x, VAEND);
    }
}

static void _deque_pop_back(void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    struct DequeIter *finish = &this->finish;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (finish->cur == finish->first) {
        deallocate(*finish->map_node, this->buf_size * memb_size);
        finish->map_node--;
        finish->cur = finish->last = (char*)*finish->map_node + this->buf_size * memb_size;
        finish->first = *finish->map_node;
    }
    finish->cur = (char*)finish->cur - memb_size;
    if (this->_t.f == OBJ)
        destroy(finish->cur);
}

static void _deque_pop_front(void *_this)
{
    struct Deque *this = offsetOf(_this, __Deque);
    assert(!_deque_empty(this));
    struct DequeIter *start = &this->start;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (this->_t.f == OBJ)
        destroy(start->cur);
    start->cur = (char*)start->cur + memb_size;
    if (start->cur == start->last) {
        deallocate(*start->map_node, this->buf_size * memb_size);
        start->map_node++;
        start->cur = start->first = *start->map_node;
        start->last = (char*)*start->map_node + this->buf_size * memb_size;
    }
}

static Iterator _deque_erase(void *_this, Iterator _iter)
{
    assert(classOf(_iter) == __DequeIter && !_deque_empty(_this));
    struct Deque *this = offsetOf(_this, __Deque);
    struct DequeIter *it = offsetOf(_iter, __DequeIter);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    Iterator next = _iter_add(_iter, VA(1));
    offset_t index = dist_aux(&this->start, it, memb_size, this->buf_size);
    if (index < _deque_size(_this) / 2) {
        Iterator front = _deque_begin(_this);
        copy_backward(front, _iter, next);
        _deque_pop_front(_this);
    } else {
        Iterator back = _deque_end(_this);
        copy(next, back, _iter);
        _deque_pop_back(_this);
    }
    *it = this->start;
    _iter_self_add(_iter, VA(index));
    return _iter;
}

static Iterator _deque_insert(void *_this, Iterator _iter, FormWO_t x)
{
    assert(classOf(_iter) == __DequeIter);
    struct Deque *this = offsetOf(_this, __Deque);
    struct DequeIter *it = offsetOf(_iter, __DequeIter);
    if (it->cur == this->start.cur) {
        _deque_push_front(_this, x);
        *it = this->start;
        return _iter;
    } else if (it->cur == this->finish.cur) {
        _deque_push_back(_this, x);
        *it = this->finish;
        _iter_dec(_iter);
        return _iter;
    }
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    offset_t index = dist_aux(&this->start, it, memb_size, this->buf_size);
    if (index < _deque_size(_this) / 2) {
        _deque_push_front(_this, VAEND);
        Iterator front1 = _deque_begin(_this);
        Iterator front2 = _iter_add(front1, VA(1));
        *it = this->start;
        _iter_self_add(_iter, VA(index + 1));
        copy(front2, _iter, front1);
        _iter_dec(_iter);
    } else {
        _deque_push_back(_this, VAEND);
        Iterator back1 = _deque_end(_this);
        Iterator back2 = _iter_sub(back1, VA(1));
        *it = this->start;
        _iter_self_add(_iter, VA(index));
        copy_backward(_iter, back2, back1);
    }
    //赋值
    if (this->_t.f == OBJ) {
        Object obj = _iter_derefer(_iter);
        THIS(obj).assign(x);
    } else {
        assert(x._.f != OBJ);
        if (x._.f == POD)
            memcpy(_iter_derefer(_iter), &x.mem, this->_t.size);
        else if (x._.f == ADDR)
            memcpy(_iter_derefer(_iter), x.mem, this->_t.size);
        else if (x._.f == END)
            memset(_iter_derefer(_iter), 0, memb_size);
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
        if (this->_t.f == OBJ) {
            for (char *ptr = *node; ptr < end; ptr += memb_size)
                destroy(ptr);
        }
        deallocate(*node, this->buf_size);
    }
    if (this->start.map_node != this->finish.map_node) {
        if (this->_t.f == OBJ) {
            for (char *ptr = this->finish.first; ptr < (char *) this->finish.cur; ptr += memb_size)
                destroy(ptr);
            for (char *ptr = this->start.cur; ptr < (char *) this->start.last; ptr += memb_size)
                destroy(ptr);
        }
        deallocate(*this->finish.map_node, this->buf_size);
    } else {
        if (this->_t.f == OBJ) {
            for (char *ptr = this->start.cur; ptr < (char *) this->finish.cur; ptr += memb_size)
                destroy(ptr);
        }
    }
    this->finish = this->start;
}

static void _deque_resize(void *_this, size_t new_size)
{
    struct Deque *this = offsetOf(_this, __Deque);
    size_t n = _deque_size(_this);
    while (n > new_size)
        _deque_pop_back(_this), n--;
    while (n < new_size)
        _deque_push_back(_this, VAEND), n++;
}

static void *_deque_brackets(const void *_this, FormWO_t _x)
{
    struct Deque *this = offsetOf(_this, __Deque);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    long long x = toInt(_x);
    assert(x >= 0 && x < _deque_size(_this));
    struct DequeIter *start = &this->start;
    size_t start_len = ((char*)start->last - (char*)start->cur) /  memb_size;
    if (x < start_len)
        return (char*)start->cur + x * memb_size;
    x -= start_len;
    int node_index = x / this->buf_size + 1;
    void **node = start->map_node + node_index;
    x %= this->buf_size;
    return *node + x * memb_size;
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->end);
    return class->end(_this);
}

static void* _front(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->front);
    return class->front(_this);
}

static void* _back(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->back);
    return class->back(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _push_back(FormWO_t x)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->push_back);
    return class->push_back(_this, x);
}

static void _push_front(FormWO_t x)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->push_front);
    return class->push_front(_this, x);
}

static void _pop_back(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->pop_back);
    class->pop_back(_this);
}

static void _pop_front(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->pop_front);
    class->pop_front(_this);
}

static Iterator _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->erase);
    return class->erase(_this, iter);
}

static Iterator _insert(Iterator iter, FormWO_t x)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->insert);
    return class->insert(_this, iter, x);
}

static void _resize(size_t new_size)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->resize);
    class->resize(_this, new_size);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Deque _d)
{
    void *_this = pop_this();
    const struct DequeClass *class = offsetOf(classOf(_this), __DequeClass);
    assert(class->swap);
    class->swap(_this, _d);
}
