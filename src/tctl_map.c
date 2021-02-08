//
// Created by xjs on 2020/11/18.
//

#include "include/_tctl_map.h"
#include "../include/tctl_rb_tree.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, PAIR

struct MapClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Pair x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Map _s);
};

struct Map {
    Form_t key_f;
    Form_t val_f;
    RB_tree c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(Pair x);
static size_t _count(FormWO_t x);
static Iterator _find(FormWO_t x);
static void _clear(void);
static void _swap(Map _s);

//Mapclass
static void *_mapclass_ctor(void *_this, va_list *app);

//Map
static void *_map_ctor(void *_this, va_list *app);
static void *_map_dtor(void *_this);
static Iterator _map_begin(const void *_this);
static Iterator _map_end(const void *_this);
static size_t _map_size(const void *_this);
static bool _map_empty(const void *_this);
static void _map_erase(void *_this, Iterator iter);
static Iterator _map_insert(void *_this, Pair x);
static size_t _map_count(void *_this, FormWO_t x);
static Iterator _map_find(void *_this, FormWO_t x);
static void _map_clear(void *_this);
static void _map_swap(void *_this, Map _s);

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

void initMap(void)
{
    initClass();
    initIterator();
    initRB_tree();
    if (!_MapS) {
        _MapS = (void*)&MapS;
        memcpy((void*)&MapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__MapClass) {
        __MapClass = new(T(MetaClass), "MapClass",
                           T(Class), sizeof(struct MapClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _mapclass_ctor, NULL);
    }
    if (!__Map) {
        __Map = new(_MapClass(), "Priority_queue",
                     T(Object), sizeof(struct Map) + classSz(_Object().class),
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

Form_t _Map(void)
{
    Form_t t = {OBJ, .class = __Map};
    return t;
}

Form_t _MapClass(void)
{
    Form_t t = {OBJ, .class = __MapClass};
    return t;
}

//MapClass
static void *_mapclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__MapClass, _this, app);
    struct MapClass *this = offsetOf(_this, __MapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&MapS + sizeof(MapS._);
    voidf *end = (void*)&MapS + sizeof(MapS);
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

//Map
static void *_map_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Map, _this, app);
    struct Map *this = offsetOf(_this, __Map);
    this->c = malloc(classSz(_RB_tree().class));
    //获取key和val的类型
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    f._.f -= FORM;
    this->key_f = f._;
    f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    f._.f -= FORM;
    this->val_f = f._;
    //构建rb_tree
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ && t._.class == __Map) { //复制构造
        struct Map *s = offsetOf(t.mem, __Map);
        construct(_RB_tree(), this->c, VA(_Pair()), VA(s->c), VAEND);
    } else { //迭代器构造和默认构造
        construct(_RB_tree(), this->c, VA(_Pair()), t, VAEND);
        t = va_arg(*app, FormWO_t);
        if (t._.f == END)
            return _this;
        //迭代器
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator first = t.mem;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, FormWO_t);
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator last = t.mem;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        Form_t it_t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            THIS(this->c).insert_unique(FORM_WITH_OBJ(it_t, THIS(first).derefer()));
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _this;
}

static void *_map_dtor(void *_this)
{
    _this = super_dtor(__Map, _this);
    struct Map *this = offsetOf(_this, __Map);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _map_begin(const void *_this)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).begin();
}

static Iterator _map_end(const void *_this)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).end();
}

static size_t _map_size(const void *_this)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).size();
}

static bool _map_empty(const void *_this)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).empty();
}

static void _map_erase(void *_this, Iterator iter)
{
    struct Map *this = offsetOf(_this, __Map);
    THIS(this->c).erase(iter);
}

static Iterator _map_insert(void *_this, Pair x)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).insert_unique(VA(x));
}

static size_t _map_count(void *_this, FormWO_t x)
{
    struct Map *this = offsetOf(_this, __Map);
    return THIS(this->c).count(x);
}

static Iterator _map_find(void *_this, FormWO_t x)
{
    struct Map *this = offsetOf(_this, __Map);
    Pair p = new(T(Pair), VA(this->key_f, this->val_f, x));
    Iterator res = THIS(this->c).find(VA(p));
    delete(p);
    return res;
}

static void _map_clear(void *_this)
{
    struct Map *this = offsetOf(_this, __Map);
    THIS(this->c).clear();
}

static void _map_swap(void *_this, Map _s)
{
    struct Map *this = offsetOf(_this, __Map);
    struct Map *s = offsetOf(_s, __Map);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->begin);
    return class->begin(_this);
}
static Iterator _end(void)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->end);
    return class->end(_this);
}
static size_t _size(void)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->size);
    return class->size(_this);
}
static bool _empty(void)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->empty);
    return class->empty(_this);
}
static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->erase);
    class->erase(_this, iter);
}
static Iterator _insert(Pair x)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->insert);
    return class->insert(_this, x);
}
static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->count);
    return class->count(_this, x);
}
static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->find);
    return class->find(_this, x);
}
static void _clear(void)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->clear);
    class->clear(_this);
}
static void _swap(Map _s)
{
    void *_this = pop_this();
    const struct MapClass *class = offsetOf(classOf(_this), __MapClass);
    assert(class->swap);
    class->swap(_this, _s);
}
