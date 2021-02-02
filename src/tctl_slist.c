//
// Created by xjs on 2020/9/29.
//

#include "include/_tctl_slist.h"
#include "include/_tctl_iterator.h"
#include "include/_tctl_metaclass.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_uint.h"
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, SLIST

struct SlistNode {
    struct SlistNode *nxt;
    byte data[0];
};

struct SlistClass {
    Iterator (*begin)(void *_this);
    Iterator (*end)(void *_this);
    void* (*front)(void *_this);
    size_t (*size)(void *_this);
    bool (*empty)(void *_this);
    void (*push_front)(void *_this, FormWO_t x);
    void (*pop_front)(void *_this);
    Iterator (*erase_after)(void *_this, Iterator iter);
    Iterator (*insert_after)(void *_this, Iterator iter, FormWO_t x);
    void (*reverse)(void *_this);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Slist l);
};

struct Slist {
    Form_t _t;
    struct SlistNode _head;
};

struct SlistIter {
    struct SlistNode *node;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static void *_front(void);
static size_t _size(void);
static bool _empty(void);
static void _push_front(FormWO_t x);
static void _pop_front(void);
static Iterator _erase_after(Iterator iter);
static Iterator _insert_after(Iterator iter, FormWO_t x);
static void _reverse(void);
static void _clear(void);
static void _swap(Slist l);
//slistclass
static void *_slistclass_ctor(void *_this, va_list *app);
//slist
static void *_slist_ctor(void *_this, va_list *app);
static void *_slist_dtor(void *_this);
static Iterator _slist_begin(const void *_this);
static Iterator _slist_end(const void *_this);
static void *_slist_front(const void *_this);
static size_t _slist_size(const void *_this);
static bool _slist_empty(const void *_this);
static void _slist_push_front(void *_this, FormWO_t x);
static void _slist_pop_front(void *_this);
static Iterator _slist_erase_after(void *_this, Iterator iter);
static Iterator _slist_insert_after(void *_this, Iterator iter, FormWO_t x);
static void _slist_reverse(void *_this);
static void _slist_clear(void *_this);
static void _slist_swap(void *_this, Slist _l);
//iterator
static void _iter_assign(void *_this, FormWO_t _x);
static void _iter_inc(void *_this);
static bool _iter_equal(const void *_this, FormWO_t _x);
static void *_iter_ctor(void *_this, va_list *app);
static void *_iter_derefer(const void *_this);
//init
static const void *__SlistIter = NULL;
static const void *__Slist = NULL;
static const void *__SlistClass = NULL;
volatile static struct SlistSelector SlistS = {
    {},
    _begin,
    _end,
    _front,
    _size,
    _empty,
    _push_front,
    _pop_front,
    _erase_after,
    _insert_after,
    _reverse,
    _clear,
    _swap
};
const struct SlistSelector *_SlistS = NULL;

void initSlist(void)
{
    initClass();
    initIterator();
    if (!_SlistS) {
        _SlistS = (void*)&SlistS;
        memcpy((void*)&SlistS, _ClassS, sizeof(*_ClassS));
    }
    if (!__SlistIter) {
        __SlistIter = new(_IteratorClass(), "SlistIter",
                         T(Iterator), sizeof(struct SlistIter) + classSz(_Iterator().class),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         Selector, _IteratorS, NULL);
    }
    if (!__SlistClass) {
        __SlistClass = new(T(MetaClass), "SlistClass",
                           T(Class), sizeof(struct SlistClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _slistclass_ctor, NULL);
    }
    if (!__Slist) {
        __Slist = new(_SlistClass(), "Slist",
                       T(Object), sizeof(struct Slist) + classSz(_Object().class),
                       _MetaClassS->ctor, _slist_ctor,
                       _MetaClassS->dtor, _slist_dtor,
                       SlistS.begin, _slist_begin,
                       SlistS.end, _slist_end,
                       SlistS.front, _slist_front,
                       SlistS.size, _slist_size,
                       SlistS.empty, _slist_empty,
                       SlistS.push_front, _slist_push_front,
                       SlistS.pop_front, _slist_pop_front,
                       SlistS.erase_after, _slist_erase_after,
                       SlistS.insert_after, _slist_insert_after,
                       SlistS.reverse, _slist_reverse,
                       SlistS.clear, _slist_clear,
                       SlistS.swap, _slist_swap,
                       Selector, _SlistS, NULL);
    }
}

Form_t _Slist(void)
{
    Form_t t = {OBJ, .class = __Slist};
    return t;
}

Form_t _SlistClass(void)
{
    Form_t t = {OBJ, .class = __SlistClass};
    return t;
}

static Form_t _SlistIter(void)
{
    Form_t t = {OBJ, .class = __SlistIter};
    return t;
}

//private
static void _dealSlistArgs(void *_this, FormWO_t *args, int n)
{
    struct Slist *this = offsetOf(_this, __Slist);
    if (args->_.class == __Slist) { //复制一个List
        struct Slist *L = offsetOf(args->mem, __Slist);
        struct SlistNode *node = L->_head.nxt;
        Form_t t = L->_t;
        while (node)
        {
            void *obj = node->data;
            if (t.f == POD) {
                char (*p)[t.size] = obj;
                _slist_push_front(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _slist_push_front(_this, FORM_WITH_OBJ(t, obj));
            }
            node = node->nxt;
        }
        _slist_reverse(_this);
    } else if (args->_.f == POD || args->_.f == ADDR || args->_.class != _Iterator().class) { //size_type n, T value = T() 构造方法
        unsigned long long nmemb = toUInt(*args);
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _slist_push_front(_this, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _slist_push_front(_this, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(args[1]._.class == _Iterator().class); //因为上面的if已经检测过args[0]
        Iterator first = args[0].mem;
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, VA(first));
        Iterator last = args[1].mem;
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, VA(last));
        Form_t t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            void *obj = THIS(first).derefer();
            if (t.f == POD) {
                char (*p)[t.size] = obj;
                _slist_push_front(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _slist_push_front(_this, FORM_WITH_OBJ(t, obj));
            }
            THIS(first).inc();
        }
        _slist_reverse(_this);
        destroy(first);
        destroy(last);
    }
}

//public
//Iterator
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__SlistIter, _this, app);
    struct SlistIter *this = offsetOf(_this, __SlistIter);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == OBJ) {
        if (t._.class == _Iterator().class) {
            assert(classOf(t.mem) == __SlistIter);
            struct SlistIter *it = offsetOf(t.mem, __SlistIter);
            *this = *it;
        }
    } else if (t._.f == POD) {
        this->node = t.mem;
    } else {
        this->node = *(struct SlistNode**)t.mem;
    }
    return _this;
}

