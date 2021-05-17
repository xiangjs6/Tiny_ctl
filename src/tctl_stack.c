//
// Created by xjs on 2020/9/22.
//

#include "../include/tctl_stack.h"
#include "include/_tctl_stack.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>
#define Import CLASS, OBJECT, METACLASS, DEQUE

struct StackClass {
    void *(*top)(void *_self);
    void (*push)(void *_self, const void *x);
    void (*pop)(void *_self);
    bool (*empty)(void *_self);
    size_t (*size)(void *_self);
};

struct Stack {
    Deque c;
};

//selector
static void *_top(void);
static void _push(const void *x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
//stackclass
static void *_stackclass_ctor(void *_self, va_list *app);
//stack
static void *_stack_ctor(void *_self, va_list *app);
static void *_stack_dtor(void *_self);
static void *_stack_top(const void *_self);
static void _stack_push(void *_self, const void *x);
static void _stack_pop(void *_self);
static bool _stack_empty(void *_self);
static size_t _stack_size(void *_self);

//init
static const void *__Stack = NULL;
static const void *__StackClass = NULL;
volatile static struct StackSelector StackS = {
    {0},
    _top,
    _push,
    _pop,
    _empty,
    _size
};
const struct StackSelector *_StackS = NULL;

static void initStack(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_StackS) {
        _StackS = (void*)&StackS;
        memcpy((void*)&StackS, _ClassS, sizeof(*_ClassS));
    }
    if (!__StackClass) {
        __StackClass = new(T(MetaClass), "StackClass",
                           T(Class), sizeof(struct StackClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _stackclass_ctor, NULL);
    }
    if (!__Stack) {
        __Stack = new(_StackClass(), "Stack",
                     T(Object), sizeof(struct Stack) + classSz(T(Object)),
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

const void *_Stack(void)
{
    if (!__Stack)
        initStack();
    return __Stack;
}

const void *_StackClass(void)
{
    if (!__StackClass)
        initStack();
    return __StackClass;
}

//StackClass
static void *_stackclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__StackClass, _self, app);
    struct VectorClass *self = offsetOf(_self, __StackClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&StackS + sizeof(StackS._));
    voidf *end = (voidf*)((char*)&StackS + sizeof(StackS));
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

//Stack
static void *_stack_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Stack, _self, app);
    struct Stack *self = offsetOf(_self, __Stack);
    self->c = malloc(classSz(T(Deque)));
    construct_v(_Deque(), self->c, app);
    return _self;
}

static void *_stack_dtor(void *_self)
{
    _self = super_dtor(__Stack, _self);
    struct Stack *self = offsetOf(_self, __Stack);
    destroy(self->c);
    free(self->c);
    return _self;
}

static void *_stack_top(const void *_self)
{
    struct Stack *self = offsetOf(_self, __Stack);
    return THIS(self->c).back();
}

static void _stack_push(void *_self, const void *x)
{
    struct Stack *self = offsetOf(_self, __Stack);
    THIS(self->c).push_back(x);
}

static void _stack_pop(void *_self)
{
    struct Stack *self = offsetOf(_self, __Stack);
    THIS(self->c).pop_back();
}

static bool _stack_empty(void *_self)
{
    struct Stack *self = offsetOf(_self, __Stack);
    return THIS(self->c).empty();
}

static size_t _stack_size(void *_self)
{
    struct Stack *self = offsetOf(_self, __Stack);
    return THIS(self->c).size();
}

static void *_top(void)
{
    void *_self = pop_this();
    const struct StackClass *class = offsetOf(classOf(_self), __StackClass);
    assert(class->top);
    return class->top(_self);
}

static void _push(const void *x)
{
    void *_self = pop_this();
    const struct StackClass *class = offsetOf(classOf(_self), __StackClass);
    assert(class->push);
    class->push(_self, x);
}

static void _pop(void)
{
    void *_self = pop_this();
    const struct StackClass *class = offsetOf(classOf(_self), __StackClass);
    assert(class->push);
    class->pop(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct StackClass *class = offsetOf(classOf(_self), __StackClass);
    assert(class->empty);
    return class->empty(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct StackClass *class = offsetOf(classOf(_self), __StackClass);
    assert(class->size);
    return class->size(_self);
}
