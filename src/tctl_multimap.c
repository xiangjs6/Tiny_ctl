//
// Created by xjs on 2020/11/19.
//

#include "include/_tctl_multimap.h"
#include "../include/tctl_rb_tree.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, PAIR

struct MultiMapClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Pair x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, MultiMap _s);
};

struct MultiMap {
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
static void _swap(MultiMap _s);

//MultiMapclass
static void *_multimapclass_ctor(void *_this, va_list *app);

//MultiMap
static void *_multimap_ctor(void *_this, va_list *app);
static void *_multimap_dtor(void *_this);
static Iterator _multimap_begin(const void *_this);
static Iterator _multimap_end(const void *_this);
static size_t _multimap_size(const void *_this);
static bool _multimap_empty(const void *_this);
static void _multimap_erase(void *_this, Iterator iter);
static Iterator _multimap_insert(void *_this, Pair x);
static size_t _multimap_count(void *_this, FormWO_t x);
static Iterator _multimap_find(void *_this, FormWO_t x);
static void _multimap_clear(void *_this);
static void _multimap_swap(void *_this, MultiMap _s);

//init
static const void *__MultiMap = NULL;
static const void *__MultiMapClass = NULL;
volatile static struct MultiMapSelector MultiMapS = {
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
const struct MultiMapSelector *_MultiMapS = NULL;

static void initMultiMap(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_MultiMapS) {
        _MultiMapS = (void*)&MultiMapS;
        memcpy((void*)&MultiMapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__MultiMapClass) {
        __MultiMapClass = new(T(MetaClass), "MultiMapClass",
                           T(Class), sizeof(struct MultiMapClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _multimapclass_ctor, NULL);
    }
    if (!__MultiMap) {
        __MultiMap = new(_MultiMapClass(), "MultiMap",
                     T(Object), sizeof(struct MultiMap) + classSz(_Object().class),
                     _MetaClassS->ctor, _multimap_ctor,
                     _MetaClassS->dtor, _multimap_dtor,
                     _MultiMapS->begin, _multimap_begin,
                     _MultiMapS->end, _multimap_end,
                     _MultiMapS->size, _multimap_size,
                     _MultiMapS->empty, _multimap_empty,
                     _MultiMapS->erase, _multimap_erase,
                     _MultiMapS->insert, _multimap_insert,
                     _MultiMapS->count, _multimap_count,
                     _MultiMapS->find, _multimap_find,
                     _MultiMapS->clear, _multimap_clear,
                     _MultiMapS->swap, _multimap_swap,
                     Selector, _MultiMapS, NULL);
    }
}

Form_t _MultiMap(void)
{
    if (!__MultiMap)
        initMultiMap();
    return (Form_t){OBJ, .class = __MultiMap};
}

Form_t _MultiMapClass(void)
{
    if (!__MultiMapClass)
        initMultiMap();
    return (Form_t){OBJ, .class = __MultiMapClass};
}

//MultiMapClass
static void *_multimapclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__MultiMapClass, _this, app);
    struct MultiMapClass *this = offsetOf(_this, __MultiMapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&MultiMapS + sizeof(MultiMapS._);
    voidf *end = (void*)&MultiMapS + sizeof(MultiMapS);
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

//MultiMap
static void *_multimap_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__MultiMap, _this, app);
    struct MultiMap *this = offsetOf(_this, __MultiMap);
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
    if (t._.f == OBJ && t._.class == __MultiMap) { //复制构造
        struct MultiMap *s = offsetOf(t.mem, __MultiMap);
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

static void *_multimap_dtor(void *_this)
{
    _this = super_dtor(__MultiMap, _this);
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _multimap_begin(const void *_this)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    return THIS(this->c).begin();
}

static Iterator _multimap_end(const void *_this)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    return THIS(this->c).end();
}

static size_t _multimap_size(const void *_this)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    return THIS(this->c).size();
}

static bool _multimap_empty(const void *_this)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    return THIS(this->c).empty();
}

static void _multimap_erase(void *_this, Iterator iter)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    THIS(this->c).erase(iter);
}

static Iterator _multimap_insert(void *_this, Pair x)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    return THIS(this->c).insert_unique(VA(x));
}

static size_t _multimap_count(void *_this, FormWO_t x)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    Pair p = new(T(Pair), VA(this->key_f, this->val_f, x));
    size_t res = THIS(this->c).count(VA(p));
    delete(p);
    return res;
}

static Iterator _multimap_find(void *_this, FormWO_t x)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    Pair p = new(T(Pair), VA(this->key_f, this->val_f, x));
    Iterator res = THIS(this->c).find(VA(p));
    delete(p);
    return res;
}

static void _multimap_clear(void *_this)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    THIS(this->c).clear();
}

static void _multimap_swap(void *_this, MultiMap _s)
{
    struct MultiMap *this = offsetOf(_this, __MultiMap);
    struct MultiMap *s = offsetOf(_s, __MultiMap);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(Pair x)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->find);
    return class->find(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(MultiMap _s)
{
    void *_this = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_this), __MultiMapClass);
    assert(class->swap);
    class->swap(_this, _s);
}