static bool _iter_equal(const void *_this, FormWO_t _x)
{
    const struct SlistIter *this = offsetOf(_this, __SlistIter);
    const struct SlistIter *x = offsetOf(_x.mem, __SlistIter);
    return this->node == x->node;
}

static void _iter_inc(void *_this)
{
    struct SlistIter *this = offsetOf(_this, __SlistIter);
    assert(this->node);
    this->node = this->node->nxt;
}

static void _iter_assign(void *_this, FormWO_t _x)
{
    struct SlistIter *this = offsetOf(_this, __SlistIter);
    struct SlistIter *x = offsetOf(_x.mem, __SlistIter);
    this->node = x->node;
}

static void *_iter_derefer(const void *_this)
{
    struct SlistIter *this = offsetOf(_this, __SlistIter);
    return this->node->data;
}

//SlistClass
static void *_slistclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__SlistClass, _this, app);
    struct ListClass *this = offsetOf(_this, __SlistClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&SlistS + sizeof(SlistS._);
    voidf *end = (void*)&SlistS + sizeof(SlistS);
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

//Slist
static void *_slist_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Slist, _this, app);
    struct Slist *this = offsetOf(_this, __Slist);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f >= FORM);
    t._.f -=FORM;
    this->_t = t._;
    this->_head.nxt = NULL;
    FormWO_t args[2];
    int i = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _dealSlistArgs(_this, args, i);
    return _this;
}

static void *_slist_dtor(void *_this)
{
    _this = super_dtor(__Slist, _this);
    struct Slist *this = offsetOf(_this, __Slist);
    _slist_clear(_this);
    return _this;
}

