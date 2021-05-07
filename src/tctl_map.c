//
// Created by xjs on 2020/11/18.
//

#include "include/_tctl_map.h"
#include "../include/tctl_rb_tree.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, PAIR, ANY

struct MapClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Pair x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Map _s);
};

struct Map {
    void *key_class;
    void *val_class;
    RB_tree c;
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
static void _clear(void);
static void _swap(Map _s);

//Mapclass
static void *_mapclass_ctor(void *_self, va_list *app);

//Map
static void *_map_ctor(void *_self, va_list *app);
static void *_map_dtor(void *_self);
static Iterator _map_begin(const void *_self);
static Iterator _map_end(const void *_self);
static size_t _map_size(const void *_self);
static bool _map_empty(const void *_self);
static void _map_erase(void *_self, Iterator iter);
static Iterator _map_insert(void *_self, Pair x);
static size_t _map_count(void *_self, const void *x);
static Iterator _map_find(void *_self, const void *x);
static void _map_clear(void *_self);
static void _map_swap(void *_self, Map _s);

//init
static const void *__Map = NULL;
static const void *__MapClass = NULL;
volatile static struct MapSelector MapS = {
    {},
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
const struct MapSelector *_MapS = NULL;

static void initMap(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_MapS) {
        _MapS = (void*)&MapS;
        memcpy((void*)&MapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__MapClass) {
        __MapClass = new(T(MetaClass), "MapClass",
                           T(Class), sizeof(struct MapClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _mapclass_ctor, NULL);
    }
    if (!__Map) {
        __Map = new(_MapClass(), "Map",
                     T(Object), sizeof(struct Map) + classSz(T(Object)),
                     _MetaClassS->ctor, _map_ctor,
                     _MetaClassS->dtor, _map_dtor,
                     _MapS->begin, _map_begin,
                     _MapS->end, _map_end,
                     _MapS->size, _map_size,
                     _MapS->empty, _map_empty,
                     _MapS->erase, _map_erase,
                     _MapS->insert, _map_insert,
                     _MapS->count, _map_count,
                     _MapS->find, _map_find,
                     _MapS->clear, _map_clear,
                     _MapS->swap, _map_swap,
                     Selector, _MapS, NULL);
    }
}

const void *_Map(void)
{
    if (!__Map)
        initMap();
    return __Map;
}

const void *_MapClass(void)
{
    if (!__MapClass)
        initMap();
    return __MapClass;
}

//MapClass
static void *_mapclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__MapClass, _self, app);
    struct MapClass *self = offsetOf(_self, __MapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&MapS + sizeof(MapS._);
    voidf *end = (void*)&MapS + sizeof(MapS);
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

//Map
static void *_map_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Map, _self, app);
    struct Map *self = offsetOf(_self, __Map);
    self->c = malloc(classSz(T(RB_tree)));
    //获取key和val的类型
    void *f = va_arg(*app, void*);
    self->key_class = f;
    f = va_arg(*app, void*);
    self->val_class = f;
    //构建rb_tree
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Map) { //复制构造
        struct Map *s = offsetOf(t, __Map);
        construct(_RB_tree(), self->c, T(Pair), s->c, VAEND);
    } else { //迭代器构造和默认构造
        construct(_RB_tree(), self->c, T(Pair), t, VAEND);
        t = va_arg(*app, void*);
        if (t == VAEND)
            return _self;
        //迭代器
        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        while (!THIS(first).equal(VA(last)))
        {
            THIS(self->c).insert_unique(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_map_dtor(void *_self)
{
    _self = super_dtor(__Map, _self);
    struct Map *self = offsetOf(_self, __Map);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _map_begin(const void *_self)
{
    struct Map *self = offsetOf(_self, __Map);
    return THIS(self->c).begin();
}

static Iterator _map_end(const void *_self)
{
    struct Map *self = offsetOf(_self, __Map);
    return THIS(self->c).end();
}

static size_t _map_size(const void *_self)
{
    struct Map *self = offsetOf(_self, __Map);
    return THIS(self->c).size();
}

static bool _map_empty(const void *_self)
{
    struct Map *self = offsetOf(_self, __Map);
    return THIS(self->c).empty();
}

static void _map_erase(void *_self, Iterator iter)
{
    struct Map *self = offsetOf(_self, __Map);
    THIS(self->c).erase(iter);
}

static Iterator _map_insert(void *_self, Pair x)
{
    struct Map *self = offsetOf(_self, __Map);
    return THIS(self->c).insert_unique(x);
}

static size_t _map_count(void *_self, const void *x)
{
    struct Map *self = offsetOf(_self, __Map);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    size_t res = THIS(self->c).count(p);
    delete(p);
    return res;
}

static Iterator _map_find(void *_self, const void *x)
{
    struct Map *self = offsetOf(_self, __Map);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    Iterator res = THIS(self->c).find(p);
    delete(p);
    return res;
}

static void _map_clear(void *_self)
{
    struct Map *self = offsetOf(_self, __Map);
    THIS(self->c).clear();
}

static void _map_swap(void *_self, Map _s)
{
    struct Map *self = offsetOf(_self, __Map);
    struct Map *s = offsetOf(_s, __Map);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(Pair x)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->find);
    return class->find(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Map _s)
{
    void *_self = pop_this();
    const struct MapClass *class = offsetOf(classOf(_self), __MapClass);
    assert(class->swap);
    class->swap(_self, _s);
}
