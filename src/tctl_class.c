//
// Created by xjs on 2020/12/30.
//

#include "_tctl_class.h"
#include <memory.h>
#include <assert.h>
#define Import METACLASS

static bool equal(const void *x);
static int cmp(const void *x);
static void *at(int x);
static void inc(void);
static void dec(void);
static void self_add(const void *x);
static void self_sub(const void *x);
static void asign(const void *x);
static void *add(const void *x);
static void *sub(const void *x);
static void *mul(const void *x);
static void *div(const void *x);
static void *mod(const void *x);
//init
volatile static typeof(*_ClassS) ClassS = {
        {},
        equal,
        cmp,
        at,
        inc,
        dec,
        self_add,
        self_sub,
        asign,
        add,
        sub,
        mul,
        div,
        mod
};
typeof(_ClassS) _ClassS = NULL;
const void *_Class = NULL;

//selector
static bool equal(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->equal);
    return class->equal(_this, x);
}
static int cmp(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->cmp);
    return class->cmp(_this, x);
}
static void *at(int x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->at);
    return class->at(_this, x);
}
static void inc(void)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->inc);
    return class->inc(_this);
}
static void dec(void)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->dec);
    return class->dec(_this);
}
static void self_add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->self_add);
    class->self_add(_this, x);
}
static void self_sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->self_sub);
    class->self_sub(_this, x);
}
static void asign(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->asign);
    class->asign(_this, x);
}
static void *add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->add);
    return class->add(_this, x);
}
static void *sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->sub);
    return class->sub(_this, x);
}
static void *mul(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->mul);
    return class->mul(_this, x);
}
static void *div(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->div);
    return class->div(_this, x);
}
static void *mod(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->mod);
    return class->mod(_this, x);
}

static void *_ctor(void *_this, va_list *app)
{
    struct Class *this = super_ctor(_Class, _this, app);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&ClassS + sizeof(ClassS._);
    voidf *end = (void*)&ClassS + sizeof(ClassS);
    voidf *this_begin = (void*)this + sizeof(this->_);
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
    return this;
}


void initClass(void)
{
    if (!_ClassS) {
        _ClassS = (void*)&ClassS;
        memcpy((void*)&ClassS, _MetaClassS, sizeof(*_MetaClassS));
    }
    if (!_Class) {
        _Class = new(MetaClass, "Class",
                     _MetaClass, sizeof(struct Class),
                     _MetaClassS->ctor, _ctor);
    }
}