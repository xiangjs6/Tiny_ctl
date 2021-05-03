//
// Created by xjs on 2020/9/29.
//

#include "include/_tctl_slist.h"
#include "include/_tctl_iterator.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_int.h"
#include "../include/tctl_any.h"
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, SLIST, ANY, INT

#define DATA(node) ((void*)((char*)&(node) + sizeof(node)))

struct SlistNode {
    struct SlistNode *nxt;
};

struct SlistClass {
    Iterator (*begin)(void *_self);
    Iterator (*end)(void *_self);
    void* (*front)(void *_self);
    size_t (*size)(void *_self);
    bool (*empty)(void *_self);
    void (*push_front)(void *_self, const void *x);
    void (*pop_front)(void *_self);
    Iterator (*erase_after)(void *_self, Iterator iter);
    Iterator (*insert_after)(void *_self, Iterator iter, const void *x);
    void (*remove)(void *_self, const void *x);
    void (*unique)(void *_self);
    void (*splice_after)(void *_self, Iterator _position, Slist l, va_list *app);
    void (*merge)(void *_self, Slist l, Compare cmp);
    void (*reverse)(void *_self);
    void (*sort)(void *_self, Compare cmp);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Slist l);
};

struct Slist {
    void *class;
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
static void _push_front(const void *x);
static void _pop_front(void);
static Iterator _erase_after(Iterator iter);
static Iterator _insert_after(Iterator iter, const void *x);
static void _remove(const void *x);
static void _unique(void);
static void _splice_after(Iterator position, Slist l, ...);
static void _merge(Slist l, Compare cmp);
static void _reverse(void);
static void _sort(Compare cmp);
static void _clear(void);
static void _swap(Slist l);
//slistclass
static void *_slistclass_ctor(void *_self, va_list *app);
//slist
static void *_slist_ctor(void *_self, va_list *app);
static void *_slist_dtor(void *_self);
static Iterator _slist_begin(const void *_self);
static Iterator _slist_end(const void *_self);
static void *_slist_front(const void *_self);
static size_t _slist_size(const void *_self);
static bool _slist_empty(const void *_self);
static void _slist_push_front(void *_self, const void *x);
static void _slist_pop_front(void *_self);
static Iterator _slist_erase_after(void *_self, Iterator iter);
static Iterator _slist_insert_after(void *_self, Iterator iter, const void *x);
static void _slist_remove(void *_self, const void *x);
static void _slist_unique(void *_self);
static void _slist_splice_after(void *_self, Iterator _position, Slist l, va_list *app);
static void _slist_merge(void *_self, Slist l, Compare cmp);
static void _slist_reverse(void *_self);
static void _slist_sort(void *_self, Compare cmp);
static void _slist_clear(void *_self);
static void _slist_swap(void *_self, Slist _l);
//iterator
static void _iter_assign(void *_self, const void *_x);
static void _iter_inc(void *_self);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
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
                         T(Iterator), sizeof(struct SlistIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         Selector, _IteratorS, NULL);
    }
    if (!__SlistClass) {
        __SlistClass = new(T(MetaClass), "SlistClass",
                           T(Class), sizeof(struct SlistClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _slistclass_ctor, NULL);
    }
    if (!__Slist) {
        __Slist = new(_SlistClass(), "Slist",
                       T(Object), sizeof(struct Slist) + classSz(T(Object)),
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

const void *_Slist(void)
{
    if (!__Slist)
        initSlist();
    return __Slist;
}

const void *_SlistClass(void)
{
    if (!__SlistClass)
        initSlist();
    return __SlistClass;
}

static const void *_SlistIter(void)
{
    if (!__SlistIter)
        initSlist();
    return __SlistIter;
}

//private
static void _deal_Slist_Args(void *_self, MetaObject *args, int n)
{
    if (classOf(*args) == __Slist) { //复制一个List
        struct Slist *L = offsetOf(*args, __Slist);
        struct SlistNode *node = L->_head.nxt;
        while (node)
        {
            void *obj = DATA(*node);
            _slist_push_front(_self, obj);
            node = node->nxt;
        }
        _slist_reverse(_self);
    } else if (classOf(*args) == T(Any)) { //size_type n, T value = T() 构造方法
        Any any = (Any)*args;
        Int nmemb = THIS(any).cast(T(Int));
        if (n == 1) {
            for (size_t i = 0; i < nmemb->val; i++)
                _slist_push_front(_self, VAEND);
        } else {
            for (size_t i = 0; i < nmemb->val; i++)
                _slist_push_front(_self, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(class_check(args[0], T(Iterator)));
        assert(class_check(args[1], T(Iterator))); //因为上面的if已经检测过args[0]
        Iterator first = (Iterator)args[0];
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, first, VAEND);
        Iterator last = (Iterator)args[1];
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, last, VAEND);
        while (!THIS(first).equal(last))
        {
            void *obj = THIS(first).derefer();
            _slist_push_front(_self, obj);
            THIS(first).inc();
        }
        _slist_reverse(_self);
        destroy(first);
        destroy(last);
    }
}

//public
//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__SlistIter, _self, app);
    struct SlistIter *self = offsetOf(_self, __SlistIter);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __SlistIter) {
        struct SlistIter *it = offsetOf(t, __SlistIter);
        *self = *it;
    } else {
        assert(classOf(t) == T(Any));
        Any any = (Any)t;
        assert(sizeof(void*) == THIS(any).size());
        memcpy(&self->node, THIS(any).value(), sizeof(void*));
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    const struct SlistIter *self = offsetOf(_self, __SlistIter);
    const struct SlistIter *x = offsetOf(_x, __SlistIter);
    return self->node == x->node;
}

static void _iter_inc(void *_self)
{
    struct SlistIter *self = offsetOf(_self, __SlistIter);
    assert(self->node);
    self->node = self->node->nxt;
}

static void _iter_assign(void *_self, const void *_x)
{
    struct SlistIter *self = offsetOf(_self, __SlistIter);
    struct SlistIter *x = offsetOf(_x, __SlistIter);
    self->node = x->node;
}

static void *_iter_derefer(const void *_self)
{
    struct SlistIter *self = offsetOf(_self, __SlistIter);
    return DATA(*self->node);
}

//SlistClass
static void *_slistclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__SlistClass, _self, app);
    struct ListClass *self = offsetOf(_self, __SlistClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&SlistS + sizeof(SlistS._);
    voidf *end = (void*)&SlistS + sizeof(SlistS);
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

//Slist
static void *_slist_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Slist, _self, app);
    struct Slist *self = offsetOf(_self, __Slist);
    self->class = va_arg(*app, void*);
    self->_head.nxt = NULL;
    MetaObject args[2];
    void *t;
    int i = 0;
    while ((t = va_arg(*app, void*)) != VAEND)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _deal_Slist_Args(_self, args, i);
    return _self;
}

