//
// Created by xjs on 2020/12/20.
//

#include "include/_tctl_unordered_multiset.h"
#include "../include/tctl_hashtable.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR, ANY

struct Unordered_MultiSetClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, const void *x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    size_t (*bucket_count)(void *_self);
    size_t (*max_bucket_count)(void *_self);
    void (*reserve)(void *_self, size_t x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Unordered_MultiSet _s);
};

struct Unordered_MultiSet {
    Hashtable c;
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
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_MultiSet _s);

//Unordered_MultiSetclass
static void *_unordered_multisetclass_ctor(void *_self, va_list *app);

//Unordered_MultiSet
static void *_unordered_multiset_ctor(void *_self, va_list *app);
static void *_unordered_multiset_dtor(void *_self);
static Iterator _unordered_multiset_begin(const void *_self);
static Iterator _unordered_multiset_end(const void *_self);
static size_t _unordered_multiset_size(const void *_self);
static bool _unordered_multiset_empty(const void *_self);
static void _unordered_multiset_erase(void *_self, Iterator iter);
static Iterator _unordered_multiset_insert(void *_self, const void *x);
static size_t _unordered_multiset_count(void *_self, const void *x);
static Iterator _unordered_multiset_find(void *_self, const void* x);
static size_t _unordered_multiset_bucket_count(void *_self);
static size_t _unordered_max_bucket_count(void *_self);
void _unordered_multiset_reserve(void *_self, size_t x);
static void _unordered_multiset_clear(void *_self);
static void _unordered_multiset_swap(void *_self, Unordered_MultiSet _s);

//init
static const void *__Unordered_MultiSet = NULL;
static const void *__Unordered_MultiSetClass = NULL;
volatile static struct Unordered_MultiSetSelector Unordered_MultiSetS = {
    {0},
    _begin,
    _end,
    _size,
    _empty,
    _erase,
    _insert,
    _count,
    _find,
    _bucket_count,
    _max_bucket_count,
    _reserve,
    _clear,
    _swap
};
const struct Unordered_MultiSetSelector *_Unordered_MultiSetS = NULL;

static void initUnordered_MultiSet(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Unordered_MultiSetS) {
        _Unordered_MultiSetS = (void*)&Unordered_MultiSetS;
        memcpy((void*)&Unordered_MultiSetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Unordered_MultiSetClass) {
        __Unordered_MultiSetClass = new(T(MetaClass), "Unordered_MultiSetClass",
                           T(Class), sizeof(struct Unordered_MultiSetClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _unordered_multisetclass_ctor, NULL);
    }
    if (!__Unordered_MultiSet) {
        __Unordered_MultiSet = new(_Unordered_MultiSetClass(), "Unordered_MultiSet",
                     T(Object), sizeof(struct Unordered_MultiSet) + classSz(T(Object)),
                     _MetaClassS->ctor, _unordered_multiset_ctor,
                     _MetaClassS->dtor, _unordered_multiset_dtor,
                     _Unordered_MultiSetS->begin, _unordered_multiset_begin,
                     _Unordered_MultiSetS->end, _unordered_multiset_end,
                     _Unordered_MultiSetS->size, _unordered_multiset_size,
                     _Unordered_MultiSetS->empty, _unordered_multiset_empty,
                     _Unordered_MultiSetS->erase, _unordered_multiset_erase,
                     _Unordered_MultiSetS->insert, _unordered_multiset_insert,
                     _Unordered_MultiSetS->count, _unordered_multiset_count,
                     _Unordered_MultiSetS->find, _unordered_multiset_find,
                     _Unordered_MultiSetS->bucket_count, _unordered_multiset_bucket_count,
                     _Unordered_MultiSetS->max_bucket_count, _unordered_max_bucket_count,
                     _Unordered_MultiSetS->reserve, _unordered_multiset_reserve,
                     _Unordered_MultiSetS->clear, _unordered_multiset_clear,
                     _Unordered_MultiSetS->swap, _unordered_multiset_swap,
                     Selector, _Unordered_MultiSetS, NULL);
    }
}

const void *_Unordered_MultiSet(void)
{
    if (!__Unordered_MultiSet)
        initUnordered_MultiSet();
    return __Unordered_MultiSet;
}

const void *_Unordered_MultiSetClass(void)
{
    if (!__Unordered_MultiSetClass)
        initUnordered_MultiSet();
    return __Unordered_MultiSetClass;
}

//private
static void *_get_val(void *x)
{
    return x;
}

//public
//Unordered_MultiSetClass
static void *_unordered_multisetclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_MultiSetClass, _self, app);
    struct Unordered_MultiSetClass *self = offsetOf(_self, __Unordered_MultiSetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&Unordered_MultiSetS + sizeof(Unordered_MultiSetS._));
    voidf *end = (voidf*)((char*)&Unordered_MultiSetS + sizeof(Unordered_MultiSetS));
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

//Unordered_MultiSet
static void *_unordered_multiset_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_MultiSet, _self, app);
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    self->c = malloc(classSz(T(Hashtable)));
    void *class = va_arg(*app, void*);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Unordered_MultiSet) { //复制构造
        struct Unordered_MultiSet *s = offsetOf(t, __Unordered_MultiSet);
        construct(T(Hashtable), self->c, class, s->c, VAEND);
    } else { //迭代器构造和默认构造
        assert(classOf(t) == T(Any));
        Any equal = t;
        Any hash = va_arg(*app, void*);
        assert(classOf(hash) == T(Any));
        t = va_arg(*app, void*);
        Any get_val;
        if (t != VAEND && classOf(t) == T(Any))
            get_val = t;
        else
            get_val = VA(_get_val, FUNC);
        construct(T(Hashtable), self->c, class, equal, hash, get_val, VAEND);
        if (t == VAEND)
            return _self;
        t = va_arg(*app, void*);
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

static void *_unordered_multiset_dtor(void *_self)
{
    _self = super_dtor(__Unordered_MultiSet, _self);
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _unordered_multiset_begin(const void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).begin();
}

static Iterator _unordered_multiset_end(const void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).end();
}

static size_t _unordered_multiset_size(const void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).size();
}

static bool _unordered_multiset_empty(const void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).empty();
}

static void _unordered_multiset_erase(void *_self, Iterator iter)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    THIS(self->c).erase(iter);
}

static Iterator _unordered_multiset_insert(void *_self, const void *x)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).insert_equal(x);
}

static size_t _unordered_multiset_count(void *_self, const void *x)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).count(x);
}

static Iterator _unordered_multiset_find(void *_self, const void *x)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).find(x);
}

static size_t _unordered_multiset_bucket_count(void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    return THIS(self->c).max_bucket_count();
}

void _unordered_multiset_reserve(void *_self, size_t x)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    THIS(self->c).resize(x);
}

static void _unordered_multiset_clear(void *_self)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    THIS(self->c).clear();
}

static void _unordered_multiset_swap(void *_self, Unordered_MultiSet _s)
{
    struct Unordered_MultiSet *self = offsetOf(_self, __Unordered_MultiSet);
    struct Unordered_MultiSet *s = offsetOf(_s, __Unordered_MultiSet);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->find);
    return class->find(_self, x);
}

static size_t _bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->bucket_count);
    return class->bucket_count(_self);
}

static size_t _max_bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_self);
}

static void _reserve(size_t x)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->reserve);
    class->reserve(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Unordered_MultiSet _s)
{
    void *_self = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_self), __Unordered_MultiSetClass);
    assert(class->swap);
    class->swap(_self, _s);
}
