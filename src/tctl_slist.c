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
    void (*remove)(void *_this, FormWO_t x);
    void (*unique)(void *_this);
    void (*splice_after)(void *_this, Iterator _position, Slist l, va_list *app);
    void (*merge)(void *_this, Slist l, Compare cmp);
    void (*reverse)(void *_this);
    void (*sort)(void *_this, Compare cmp);
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
static void _remove(FormWO_t x);
static void _unique(void);
static void _splice_after(Iterator position, Slist l, ...);
static void _merge(Slist l, Compare cmp);
static void _reverse(void);
static void _sort(Compare cmp);
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
static void _slist_remove(void *_this, FormWO_t x);
static void _slist_unique(void *_this);
static void _slist_splice_after(void *_this, Iterator _position, Slist l, va_list *app);
static void _slist_merge(void *_this, Slist l, Compare cmp);
static void _slist_reverse(void *_this);
static void _slist_sort(void *_this, Compare cmp);
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
    _remove,
    _unique,
    _splice_after,
    _merge,
    _reverse,
    _sort,
    _clear,
    _swap
};
const struct SlistSelector *_SlistS = NULL;

static void initSlist(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
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
                       SlistS.remove, _slist_remove,
                       SlistS.unique, _slist_unique,
                       SlistS.splice_after, _slist_splice_after,
                       SlistS.merge, _slist_merge,
                       SlistS.reverse, _slist_reverse,
                       SlistS.sort, _slist_sort,
                       SlistS.clear, _slist_clear,
                       SlistS.swap, _slist_swap,
                       Selector, _SlistS, NULL);
    }
}

Form_t _Slist(void)
{
    if (!__Slist)
        initSlist();
    return (Form_t){OBJ, .class = __Slist};
}

Form_t _SlistClass(void)
{
    if (!__SlistClass)
        initSlist();
    return (Form_t){OBJ, .class = __SlistClass};
}

static Form_t _SlistIter(void)
{
    if (!__SlistIter)
        initSlist();
    return (Form_t){OBJ, .class = __SlistIter};
}

//private
static void _dealSlistArgs(void *_this, FormWO_t *args, int n)
{
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
    _slist_clear(_this);
    return _this;
}

static Iterator _slist_begin(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    return construct(_SlistIter(), mem, VA(t, ForwardIter, this->_head.nxt));
}

static Iterator _slist_end(const void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    return construct(_SlistIter(), mem, VA(t, ForwardIter, NULL));
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
    it->node->nxt = node->nxt;
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

static void _slist_remove(void *_this, FormWO_t x)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct SlistNode *node = &this->_head;
    if (this->_t.f == POD) {
        size_t memb_size = this->_t.size;
        assert(x._.class != OBJ);
        void *mem = x._.f == POD ? &x.mem : x.mem;
        while (node->nxt)
        {
            if (!memcmp(node->nxt->data, mem, memb_size)) {
                struct SlistNode *tmp = node->nxt;
                node->nxt = node->nxt->nxt;
                deallocate(tmp, memb_size + sizeof(struct SlistNode));
                break;
            }
        }
    } else {
        while (node->nxt)
        {
            Object obj = (Object)node->nxt->data;
            if (THIS(obj).equal(x)) {
                struct SlistNode *tmp = node->nxt;
                node->nxt = node->nxt->nxt;
                destroy(node->data);
                deallocate(tmp, classSz(this->_t.class) + sizeof(struct SlistNode));
                break;
            }
            node = node->nxt;
        }
    }
}

static void _slist_unique(void *_this)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct SlistNode *node = this->_head.nxt;
    if (!node)
        return;
    struct SlistNode *next_node = node->nxt;
    if (this->_t.f == POD) {
        size_t memb_size = this->_t.size;
        while (next_node)
        {
            if (!memcmp(next_node->data, node->data, memb_size)) {
                node->nxt = node->nxt->nxt;
                deallocate(next_node, memb_size + sizeof(struct SlistNode));
                next_node = node;
            } else
                node = next_node;
            next_node = next_node->nxt;
        }
    } else {
        FormWO_t x = FORM_WITH_OBJ(this->_t);
        size_t memb_size = classSz(this->_t.class);
        while (next_node)
        {
            Object obj = (Object)node->data;
            x.mem = next_node->data;
            if (THIS(obj).equal(x)) {
                node->nxt = node->nxt->nxt;
                destroy(next_node->data);
                deallocate(next_node, memb_size + sizeof(struct SlistNode));
                next_node = node;
            } else
                node = next_node;
            next_node = next_node->nxt;
        }
    }
}