static void *_slist_dtor(void *_self)
{
    _self = super_dtor(__Slist, _self);
    _slist_clear(_self);
    return _self;
}

static Iterator _slist_begin(const void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    char any_mem[classSz(T(Any))];
    Any any = VA_ANY(self->_head.nxt, NULL, any_mem);
    Iterator ret = construct(__SlistIter, mem, VA(ForwardIter), self->class, any, VAEND);
    destroy(any);
    return ret;
}

static Iterator _slist_end(const void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    void *mem = ARP_MallocARelDtor(classSz(__SlistIter), destroy);
    char any_mem[classSz(T(Any))];
    Any any = VA_ANY(TEMP_VAR(void*, NULL), NULL, any_mem);
    Iterator ret = construct(__SlistIter, mem, VA(ForwardIter), self->class, any, VAEND);
    destroy(any);
    return ret;
}

static void *_slist_front(const void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    return DATA(*self->_head.nxt);
}

static size_t _slist_size(const void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct SlistNode *node = self->_head.nxt;
    size_t size = 0;
    while (node != NULL)
    {
        size++;
        node = node->nxt;
    }
    return size;
}

static bool _slist_empty(const void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    return self->_head.nxt == NULL;
}

static void _slist_push_front(void *_self, const void *_x)
{
    struct Slist *self = offsetOf(_self, __Slist);
    size_t memb_size = classSz(self->class);
    struct SlistNode *new_node = allocate(sizeof(struct SlistNode) + memb_size);
    construct(self->class, DATA(*new_node), _x, VAEND);
    new_node->nxt = self->_head.nxt;
    self->_head.nxt = new_node;
}

