//
// Created by xjs on 2020/9/22.
//

#include "include/_tctl_metaclass.h"
#include "include/_tctl_queue.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>

#define Import CLASS, OBJECT, METACLASS, DEQUE

struct QueueClass {
    void *(*front)(const void *_self);
    void *(*back)(const void *_self);
    void (*push)(void *_self, const void *x);
    void (*pop)(void *_self);
    bool (*empty)(void *_self);
    size_t (*size)(void *_self);
};

struct Queue {
    Deque c;
};

//selector
static void *_front(void);
static void *_back(void);
static void _push(const void *x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
//queueclass
static void *_queueclass_ctor(void *_self, va_list *app);
//queue
static void *_queue_ctor(void *_self, va_list *app);
static void *_queue_dtor(void *_self);
static void *_queue_front(const void *_self);
static void *_queue_back(const void *_self);
static void _queue_push(void *_self, const void *x);
static void _queue_pop(void *_self);
static bool _queue_empty(void *_self);
static size_t _queue_size(void *_self);

//init
static const void *__Queue = NULL;
static const void *__QueueClass = NULL;
volatile static struct QueueSelector QueueS = {
    {},
    _front,
    _back,
    _push,
    _pop,
    _empty,
    _size
};
const struct QueueSelector *_QueueS = NULL;

static void initQueue(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_QueueS) {
        _QueueS = (void*)&QueueS;
        memcpy((void*)&QueueS, _ClassS, sizeof(*_ClassS));
    }
    if (!__QueueClass) {
        __QueueClass = new(T(MetaClass), "QueueClass",
                           T(Class), sizeof(struct QueueClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _queueclass_ctor, NULL);
    }
    if (!__Queue) {
        __Queue = new(_QueueClass(), "Queue",
                     T(Object), sizeof(struct Queue) + classSz(T(Object)),
                     _MetaClassS->ctor, _queue_ctor,
                     _MetaClassS->dtor, _queue_dtor,
                     _QueueS->front, _queue_front,
                     _QueueS->back, _queue_back,
                     _QueueS->push, _queue_push,
                     _QueueS->pop, _queue_pop,
                     _QueueS->empty, _queue_empty,
                     _QueueS->size, _queue_size,
                     Selector, _QueueS, NULL);
    }
}

const void *_Queue(void)
{
    if (!__Queue)
        initQueue();
    return __Queue;
}

const void *_QueueClass(void)
{
    if (!__QueueClass)
        initQueue();
    return __QueueClass;
}

//QueueClass
static void *_queueclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__QueueClass, _self, app);
    struct VectorClass *self = offsetOf(_self, __QueueClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&QueueS + sizeof(QueueS._);
    voidf *end = (void*)&QueueS + sizeof(QueueS);
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

//Queue
static void *_queue_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Queue, _self, app);
    struct Queue *self = offsetOf(_self, __Queue);
    self->c = malloc(classSz(T(Deque)));
    construct_v(_Deque(), self->c, app);
    return _self;
}

static void *_queue_dtor(void *_self)
{
    _self = super_dtor(__Queue, _self);
    struct Queue *self = offsetOf(_self, __Queue);
    destroy(self->c);
    free(self->c);
    return _self;
}


static void *_queue_front(const void *_self)
{
    struct Queue *self = offsetOf(_self, __Queue);
    return THIS(self->c).front();
}

static void *_queue_back(const void *_self)
{
    struct Queue *self = offsetOf(_self, __Queue);
    return THIS(self->c).back();
}

static void _queue_push(void *_self, const void *x)
{
    struct Queue *self = offsetOf(_self, __Queue);
    THIS(self->c).push_back(x);
}

static void _queue_pop(void *_self)
{
    struct Queue *self = offsetOf(_self, __Queue);
    THIS(self->c).pop_front();
}

static bool _queue_empty(void *_self)
{
    struct Queue *self = offsetOf(_self, __Queue);
    return THIS(self->c).empty();
}

static size_t _queue_size(void *_self)
{
    struct Queue *self = offsetOf(_self, __Queue);
    return THIS(self->c).size();
}

static void *_front(void)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->front);
    return class->front(_self);
}

static void *_back(void)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->back);
    return class->back(_self);
}

static void _push(const void *x)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->push);
    class->push(_self, x);
}

static void _pop(void)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->push);
    class->pop(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->empty);
    return class->empty(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_self), __QueueClass);
    assert(class->size);
    return class->size(_self);
}

