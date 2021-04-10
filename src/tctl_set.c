//
// Created by xjs on 2020/11/16.
//

#include "include/_tctl_set.h"
#include "../include/tctl_rb_tree.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR

struct SetClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, FormWO_t x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Set _s);
};

struct Set {
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
static void _swap(Set _s);

//Setclass
static void *_setclass_ctor(void *_this, va_list *app);

//Set
static void *_set_ctor(void *_this, va_list *app);
static void *_set_dtor(void *_this);
static Iterator _set_begin(const void *_this);
static Iterator _set_end(const void *_this);
static size_t _set_size(const void *_this);
static bool _set_empty(const void *_this);
static void _set_erase(void *_this, Iterator iter);
static Iterator _set_insert(void *_this, FormWO_t x);
static size_t _set_count(void *_this, FormWO_t x);
static Iterator _set_find(void *_this, FormWO_t x);
static void _set_clear(void *_this);
static void _set_swap(void *_this, Set _s);

//init
static const void *__Set = NULL;
static const void *__SetClass = NULL;
volatile static struct SetSelector SetS = {
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
const struct SetSelector *_SetS = NULL;

static void initSet(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_SetS) {
        _SetS = (void*)&SetS;
        memcpy((void*)&SetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__SetClass) {
        __SetClass = new(T(MetaClass), "SetClass",
                           T(Class), sizeof(struct SetClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _setclass_ctor, NULL);
    }
    if (!__Set) {
        __Set = new(_SetClass(), "Set",
                     T(Object), sizeof(struct Set) + classSz(_Object().class),
                     _MetaClassS->ctor, _set_ctor,
                     _MetaClassS->dtor, _set_dtor,
                     _SetS->begin, _set_begin,
                     _SetS->end, _set_end,
                     _SetS->size, _set_size,
                     _SetS->empty, _set_empty,
                     _SetS->erase, _set_erase,
                     _SetS->insert, _set_insert,
                     _SetS->count, _set_count,
                     _SetS->find, _set_find,
                     _SetS->clear, _set_clear,
                     _SetS->swap, _set_swap,
                     Selector, _SetS, NULL);
    }
}

Form_t _Set(void)
{
    if (!__Set)
        initSet();
    return (Form_t){OBJ, .class = __Set};
}

Form_t _SetClass(void)
{
    if (!__SetClass)
        initSet();
    return (Form_t){OBJ, .class = __SetClass};
}

//SetClass
static void *_setclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__SetClass, _this, app);
    struct SetClass *this = offsetOf(_this, __SetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&SetS + sizeof(SetS._);
    voidf *end = (void*)&SetS + sizeof(SetS);
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

//Set
static void *_set_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Set, _this, app);
    struct Set *this = offsetOf(_this, __Set);
    this->c = malloc(classSz(_RB_tree().class));
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ) { //复制构造
        assert(t._.class == __Set);
        struct Set *s = offsetOf(*(Object*)t.mem, __Set);
        construct(_RB_tree(), this->c, f, VA(s->c), VAEND);
    } else { //迭代器构造和默认构造
        construct(_RB_tree(), this->c, f, t, VAEND);
        t = va_arg(*app, FormWO_t);
        if (t._.f == END)
            return _this;

        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator first = *(Iterator*)t.mem;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, FormWO_t);
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator last = *(Iterator*)t.mem;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        Form_t it_t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            THIS(this->c).insert_unique(FORM_WITH_OBJ(it_t, V(THIS(first).derefer())));
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _this;
}

static void *_set_dtor(void *_this)
{
    _this = super_dtor(__Set, _this);
    struct Set *this = offsetOf(_this, __Set);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _set_begin(const void *_this)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).begin();
}

static Iterator _set_end(const void *_this)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).end();
}

static size_t _set_size(const void *_this)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).size();
}

static bool _set_empty(const void *_this)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).empty();
}

static void _set_erase(void *_this, Iterator iter)
{
    struct Set *this = offsetOf(_this, __Set);
    THIS(this->c).erase(iter);
}

static Iterator _set_insert(void *_this, FormWO_t x)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).insert_unique(x);
}

static size_t _set_count(void *_this, FormWO_t x)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).count(x);
}

static Iterator _set_find(void *_this, FormWO_t x)
{
    struct Set *this = offsetOf(_this, __Set);
    return THIS(this->c).find(x);
}

static void _set_clear(void *_this)
{
    struct Set *this = offsetOf(_this, __Set);
    THIS(this->c).clear();
}

static void _set_swap(void *_this, Set _s)
{
    struct Set *this = offsetOf(_this, __Set);
    struct Set *s = offsetOf(_s, __Set);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(FormWO_t x)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->find);
    return class->find(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Set _s)
{
    void *_this = pop_this();
    const struct SetClass *class = offsetOf(classOf(_this), __SetClass);
    assert(class->swap);
    class->swap(_this, _s);
}