static void _slist_pop_front(void *_self)
{
    assert(!_slist_empty(_self));
    struct Slist *self = offsetOf(_self, __Slist);
    size_t memb_size = classSz(self->class);
    struct SlistNode *node = self->_head.nxt;
    destroy(DATA(*node));
    self->_head.nxt = self->_head.nxt->nxt;
    deallocate(node, memb_size + sizeof(struct SlistNode));
}

static Iterator _slist_erase_after(void *_self, Iterator iter)
{
    struct Slist *self = offsetOf(_self, __Slist);
    assert(classOf(iter) == __SlistIter);
    struct SlistIter *it = offsetOf(iter, __SlistIter);
    struct SlistNode *node = it->node->nxt;
    size_t memb_size = classSz(self->class);
    assert(it->node);
    it->node->nxt = node->nxt;
    destroy(DATA(*node));
    deallocate(node, memb_size + sizeof(struct SlistNode));
    return iter;
}

static Iterator _slist_insert_after(void *_self, Iterator iter, const void *_x)
{
    struct Slist *self = offsetOf(_self, __Slist);
    assert(classOf(iter) == __SlistIter);
    struct SlistIter *it = offsetOf(iter, __SlistIter);
    size_t memb_size = classSz(self->class);
    struct SlistNode *new_node = allocate(sizeof(struct SlistNode) + memb_size);
    construct(self->class, DATA(*new_node), _x, VAEND);
    new_node->nxt = it->node->nxt;
    it->node->nxt = new_node;
    return iter;
}

static void _slist_remove(void *_self, const void *x)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct SlistNode *node = &self->_head;
    while (node->nxt)
    {
        Object obj = DATA(*node);
        if (THIS(obj).equal(x)) {
            struct SlistNode *tmp = node->nxt;
            node->nxt = node->nxt->nxt;
            destroy(DATA(*node));
            deallocate(tmp, classSz(self->class) + sizeof(struct SlistNode));
            break;
        }
        node = node->nxt;
    }
}

static void _slist_unique(void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct SlistNode *node = self->_head.nxt;
    if (!node)
        return;
    struct SlistNode *next_node = node->nxt;
    size_t memb_size = classSz(self->class);
    while (next_node)
    {
        Object obj = DATA(*node);
        Object x = DATA(*next_node);
        if (THIS(obj).equal(x)) {
            node->nxt = node->nxt->nxt;
            destroy(DATA(*next_node));
            deallocate(next_node, memb_size + sizeof(struct SlistNode));
            next_node = node;
        } else
            node = next_node;
        next_node = next_node->nxt;
    }
}