static void _slist_splice_after(void *_this, Iterator _position, Slist l, va_list *app)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct Slist *L = offsetOf(l, __Slist);
    assert(L->_t.f == this->_t.f && L->_t.class == this->_t.class);
    FormWO_t t;
    FormWO_t args[2];
    int n = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(n < 2);
        args[n++] = t;
    }
    assert(classOf(_position) == __SlistIter);
    struct SlistNode *pos_node = ((struct SlistIter*)offsetOf(_position, __SlistIter))->node;
    struct SlistNode *first_node = NULL;
    struct SlistNode *last_node = NULL;
    if (n == 0) {
        if (_slist_empty(L))
            return;
        first_node = L->_head.nxt;
    } else if (n == 1) {
        assert(args[0]._.f == OBJ);
        first_node = ((struct SlistIter*)offsetOf(args[0].mem, __SlistIter))->node;
        last_node = first_node->nxt;
        if (pos_node == first_node || pos_node == last_node)
            return;
    } else {
        assert(args[0]._.f == OBJ);
        assert(args[1]._.f == OBJ);
        first_node = ((struct SlistIter*)offsetOf(args[0].mem, __SlistIter))->node;
        last_node = ((struct SlistIter*)offsetOf(args[1].mem, __SlistIter))->node;
        if (first_node == last_node)
            return;
    }

    assert(last_node);
    if (pos_node != first_node && pos_node != last_node) {
        struct SlistNode *_first = first_node->nxt;
        struct SlistNode *_after = pos_node->nxt;
        first_node->nxt = last_node->nxt;
        pos_node->nxt = _first;
        last_node->nxt = _after;
    }
}

static void _slist_merge(void *_this, Slist l, Compare cmp)
{
    struct Slist *this = offsetOf(_this, __Slist);
    struct Slist *L = offsetOf(l, __Slist);
    Form_t t1 = this->_t;
    if (t1.f == POD)
        t1.f = ADDR;
    Form_t t2 = L->_t;
    if (t2.f == POD)
        t2.f = ADDR;

    struct SlistNode *node = &this->_head;
    while (node->nxt && L->_head.nxt)
    {
        FormWO_t v1 = FORM_WITH_OBJ(t1, node->nxt->data);
        FormWO_t v2 = FORM_WITH_OBJ(t2, L->_head.nxt->data);
        if (cmp(v1, v2) > 0) {
            struct SlistNode *first_node = &L->_head;
            struct SlistNode *last_node = L->_head.nxt;
            if (node != first_node) {
                struct SlistNode *_first = first_node->nxt;
                struct SlistNode *_after = node->nxt;
                first_node->nxt = last_node->nxt;
                node->nxt = _first;
                last_node->nxt = _after;
            }
        }
        node = node->nxt;
    }
    if (L->_head.nxt) {
        node->nxt = L->_head.nxt;
        L->_head.nxt = NULL;
    }
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

static void _slist_sort(void *_this, Compare cmp)
{
    struct Slist *this = offsetOf(_this, __Slist);
    if (this->_head.nxt == NULL || this->_head.nxt->nxt == NULL)
        return;
    Slist _carry = new(T(Slist), VA(this->_t));
    struct Slist *carry = offsetOf(_carry, __Slist);
    Slist counter[64];
    int fill = 0;
    while (this->_head.nxt)
    {
        struct SlistNode *node = &carry->_head;
        struct SlistNode *first_node = &this->_head;
        struct SlistNode *last_node = this->_head.nxt;
        if (node != first_node) {
            struct SlistNode *_first = first_node->nxt;
            struct SlistNode *_after = node->nxt;
            first_node->nxt = last_node->nxt;
            node->nxt = _first;
            last_node->nxt = _after;
        }
        int i = 0;
        while (i < fill && !THIS(counter[i]).empty())
        {
            THIS(counter[i]).merge(_carry, cmp);
            THIS(_carry).swap(counter[i++]);
        }
        if (i == fill)
            counter[fill++] = new(T(Slist), VA(this->_t));
        THIS(counter[i]).swap(_carry);
    }
    for (int i = 1; i < fill; i++) {
        THIS(counter[i]).merge(counter[i - 1], cmp);
        delete(counter[i - 1]);
    }
    _slist_swap(_this, counter[fill - 1]);
    delete(counter[fill - 1]);
    delete(_carry);
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

static void _remove(FormWO_t x)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->remove);
    class->remove(_this, x);
}

static void _unique(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->unique);
    class->unique(_this);
}

static void _splice_after(Iterator position, Slist l, ...)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->splice_after);
    va_list ap;
    va_start(ap, l);
    class->splice_after(_this, position, l, &ap);
    va_end(ap);
}

static void _merge(Slist l, Compare cmp)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->merge);
    class->merge(_this, l, cmp);
}

static void _reverse(void)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->reverse);
    class->reverse(_this);
}

static void _sort(Compare cmp)
{
    void *_this = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_this), __SlistClass);
    assert(class->sort);
    class->sort(_this, cmp);
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
