//
// Created by xjs on 2020/12/30.
//

#include "include/_tctl_class.h"
#include "include/_tctl_metaclass.h"
#include <memory.h>
#include <assert.h>
#define Import METACLASS, METAOBJECT

struct Class {
    bool (*equal)(const void *_this, FormWO_t x);
    int (*cmp)(const void *_this, FormWO_t x);
    void *(*brackets)(const void *_this, FormWO_t x);
    void (*inc)(void *_this);
    void (*dec)(void *_this);
    void (*self_add)(void *_this, FormWO_t x);
    void (*self_sub)(void *_this, FormWO_t x);
    void (*asign)(void *_this, FormWO_t x);
    void *(*add)(const void *_this, FormWO_t x);
    void *(*sub)(const void *_this, FormWO_t x);
    void *(*mul)(const void *_this, FormWO_t x);
    void *(*div)(const void *_this, FormWO_t x);
    void *(*mod)(const void *_this, FormWO_t x);
    void *(*cast)(const void *_this, const char *c);
};

struct Object {};

static bool _equal(FormWO_t x);
static int _cmp(FormWO_t x);
static void *_brackets(FormWO_t x);
static void _inc(void);
static void _dec(void);
static void _self_add(FormWO_t x);
static void _self_sub(FormWO_t x);
static void _asign(FormWO_t x);
static void *_add(FormWO_t x);
static void *_sub(FormWO_t x);
static void *_mul(FormWO_t x);
static void *_div(FormWO_t x);
static void *_mod(FormWO_t x);
static void *_cast(const char *c);
//init
volatile static struct ClassSelector ClassS = {
        {},
        _equal,
        _cmp,
        _brackets,
        _inc,
        _dec,
        _self_add,
        _self_sub,
        _asign,
        _add,
        _sub,
        _mul,
        _div,
        _mod,
        _cast
};
const struct ClassSelector *_ClassS = NULL;
static const void *__Class = NULL;
static const void *__Object = NULL;

Form_t _Class(void)
{
    return (Form_t){OBJ, {.class = __Class}};
}

Form_t _Object(void)
{
    Form_t t = {OBJ , {.class = __Object}};
    return t;
}

//selector
static bool _equal(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->equal);
    return class->equal(_this, x);
}

static int _cmp(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->cmp);
    return class->cmp(_this, x);
}

static void *_brackets(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->brackets);
    return class->brackets(_this, x);
}

static void _inc(void)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->inc);
    return class->inc(_this);
}

static void _dec(void)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->dec);
    return class->dec(_this);
}

static void _self_add(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->self_add);
    class->self_add(_this, x);
}

static void _self_sub(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->self_sub);
    class->self_sub(_this, x);
}

static void _asign(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->asign);
    class->asign(_this, x);
}

static void *_add(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->add);
    return class->add(_this, x);
}

static void *_sub(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->sub);
    return class->sub(_this, x);
}

static void *_mul(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->mul);
    return class->mul(_this, x);
}

static void *_div(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->div);
    return class->div(_this, x);
}

static void *_mod(FormWO_t x)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->mod);
    return class->mod(_this, x);
}

static void *_cast(const char *c)
{
    void *_this = pop_this();
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->cast);
    return class->cast(_this, c);
}

static void *_class_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Class, _this, app);
    struct Class *this = offsetOf(_this, __Class);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&ClassS + sizeof(ClassS._);
    voidf *end = (void*)&ClassS + sizeof(ClassS);
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

void *__cast_aux(void *_this, const char *c)
{
    const struct Class *class = offsetOf(classOf(_this), __Class);
    assert(class->cast);
    return class->cast(_this, c);
}

void initClass(void)
{
    if (!_ClassS) {
        _ClassS = (void*)&ClassS;
        memcpy((void*)&ClassS, _MetaClassS, sizeof(*_MetaClassS));
    }
    if (!__Class) {
        __Class = new(T(MetaClass), "Class",
                      T(MetaClass), sizeof(struct Class) + classSz(_MetaClass().class),
                     _MetaClassS->ctor, _class_ctor, NULL);
    }
    if (!__Object) {
        __Object = new(T(MetaClass), "Object",
                       T(MetaObject), sizeof(struct Object) + classSz(_MetaObject().class), NULL);
    }
}