static void _slist_splice_after(void *_self, Iterator _position, Slist l, va_list *app)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct Slist *L = offsetOf(l, __Slist);
    assert(L->class == self->class);
    MetaObject args[2];
    void *t;
    int n = 0;
    while ((t = va_arg(*app, MetaObject)) != VAEND)
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
        first_node = &L->_head;
        last_node = L->_head.nxt;
        while (last_node && last_node->nxt)
            last_node = last_node->nxt;
    } else if (n == 1) {
        first_node = ((struct SlistIter*)offsetOf(args[0], __SlistIter))->node;
        last_node = first_node->nxt;
        if (pos_node == first_node || pos_node == last_node)
            return;
    } else {
        first_node = ((struct SlistIter*)offsetOf(args[0], __SlistIter))->node;
        last_node = ((struct SlistIter*)offsetOf(args[1], __SlistIter))->node;
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

static void _slist_merge(void *_self, Slist l, Compare cmp)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct Slist *L = offsetOf(l, __Slist);

    struct SlistNode *node = &self->_head;
    while (node->nxt && L->_head.nxt)
    {
        if (cmp(DATA(*node->nxt), DATA(*L->_head.nxt)) > 0) {
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

static void _slist_reverse(void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    if (!self->_head.nxt || !self->_head.nxt->nxt)
        return;
    struct SlistNode *first = self->_head.nxt;
    struct SlistNode *next = first->nxt;
    self->_head.nxt = NULL;
    while (next)
    {
        first->nxt = self->_head.nxt;
        self->_head.nxt = first;
        first = next;
        next = first->nxt;
    }
    first->nxt = self->_head.nxt;
    self->_head.nxt = first;
}

static void _slist_sort(void *_self, Compare cmp)
{
    struct Slist *self = offsetOf(_self, __Slist);
    if (self->_head.nxt == NULL || self->_head.nxt->nxt == NULL)
        return;
    Slist _carry = new(T(Slist), self->class, VAEND);
    struct Slist *carry = offsetOf(_carry, __Slist);
    Slist counter[64];
    int fill = 0;
    while (self->_head.nxt)
    {
        struct SlistNode *node = &carry->_head;
        struct SlistNode *first_node = &self->_head;
        struct SlistNode *last_node = self->_head.nxt;
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
            counter[fill++] = new(T(Slist), self->class, VAEND);
        THIS(counter[i]).swap(_carry);
    }
    for (int i = 1; i < fill; i++) {
        THIS(counter[i]).merge(counter[i - 1], cmp);
        delete(counter[i - 1]);
    }
    _slist_swap(_self, counter[fill - 1]);
    delete(counter[fill - 1]);
    delete(_carry);
}

static void _slist_clear(void *_self)
{
    struct Slist *self = offsetOf(_self, __Slist);
    while (self->_head.nxt != NULL)
        _slist_pop_front(_self);
}

static void _slist_swap(void *_self, Slist _l)
{
    struct Slist *self = offsetOf(_self, __Slist);
    struct Slist *l = offsetOf(_l, __Slist);
    struct SlistNode *temp = self->_head.nxt;
    self->_head.nxt = l->_head.nxt;
    l->_head.nxt = temp;
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->end);
    return class->end(_self);
}

static void *_front(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->front);
    return class->front(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _push_front(const void *x)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->push_front);
    class->push_front(_self, x);
}

static void _pop_front(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->pop_front);
    class->pop_front(_self);
}

static Iterator _erase_after(Iterator iter)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->erase_after);
    return class->erase_after(_self, iter);
}

static Iterator _insert_after(Iterator iter, const void *x)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->insert_after);
    return class->insert_after(_self, iter, x);
}

static void _remove(const void *x)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->remove);
    class->remove(_self, x);
}

static void _unique(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->unique);
    class->unique(_self);
}

static void _splice_after(Iterator position, Slist l, ...)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->splice_after);
    va_list ap;
    va_start(ap, l);
    class->splice_after(_self, position, l, &ap);
    va_end(ap);
}

static void _merge(Slist l, Compare cmp)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->merge);
    class->merge(_self, l, cmp);
}

static void _reverse(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->reverse);
    class->reverse(_self);
}

static void _sort(Compare cmp)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->sort);
    class->sort(_self, cmp);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Slist l)
{
    void *_self = pop_this();
    const struct SlistClass *class = offsetOf(classOf(_self), __SlistClass);
    assert(class->clear);
    class->swap(_self, l);
}
