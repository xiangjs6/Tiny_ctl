//
// Created by xjs on 2020/9/29.
//

#include "../include/tctl_heap.h"
#include "include/_tctl_priority_queue.h"
#include "../include/tctl_vector.h"
#include <memory.h>
#include <stdlib.h>
#include "../include/auto_release_pool.h"

#define Import CLASS, OBJECT, METACLASS, VECTOR

struct Priority_QueueClass {
    void *(*top)(const void *_this);
    void (*push)(void *_this, FormWO_t x);
    void (*pop)(void *_this);
    bool (*empty)(void *_this);
    size_t (*size)(void *_this);
    void (*swap)(void *_this, Priority_Queue);
};

struct Priority_Queue {
    Vector c;
    Compare cmp;
};

//selector
static void *_top(void);
static void _push(FormWO_t x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
static void _swap(Priority_Queue que);
//Priority_Queueclass
static void *_priority_queueclass_ctor(void *_this, va_list *app);
//Priority_Queue
static void *_priority_queue_ctor(void *_this, va_list *app);
static void *_priority_queue_dtor(void *_this);
static void *_priority_queue_top(const void *_this);
static void _priority_queue_push(void *_this, FormWO_t x);
static void _priority_queue_pop(void *_this);
static bool _priority_queue_empty(void *_this);
static size_t _priority_queue_size(void *_this);
static void _priority_queue_swap(void *_this, Priority_Queue _que);

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

void initPriority_Queue(void)
{
    initClass();
    initIterator();
    initVector();
    if (!_Priority_QueueS) {
        _Priority_QueueS = (void*)&Priority_QueueS;
        memcpy((void*)&Priority_QueueS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Priority_QueueClass) {
        __Priority_QueueClass = new(T(MetaClass), "Priority_QueueClass",
                           T(Class), sizeof(struct Priority_QueueClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _priority_queueclass_ctor, NULL);
    }
    if (!__Priority_Queue) {
        __Priority_Queue = new(_Priority_QueueClass(), "Priority_Queue",
                     T(Object), sizeof(struct Priority_Queue) + classSz(_Object().class),
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

Form_t _Priority_Queue(void)
{
    Form_t t = {OBJ, .class = __Priority_Queue};
    return t;
}

Form_t _Priority_QueueClass(void)
{
    Form_t t = {OBJ, .class = __Priority_QueueClass};
    return t;
}

//Priority_QueueClass
static void *_priority_queueclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Priority_QueueClass, _this, app);
    struct VectorClass *this = offsetOf(_this, __Priority_QueueClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Priority_QueueS + sizeof(Priority_QueueS._);
    voidf *end = (void*)&Priority_QueueS + sizeof(Priority_QueueS);
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

//Priority_Queue
static void *_priority_queue_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Priority_Queue, _this, app);
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    this->c = malloc(classSz(_Vector().class));
    construct_v(_Vector(), this->c, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == FUNC);
    this->cmp = t.mem;
    return _this;
}

static void *_priority_queue_dtor(void *_this)
{
    _this = super_dtor(__Priority_Queue, _this);
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    destroy(this->c);
    free(this->c);
    return _this;
}

static void *_priority_queue_top(const void *_this)
{
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    return THIS(this->c).front();
}

static void _priority_queue_push(void *_this, FormWO_t x)
{
    ARP_CreatePool();
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    THIS(this->c).push_back(x);
    push_heap(THIS(this->c).begin(), THIS(this->c).end(), this->cmp);
    ARP_FreePool();
}

static void _priority_queue_pop(void *_this)
{
    ARP_CreatePool();
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    pop_heap(THIS(this->c).begin(), THIS(this->c).end(), this->cmp);
    THIS(this->c).pop_back();
    ARP_FreePool();
}

static bool _priority_queue_empty(void *_this)
{
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    return THIS(this->c).empty();
}

static size_t _priority_queue_size(void *_this)
{
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    return THIS(this->c).size();
}

static void _priority_queue_swap(void *_this, Priority_Queue _que)
{
    struct Priority_Queue *this = offsetOf(_this, __Priority_Queue);
    struct Priority_Queue *que = offsetOf(_que, __Priority_Queue);
    THIS(this->c).swap(que->c);
    Compare tmp = this->cmp;
    this->cmp = que->cmp;
    que->cmp = tmp;
}

static void *_top(void)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->top);
    return class->top(_this);
}

static void _push(FormWO_t x)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->push);
    class->push(_this, x);
}

static void _pop(void)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->push);
    class->pop(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->empty);
    return class->empty(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->size);
    return class->size(_this);
}

static void _swap(Priority_Queue que)
{
    void *_this = pop_this();
    const struct Priority_QueueClass *class = offsetOf(classOf(_this), __Priority_QueueClass);
    assert(class->swap);
    class->swap(_this, que);
}
