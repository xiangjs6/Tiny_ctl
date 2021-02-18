//
// Created by xjs on 2020/11/19.
//

#include "include/_tctl_multiset.h"
#include "../include/tctl_rb_tree.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR

struct MultiSetClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, FormWO_t x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, MultiSet _s);
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
static Iterator _insert(FormWO_t x);
static size_t _count(FormWO_t x);
static Iterator _find(FormWO_t x);
static void _clear(void);
static void _swap(MultiSet _s);

//MultiSetclass
static void *_multisetclass_ctor(void *_this, va_list *app);

//MultiSet
static void *_multiset_ctor(void *_this, va_list *app);
static void *_multiset_dtor(void *_this);
static Iterator _multiset_begin(const void *_this);
static Iterator _multiset_end(const void *_this);
static size_t _multiset_size(const void *_this);
static bool _multiset_empty(const void *_this);
static void _multiset_erase(void *_this, Iterator iter);
static Iterator _multiset_insert(void *_this, FormWO_t x);
static size_t _multiset_count(void *_this, FormWO_t x);
static Iterator _multiset_find(void *_this, FormWO_t x);
static void _multiset_clear(void *_this);
static void _multiset_swap(void *_this, MultiSet _s);

//init
static const void *__MultiSet = NULL;
static const void *__MultiSetClass = NULL;
volatile static struct MultiSetSelector MultiSetS = {
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
                           T(Class), sizeof(struct MultiSetClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _multisetclass_ctor, NULL);
    }
    if (!__MultiSet) {
        __MultiSet = new(_MultiSetClass(), "MultiSet",
                     T(Object), sizeof(struct MultiSet) + classSz(_Object().class),
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

Form_t _MultiSet(void)
{
    if (!__MultiSet)
        initMultiSet();
    return (Form_t){OBJ, .class = __MultiSet};
}

Form_t _MultiSetClass(void)
{
    if (!__MultiSetClass)
        initMultiSet();
    return (Form_t){OBJ, .class = __MultiSetClass};
}

//MultiSetClass
static void *_multisetclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__MultiSetClass, _this, app);
    struct MultiSetClass *this = offsetOf(_this, __MultiSetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&MultiSetS + sizeof(MultiSetS._);
    voidf *end = (void*)&MultiSetS + sizeof(MultiSetS);
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

//MultiSet
static void *_multiset_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__MultiSet, _this, app);
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    this->c = malloc(classSz(_RB_tree().class));
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ && t._.class == __MultiSet) { //复制构造
        struct MultiSet *s = offsetOf(t.mem, __MultiSet);
        construct(_RB_tree(), this->c, f, VA(s->c), VAEND);
    } else { //迭代器构造和默认构造
        construct(_RB_tree(), this->c, f, t, VAEND);
        t = va_arg(*app, FormWO_t);
        if (t._.f == END)
            return _this;

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
            THIS(this->c).insert_equal(FORM_WITH_OBJ(it_t, THIS(first).derefer()));
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _this;
}

static void *_multiset_dtor(void *_this)
{
    _this = super_dtor(__MultiSet, _this);
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _multiset_begin(const void *_this)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).begin();
}

static Iterator _multiset_end(const void *_this)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).end();
}

static size_t _multiset_size(const void *_this)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).size();
}

static bool _multiset_empty(const void *_this)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).empty();
}

static void _multiset_erase(void *_this, Iterator iter)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    THIS(this->c).erase(iter);
}

static Iterator _multiset_insert(void *_this, FormWO_t x)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).insert_equal(x);
}

static size_t _multiset_count(void *_this, FormWO_t x)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).count(x);
}

static Iterator _multiset_find(void *_this, FormWO_t x)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    return THIS(this->c).find(x);
}

static void _multiset_clear(void *_this)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    THIS(this->c).clear();
}

static void _multiset_swap(void *_this, MultiSet _s)
{
    struct MultiSet *this = offsetOf(_this, __MultiSet);
    struct MultiSet *s = offsetOf(_s, __MultiSet);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(FormWO_t x)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->find);
    return class->find(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(MultiSet _s)
{
    void *_this = pop_this();
    const struct MultiSetClass *class = offsetOf(classOf(_this), __MultiSetClass);
    assert(class->swap);
    class->swap(_this, _s);
}