static Iterator _slist_begin(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    return construct(_SlistIter(), mem, VA(this->_t, ForwardIter, this->_head.nxt));
}

static Iterator _slist_end(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    return construct(_SlistIter(), mem, VA(this->_t, ForwardIter, NULL));
}

static void *_slist_front(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    return this->_head.nxt->data;
}

static size_t _slist_size(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct SlistNode *node = this->_head.nxt;
    size_t size = 0;
    while (node != NULL)
    {
        size++;
        node = node->nxt;
    }
    return size;
}

static bool _slist_empty(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    return this->_head.nxt == NULL;
}

static void _slist_push_front(void *_this, FormWO_t _x)
{
    struct Slist *this = offsetOf(_this, __Slist);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    struct SlistNode *new_node = allocate(sizeof(struct SlistNode) + memb_size);
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(new_node->data, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(new_node->data, &_x.mem, memb_size);
        else if (_x._.f == END)
            memset(new_node->data, 0, memb_size);
    } else {
        construct(this->_t, new_node->data, _x, VAEND);
    }
    new_node->nxt = this->_head.nxt;
    this->_head.nxt = new_node;
}

static void _slist_pop_front(void *_this)
{
    assert(!_slist_empty(_this));
    struct Slist *this = offsetOf(_this, __Slist);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    struct SlistNode *node = this->_head.nxt;
    if (this->_t.f == OBJ)
        destroy(node->data);
    this->_head.nxt = this->_head.nxt->nxt;
    deallocate(node, memb_size + sizeof(struct SlistNode));
}

static Iterator _slist_erase_after(void *_this, Iterator iter)
{
    struct Slist *this = offsetOf(_this, __Slist);
    assert(classOf(iter) == __SlistIter);
    struct SlistIter *it = offsetOf(iter, __SlistIter);
    struct SlistNode *node = it->node->nxt;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    assert(it->node);
    it->node->nxt =  node->nxt;
    if (this->_t.f == OBJ)
        destroy(node->data);
    deallocate(node, memb_size + sizeof(struct SlistNode));
    return iter;
}

static Iterator _slist_insert_after(void *_this, Iterator iter, FormWO_t _x)
{
    struct Slist *this = offsetOf(_this, __Slist);
    assert(classOf(iter) == __SlistIter);
    struct SlistIter *it = offsetOf(iter, __SlistIter);
    struct SlistNode *node = it->node->nxt;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    struct SlistNode *new_node = allocate(sizeof(struct SlistNode) + memb_size);
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(new_node->data, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(new_node->data, &_x.mem, memb_size);
        else if (_x._.f == END)
            memset(new_node->data, 0, memb_size);
    } else {
        construct(this->_t, new_node->data, _x, VAEND);
    }
    new_node->nxt = it->node->nxt;
    it->node->nxt = new_node;
    return iter;
}

static void _slist_reverse(void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    if (!this->_head.nxt || !this->_head.nxt->nxt)
        return;
    struct SlistNode *first = this->_head.nxt;
    struct SlistNode *next = first->nxt;
    this->_head.nxt = NULL;
    while (next)
    {
        first->nxt = this->_head.nxt;
        this->_head.nxt = first;
        first = next;
        next = first->nxt;
    }
    first->nxt = this->_head.nxt;
    this->_head.nxt = first;
}

static void _slist_clear(void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    while (this->_head.nxt != NULL)
        _slist_pop_front(_this);
}

static void _slist_swap(void *_this, Slist _l)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct Slist *l = offsetOf(_l, __Slist);
    struct SlistNode *temp = this->_head.nxt;
    this->_head.nxt = l->_head.nxt;
    l->_head.nxt = temp;
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->end);
    return class->end(_this);
}

static void *_front(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->front);
    return class->front(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _push_front(FormWO_t x)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->push_front);
    class->push_front(_this, x);
}

static void _pop_front(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->pop_front);
    class->pop_front(_this);
}

static Iterator _erase_after(Iterator iter)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->erase_after);
    return class->erase_after(_this, iter);
}

static Iterator _insert_after(Iterator iter, FormWO_t x)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->insert_after);
    return class->insert_after(_this, iter, x);
}

static void _reverse(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->reverse);
    class->reverse(_this);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Slist l)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->clear);
    class->swap(_this, l);
}
