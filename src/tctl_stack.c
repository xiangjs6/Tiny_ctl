//
// Created by xjs on 2020/9/22.
//

#include "../include/tctl_stack.h"
#include "include/_tctl_stack.h"
#include "include/_tctl_deque.h"
#include "../include/tctl_class.h"
#include "include/_tctl_class.h"
#include "../include/tctl_deque.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>
#define Import CLASS, OBJECT, METACLASS, DEQUE

struct StackClass {
    void *(*top)(void *_this);
    void (*push)(void *_this, FormWO_t x);
    void (*pop)(void *_this);
    bool (*empty)(void *_this);
    size_t (*size)(void *_this);
};

struct Stack {
    Deque c;
};

//selector
static void *_top(void);
static void _push(FormWO_t x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
//stackclass
static void *_stackclass_ctor(void *_this, va_list *app);
//stack
static void *_stack_ctor(void *_this, va_list *app);
static void *_stack_dtor(void *_this);
static void *_stack_top(const void *_this);
static void _stack_push(void *_this, FormWO_t x);
static void _stack_pop(void *_this);
static bool _stack_empty(void *_this);
static size_t _stack_size(void *_this);

//init
static const void *__Stack = NULL;
static const void *__StackClass = NULL;
volatile static struct StackSelector StackS = {
    {},
    _top,
    _push,
    _pop,
    _empty,
    _size
};
const struct StackSelector *_StackS = NULL;

void initStack(void)
{
    initClass();
    initIterator();
    if (!_StackS) {
        _StackS = (void*)&StackS;
        memcpy((void*)&StackS, _ClassS, sizeof(*_ClassS));
    }
    if (!__StackClass) {
        __StackClass = new(T(MetaClass), "StackClass",
                           T(Class), sizeof(struct StackClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _stackclass_ctor, NULL);
    }
    if (!__Stack) {
        __Stack = new(_StackClass(), "Stack",
                     T(Object), sizeof(struct Stack) + classSz(_Object().class),
                     _MetaClassS->ctor, _stack_ctor,
                     _MetaClassS->dtor, _stack_dtor,
                     _StackS->top, _stack_top,
                     _StackS->push, _stack_push,
                     _StackS->pop, _stack_pop,
                     _StackS->empty, _stack_empty,
                     _StackS->size, _stack_size,
                     Selector, _StackS, NULL);
    }
}

Form_t _Stack(void)
{
    Form_t t = {OBJ, .class = __Stack};
    return t;
}

Form_t _StackClass(void)
{
    Form_t t = {OBJ, .class = __StackClass};
    return t;
}

//StackClass
static void *_stackclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__StackClass, _this, app);
    struct VectorClass *this = offsetOf(_this, __StackClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&StackS + sizeof(StackS._);
    voidf *end = (void*)&StackS + sizeof(StackS);
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

//Stack
static void *_stack_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Stack, _this, app);
    struct Stack *this = offsetOf(_this, __Stack);
    this->c = malloc(classSz(_Deque().class));
    construct_v(_Deque(), this->c, app);
    return _this;
}

static void *_stack_dtor(void *_this)
{
    _this = super_dtor(__Stack, _this);
    struct Stack *this = offsetOf(_this, __Stack);
    destroy(this->c);
    free(this->c);
    return _this;
}

static void *_stack_top(const void *_this)
{
    struct Stack *this = offsetOf(_this, __Stack);
    return THIS(this->c).back();
}

static void _stack_push(void *_this, FormWO_t x)
{
    struct Stack *this = offsetOf(_this, __Stack);
    THIS(this->c).push_back(x);
}

static void _stack_pop(void *_this)
{
    struct Stack *this = offsetOf(_this, __Stack);
    THIS(this->c).pop_back();
}

static bool _stack_empty(void *_this)
{
    struct Stack *this = offsetOf(_this, __Stack);
    return THIS(this->c).empty();
}

static size_t _stack_size(void *_this)
{
    struct Stack *this = offsetOf(_this, __Stack);
    return THIS(this->c).size();
}

static void *_top(void)
{
    void *_this = pop_this();
    const struct StackClass *class = offsetOf(classOf(_this), __StackClass);
    assert(class->top);
    return class->top(_this);
}

static void _push(FormWO_t x)
{
    void *_this = pop_this();
    const struct StackClass *class = offsetOf(classOf(_this), __StackClass);
    assert(class->push);
    class->push(_this, x);
}

static void _pop(void)
{
    void *_this = pop_this();
    const struct StackClass *class = offsetOf(classOf(_this), __StackClass);
    assert(class->push);
    class->pop(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct StackClass *class = offsetOf(classOf(_this), __StackClass);
    assert(class->empty);
    return class->empty(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct StackClass *class = offsetOf(classOf(_this), __StackClass);
    assert(class->size);
    return class->size(_this);
}
