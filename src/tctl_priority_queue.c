//
// Created by xjs on 2020/9/29.
//

#include "../include/tctl_heap.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "include/_tctl_priority_queue.h"
#include <memory.h>
#include <stdlib.h>

#define Import CLASS, OBJECT, METACLASS, VECTOR, ANY

struct Priority_QueueClass {
    void *(*top)(const void *_self);
    void (*push)(void *_self, const void *x);
    void (*pop)(void *_self);
    bool (*empty)(void *_self);
    size_t (*size)(void *_self);
    void (*swap)(void *_self, Priority_Queue);
};

struct Priority_Queue {
    Vector c;
    Compare cmp;
};

//selector
static void *_top(void);
static void _push(const void *x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
static void _swap(Priority_Queue que);
//Priority_Queueclass
static void *_priority_queueclass_ctor(void *_self, va_list *app);
//Priority_Queue
static void *_priority_queue_ctor(void *_self, va_list *app);
static void *_priority_queue_dtor(void *_self);
static void *_priority_queue_top(const void *_self);
static void _priority_queue_push(void *_self, const void *x);
static void _priority_queue_pop(void *_self);
static bool _priority_queue_empty(void *_self);
static size_t _priority_queue_size(void *_self);
static void _priority_queue_swap(void *_self, Priority_Queue _que);

//init
static const void *__Priority_Queue = NULL;
static const void *__Priority_QueueClass = NULL;
volatile static struct Priority_QueueSelector Priority_QueueS = {
    {},
    _top,
    _push,
    _pop,
    _empty,
    _size,
    _swap
};
const struct Priority_QueueSelector *_Priority_QueueS = NULL;

static void initPriority_Queue(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Priority_QueueS) {
        _Priority_QueueS = (void*)&Priority_QueueS;
        memcpy((void*)&Priority_QueueS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Priority_QueueClass) {
        __Priority_QueueClass = new(T(MetaClass), "Priority_QueueClass",
                           T(Class), sizeof(struct Priority_QueueClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _priority_queueclass_ctor, NULL);
    }
    if (!__Priority_Queue) {
        __Priority_Queue = new(_Priority_QueueClass(), "Priority_Queue",
                     T(Object), sizeof(struct Priority_Queue) + classSz(T(Object)),
                     _MetaClassS->ctor, _priority_queue_ctor,
                     _MetaClassS->dtor, _priority_queue_dtor,
                     _Priority_QueueS->top, _priority_queue_top,
                     _Priority_QueueS->push, _priority_queue_push,
                     _Priority_QueueS->pop, _priority_queue_pop,
                     _Priority_QueueS->empty, _priority_queue_empty,
                     _Priority_QueueS->size, _priority_queue_size,
                     _Priority_QueueS->swap, _priority_queue_swap,
                     Selector, _Priority_QueueS, NULL);
    }
}

const void *_Priority_Queue(void)
{
    if (!__Priority_Queue)
        initPriority_Queue();
    return __Priority_Queue;
}

const void *_Priority_QueueClass(void)
{
    if (!__Priority_QueueClass)
        initPriority_Queue();
    return __Priority_QueueClass;
}

//Priority_QueueClass
static void *_priority_queueclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Priority_QueueClass, _self, app);
    struct VectorClass *self = offsetOf(_self, __Priority_QueueClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Priority_QueueS + sizeof(Priority_QueueS._);
    voidf *end = (void*)&Priority_QueueS + sizeof(Priority_QueueS);
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

//Priority_Queue
static void *_priority_queue_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Priority_Queue, _self, app);
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    self->c = malloc(classSz(T(Vector)));
    construct_v(_Vector(), self->c, app);
    void *t = va_arg(*app, void*);
    assert(classOf(t) == T(Any));
    self->cmp = *(void**)THIS((Any)t).value();
    return _self;
}

static void *_priority_queue_dtor(void *_self)
{
    _self = super_dtor(__Priority_Queue, _self);
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    destroy(self->c);
    free(self->c);
    return _self;
}

static void *_priority_queue_top(const void *_self)
{
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    return THIS(self->c).front();
}

static void _priority_queue_push(void *_self, const void *x)
{
    ARP_CreatePool();
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    THIS(self->c).push_back(x);
    push_heap(THIS(self->c).begin(), THIS(self->c).end(), self->cmp);
    ARP_FreePool();
}

static void _priority_queue_pop(void *_self)
{
    ARP_CreatePool();
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    pop_heap(THIS(self->c).begin(), THIS(self->c).end(), self->cmp);
    THIS(self->c).pop_back();
    ARP_FreePool();
}

static bool _priority_queue_empty(void *_self)
{
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    return THIS(self->c).empty();
}

static size_t _priority_queue_size(void *_self)
{
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    return THIS(self->c).size();
}

static void _priority_queue_swap(void *_self, Priority_Queue _que)
{
    struct Priority_Queue *self = offsetOf(_self, __Priority_Queue);
    struct Priority_Queue *que = offsetOf(_que, __Priority_Queue);
    THIS(self->c).swap(que->c);
    Compare tmp = self->cmp;
    self->cmp = que->cmp;
    que->cmp = tmp;
}

static void *_top(void)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->top);
    return class->top(_self);
}

static void _push(const void *x)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->push);
    class->push(_self, x);
}

static void _pop(void)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->push);
    class->pop(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->empty);
    return class->empty(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->size);
    return class->size(_self);
}

static void _swap(Priority_Queue que)
{
    void *_self = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_self), __Priority_QueueClass);
    assert(class->swap);
    class->swap(_self, que);
}
