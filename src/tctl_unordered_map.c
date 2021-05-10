//
// Created by xjs on 2020/12/20.
//
#include "include/_tctl_unordered_map.h"
#include "../include/tctl_hashtable.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR, PAIR, ANY

struct Unordered_MapClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Pair x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    size_t (*bucket_count)(void *_self);
    size_t (*max_bucket_count)(void *_self);
    void (*reserve)(void *_self, size_t x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Unordered_Map _s);
};

struct Unordered_Map {
    void *key_class;
    void *val_class;
    Hashtable c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(Pair x);
static size_t _count(const void *x);
static Iterator _find(const void *x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_Map _s);

//Unordered_Mapclass
static void *_unordered_mapclass_ctor(void *_self, va_list *app);

//Unordered_Map
static void *_unordered_map_ctor(void *_self, va_list *app);
static void *_unordered_map_dtor(void *_self);
static Iterator _unordered_map_begin(const void *_self);
static Iterator _unordered_map_end(const void *_self);
static size_t _unordered_map_size(const void *_self);
static bool _unordered_map_empty(const void *_self);
static void _unordered_map_erase(void *_self, Iterator iter);
static Iterator _unordered_map_insert(void *_self, Pair x);
static size_t _unordered_map_count(void *_self, const void *x);
static Iterator _unordered_map_find(void *_self, const void *x);
static size_t _unordered_map_bucket_count(void *_self);
static size_t _unordered_max_bucket_count(void *_self);
void _unordered_map_reserve(void *_self, size_t x);
static void _unordered_map_clear(void *_self);
static void _unordered_map_swap(void *_self, Unordered_Map _s);

//init
static const void *__Unordered_Map = NULL;
static const void *__Unordered_MapClass = NULL;
volatile static struct Unordered_MapSelector Unordered_MapS = {
    {},
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
const struct Unordered_MapSelector *_Unordered_MapS = NULL;

static void initUnordered_Map(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Unordered_MapS) {
        _Unordered_MapS = (void*)&Unordered_MapS;
        memcpy((void*)&Unordered_MapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Unordered_MapClass) {
        __Unordered_MapClass = new(T(MetaClass), "Unordered_MapClass",
                           T(Class), sizeof(struct Unordered_MapClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _unordered_mapclass_ctor, NULL);
    }
    if (!__Unordered_Map) {
        __Unordered_Map = new(_Unordered_MapClass(), "Unordered_Map",
                     T(Object), sizeof(struct Unordered_Map) + classSz(T(Object)),
                     _MetaClassS->ctor, _unordered_map_ctor,
                     _MetaClassS->dtor, _unordered_map_dtor,
                     _Unordered_MapS->begin, _unordered_map_begin,
                     _Unordered_MapS->end, _unordered_map_end,
                     _Unordered_MapS->size, _unordered_map_size,
                     _Unordered_MapS->empty, _unordered_map_empty,
                     _Unordered_MapS->erase, _unordered_map_erase,
                     _Unordered_MapS->insert, _unordered_map_insert,
                     _Unordered_MapS->count, _unordered_map_count,
                     _Unordered_MapS->find, _unordered_map_find,
                     _Unordered_MapS->bucket_count, _unordered_map_bucket_count,
                     _Unordered_MapS->max_bucket_count, _unordered_max_bucket_count,
                     _Unordered_MapS->reserve, _unordered_map_reserve,
                     _Unordered_MapS->clear, _unordered_map_clear,
                     _Unordered_MapS->swap, _unordered_map_swap,
                     Selector, _Unordered_MapS, NULL);
    }
}

const void *_Unordered_Map(void)
{
    if (!__Unordered_Map)
        initUnordered_Map();
    return __Unordered_Map;
}

const void *_Unordered_MapClass(void)
{
    if (!__Unordered_MapClass)
        initUnordered_Map();
    return __Unordered_MapClass;
}

//private
static void *_get_val(void *x)
{
    assert(classOf(x) == T(Pair));
    Pair p = x;
    return p->first;
}

//public
//Unordered_MapClass
static void *_unordered_mapclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_MapClass, _self, app);
    struct Unordered_MapClass *self = offsetOf(_self, __Unordered_MapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Unordered_MapS + sizeof(Unordered_MapS._);
    voidf *end = (void*)&Unordered_MapS + sizeof(Unordered_MapS);
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

//Unordered_Map
static void *_unordered_map_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_Map, _self, app);
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    self->c = malloc(classSz(T(Hashtable)));
    //获取key和val的类型
    void *class = va_arg(*app, void*);
    self->key_class = class;
    class = va_arg(*app, void*);
    self->val_class = class;
    //构建Hashtable
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Unordered_Map) { //复制构造
        struct Unordered_Map *s = offsetOf(t, __Unordered_Map);
        construct(T(Hashtable), self->c, T(Pair), s->c, VAEND);
    } else { //迭代器构造和默认构造
        assert(classOf(t) == T(Any));
        Any equal = t;
        t = va_arg(*app, void*);
        assert(classOf(t) == T(Any));
        Any hash = t;
        t = va_arg(*app, void*);
        Any get_val;
        if (t == VAEND && classOf(t) == T(Any))
            get_val = t;
        else
            get_val = VA_ANY(TEMP_VAR(void*, &_get_val), NULL);
        construct(T(Hashtable), self->c, T(Pair), equal, hash, get_val, VAEND);
        if (t == VAEND)
            return _self;
        //迭代器
        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, first, VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, last, VAEND);
        while (!THIS(first).equal(last))
        {
            THIS(self->c).insert_unique(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_unordered_map_dtor(void *_self)
{
    _self = super_dtor(__Unordered_Map, _self);
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _unordered_map_begin(const void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).begin();
}

static Iterator _unordered_map_end(const void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).end();
}

static size_t _unordered_map_size(const void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).size();
}

static bool _unordered_map_empty(const void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).empty();
}

static void _unordered_map_erase(void *_self, Iterator iter)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    THIS(self->c).erase(iter);
}

static Iterator _unordered_map_insert(void *_self, Pair x)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).insert_unique(x);
}

static size_t _unordered_map_count(void *_self, const void *x)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    size_t res = THIS(self->c).count(p);
    delete(p);
    return res;
}

static Iterator _unordered_map_find(void *_self, const void *x)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    Iterator res = THIS(self->c).find(p);
    delete(p);
    return res;
}

static size_t _unordered_map_bucket_count(void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    return THIS(self->c).max_bucket_count();
}

void _unordered_map_reserve(void *_self, size_t x)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    THIS(self->c).resize(x);
}

static void _unordered_map_clear(void *_self)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    THIS(self->c).clear();
}

static void _unordered_map_swap(void *_self, Unordered_Map _s)
{
    struct Unordered_Map *self = offsetOf(_self, __Unordered_Map);
    struct Unordered_Map *s = offsetOf(_s, __Unordered_Map);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(Pair x)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->find);
    return class->find(_self, x);
}

static size_t _bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->bucket_count);
    return class->bucket_count(_self);
}

static size_t _max_bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_self);
}

static void _reserve(size_t x)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->reserve);
    return class->reserve(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Unordered_Map _s)
{
    void *_self = pop_this();
    const struct Unordered_MapClass *class = offsetOf(classOf(_self), __Unordered_MapClass);
    assert(class->swap);
    class->swap(_self, _s);
}
