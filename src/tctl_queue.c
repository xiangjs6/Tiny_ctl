//
// Created by xjs on 2020/9/22.
//

#include "include/_tctl_metaclass.h"
#include "include/_tctl_queue.h"
#include "include/_tctl_deque.h"
#include "../include/tctl_class.h"
#include "include/_tctl_class.h"
#include "../include/tctl_deque.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>

#define Import CLASS, OBJECT, METACLASS, DEQUE

struct QueueClass {
    void *(*front)(const void *_this);
    void *(*back)(const void *_this);
    void (*push)(void *_this, FormWO_t x);
    void (*pop)(void *_this);
    bool (*empty)(void *_this);
    size_t (*size)(void *_this);
};

struct Queue {
    Deque c;
};

//selector
static void *_front(void);
static void *_back(void);
static void _push(FormWO_t x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
//queueclass
static void *_queueclass_ctor(void *_this, va_list *app);
//queue
static void *_queue_ctor(void *_this, va_list *app);
static void *_queue_dtor(void *_this);
static void *_queue_front(const void *_this);
static void *_queue_back(const void *_this);
static void _queue_push(void *_this, FormWO_t x);
static void _queue_pop(void *_this);
static bool _queue_empty(void *_this);
static size_t _queue_size(void *_this);

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
                           T(Class), sizeof(struct QueueClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _queueclass_ctor, NULL);
    }
    if (!__Queue) {
        __Queue = new(_QueueClass(), "Queue",
                     T(Object), sizeof(struct Queue) + classSz(_Object().class),
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

Form_t _Queue(void)
{
    if (!__Queue)
        initQueue();
    return (Form_t){OBJ, .class = __Queue};
}

Form_t _QueueClass(void)
{
    if (!__QueueClass)
        initQueue();
    return (Form_t){OBJ, .class = __QueueClass};
}

//QueueClass
static void *_queueclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__QueueClass, _this, app);
    struct VectorClass *this = offsetOf(_this, __QueueClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&QueueS + sizeof(QueueS._);
    voidf *end = (void*)&QueueS + sizeof(QueueS);
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

//Queue
static void *_queue_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Queue, _this, app);
    struct Queue *this = offsetOf(_this, __Queue);
    this->c = malloc(classSz(_Deque().class));
    construct_v(_Deque(), this->c, app);
    return _this;
}

static void *_queue_dtor(void *_this)
{
    _this = super_dtor(__Queue, _this);
    struct Queue *this = offsetOf(_this, __Queue);
    destroy(this->c);
    free(this->c);
    return _this;
}


static void *_queue_front(const void *_this)
{
    struct Queue *this = offsetOf(_this, __Queue);
    return THIS(this->c).front();
}

static void *_queue_back(const void *_this)
{
    struct Queue *this = offsetOf(_this, __Queue);
    return THIS(this->c).back();
}

static void _queue_push(void *_this, FormWO_t x)
{
    struct Queue *this = offsetOf(_this, __Queue);
    THIS(this->c).push_back(x);
}

static void _queue_pop(void *_this)
{
    struct Queue *this = offsetOf(_this, __Queue);
    THIS(this->c).pop_front();
}

static bool _queue_empty(void *_this)
{
    struct Queue *this = offsetOf(_this, __Queue);
    return THIS(this->c).empty();
}

static size_t _queue_size(void *_this)
{
    struct Queue *this = offsetOf(_this, __Queue);
    return THIS(this->c).size();
}

static void *_front(void)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->front);
    return class->front(_this);
}

static void *_back(void)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->back);
    return class->back(_this);
}

static void _push(FormWO_t x)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->push);
    class->push(_this, x);
}

static void _pop(void)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->push);
    class->pop(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->empty);
    return class->empty(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct QueueClass *class = offsetOf(classOf(_this), __QueueClass);
    assert(class->size);
    return class->size(_this);
}

