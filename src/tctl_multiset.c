//
// Created by xjs on 2020/11/19.
//

#include "include/_tctl_multiset.h"
#include "../include/tctl_rb_tree.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, ANY

struct MultiSetClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, const void *x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, MultiSet _s);
};

struct MultiSet {
    RB_tree c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(const void *x);
static size_t _count(const void *x);
static Iterator _find(const void *x);
static void _clear(void);
static void _swap(MultiSet _s);

//MultiSetclass
static void *_multisetclass_ctor(void *_self, va_list *app);

//MultiSet
static void *_multiset_ctor(void *_self, va_list *app);
static void *_multiset_dtor(void *_self);
static Iterator _multiset_begin(const void *_self);
static Iterator _multiset_end(const void *_self);
static size_t _multiset_size(const void *_self);
static bool _multiset_empty(const void *_self);
static void _multiset_erase(void *_self, Iterator iter);
static Iterator _multiset_insert(void *_self, const void *x);
static size_t _multiset_count(void *_self, const void *x);
static Iterator _multiset_find(void *_self, const void *x);
static void _multiset_clear(void *_self);
static void _multiset_swap(void *_self, MultiSet _s);

//init
static const void *__MultiSet = NULL;
static const void *__MultiSetClass = NULL;
volatile static struct MultiSetSelector MultiSetS = {
    {0},
    _begin,
    _end,
    _size,
    _empty,
    _erase,
    _insert,
    _count,
    _find,
    _clear,
    _swap
};
const struct MultiSetSelector *_MultiSetS = NULL;

static void initMultiSet(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_MultiSetS) {
        _MultiSetS = (void*)&MultiSetS;
        memcpy((void*)&MultiSetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__MultiSetClass) {
        __MultiSetClass = new(T(MetaClass), "MultiSetClass",
                           T(Class), sizeof(struct MultiSetClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _multisetclass_ctor, NULL);
    }
    if (!__MultiSet) {
        __MultiSet = new(_MultiSetClass(), "MultiSet",
                     T(Object), sizeof(struct MultiSet) + classSz(T(Object)),
                     _MetaClassS->ctor, _multiset_ctor,
                     _MetaClassS->dtor, _multiset_dtor,
                     _MultiSetS->begin, _multiset_begin,
                     _MultiSetS->end, _multiset_end,
                     _MultiSetS->size, _multiset_size,
                     _MultiSetS->empty, _multiset_empty,
                     _MultiSetS->erase, _multiset_erase,
                     _MultiSetS->insert, _multiset_insert,
                     _MultiSetS->count, _multiset_count,
                     _MultiSetS->find, _multiset_find,
                     _MultiSetS->clear, _multiset_clear,
                     _MultiSetS->swap, _multiset_swap,
                     Selector, _MultiSetS, NULL);
    }
}

const void *_MultiSet(void)
{
    if (!__MultiSet)
        initMultiSet();
    return __MultiSet;
}

const void *_MultiSetClass(void)
{
    if (!__MultiSetClass)
        initMultiSet();
    return __MultiSetClass;
}

//MultiSetClass
static void *_multisetclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__MultiSetClass, _self, app);
    struct MultiSetClass *self = offsetOf(_self, __MultiSetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&MultiSetS + sizeof(MultiSetS._));
    voidf *end = (voidf*)((char*)&MultiSetS + sizeof(MultiSetS));
    voidf *self_begin = (voidf*)self;
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

//MultiSet
static void *_multiset_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__MultiSet, _self, app);
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    self->c = malloc(classSz(T(RB_tree)));
    void *f = va_arg(*app, void*);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __MultiSet) { //复制构造
        struct MultiSet *s = offsetOf(t, __MultiSet);
        construct(_RB_tree(), self->c, f, s->c, VAEND);
    } else { //迭代器构造和默认构造
        construct(T(RB_tree), self->c, f, t, VAEND);
        t = va_arg(*app, void*);
        if (t == VAEND)
            return _self;

        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, first, VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, last, VAEND);
        while (!THIS(first).equal(last))
        {
            THIS(self->c).insert_equal(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_multiset_dtor(void *_self)
{
    _self = super_dtor(__MultiSet, _self);
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _multiset_begin(const void *_self)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).begin();
}

static Iterator _multiset_end(const void *_self)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).end();
}

static size_t _multiset_size(const void *_self)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).size();
}

static bool _multiset_empty(const void *_self)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).empty();
}

static void _multiset_erase(void *_self, Iterator iter)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    THIS(self->c).erase(iter);
}

static Iterator _multiset_insert(void *_self, const void *x)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).insert_equal(x);
}

static size_t _multiset_count(void *_self, const void *x)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).count(x);
}

static Iterator _multiset_find(void *_self, const void *x)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    return THIS(self->c).find(x);
}

static void _multiset_clear(void *_self)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    THIS(self->c).clear();
}

static void _multiset_swap(void *_self, MultiSet _s)
{
    struct MultiSet *self = offsetOf(_self, __MultiSet);
    struct MultiSet *s = offsetOf(_s, __MultiSet);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(const void *x)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->find);
    return class->find(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(MultiSet _s)
{
    void *_self = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_self), __MultiSetClass);
    assert(class->swap);
    class->swap(_self, _s);
}
