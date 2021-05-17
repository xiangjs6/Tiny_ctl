//
// Created by xjs on 2020/8/31.
//

#include "../include/tctl_allocator.h"
#include "../include/tctl_portable.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_def.h"
#include "include/_tctl_metaclass.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


struct MetaObject {
    const void *s;
    const struct MetaClass *class;	/* object's description */
};

struct MetaClass {
    const struct MetaObject _;
    const char *name;
    const struct MetaClass *super;
    size_t size;
    void *(*ctor)(void *self, va_list *app);
    void *(*dtor)(void *self);
    int (*differ)(const void *self, const void *b);
    int (*puto)(const void *self, FILE *fp);
    void *(*cast)(const void *self, const void *class);
};
/*
 *	initialization
 */

static void *_ctor(void *mem, ...);
static void *_dtor(void);
static int _differ(const void *b);
static int _puto (FILE *fp);
static void *_cast(const void *class);
static void *metaobject_ctor(void *_self, va_list *app);
static void *metaobject_dtor(void *_self);
static int metaobject_differ(const void *_self, const void *b);
static int metaobject_puto(const void *_self, FILE *fp);
static void *metaclass_ctor(void *_self, va_list *app);
static void *metaclass_dtor(void *_self);

static struct MetaClassSelector MetaClassS = {_ctor, _dtor, _differ, _puto, _cast};
const struct MetaClassSelector *_MetaClassS = &MetaClassS;
const void *Selector = &MetaClassS;
static const struct MetaClass _object[] = {
        {{&MetaClassS, _object + 1},
                "MetaObject", _object, sizeof(struct MetaObject),
                metaobject_ctor, metaobject_dtor, metaobject_differ, metaobject_puto
        },
        {{&MetaClassS, _object + 1},
                "MetaClass",  _object, sizeof(struct MetaClass),
                metaclass_ctor,  metaclass_dtor,  metaobject_differ, metaobject_puto
        }
};

static const void *__MetaObject = _object;
static const void *__MetaClass = _object + 1;

const void *_MetaObject(void)
{
    return __MetaObject;
}

const void *_MetaClass(void)
{
    return __MetaClass;
}
/*
 *	Object
 */
static void *metaobject_ctor(void *_self, va_list *app)
{
    struct MetaObject *self = _self;
    self->s = ((struct MetaObject*)classOf(_self))->s;
    return _self;
}

static void *metaobject_dtor(void *_self)
{
    return _self;
}

static int metaobject_differ(const void *_self, const void *b)
{
    return _self != b;
}

static int metaobject_puto(const void *_self, FILE *fp)
{
    const struct MetaClass *class = classOf(_self);
    return fprintf(fp, "%s at %p\n", class -> name, _self);
}

const void *classOf(const void *_obj)
{
    const struct MetaObject *obj = _obj;
    assert(obj && obj->class);
    return obj->class;
}

size_t sizeOf(const void *_obj)
{
    const struct MetaClass *class = classOf(_obj);
    return class->size;
}

size_t classSz(const void *obj)
{
    const struct MetaClass *class = obj;
    return class->size;
}

void *offsetOf(const void *obj, const void *_class)
{
    const struct MetaClass *class = _class;
    return (char*)obj + classSz(class->super);
}

bool class_check(const void *obj, const void *class)
{
    const void *obj_class = classOf(obj);
    while (obj_class != class)
    {
        if (obj_class == __MetaObject)
            return false;
        obj_class = super(obj_class);
    }
    return true;
}

/*
 *	MetaClass
 */

static void *metaclass_ctor(void *_self, va_list *app)
{
    struct MetaClass *self = _self;
    const size_t offset = offsetof(struct MetaClass, ctor);

    self->name = va_arg(*app, char*);
    self->super = va_arg(*app, void*);
    assert(self->super);
    self->size = va_arg(*app, size_t);

    assert(self->super);
    memcpy((char*)self + offset, (char*)self->super + offset, sizeOf(self->super) - offset);
    *(void const **)&self->_.s = self->super->_.s;
    voidf selector;
    va_list ap;
    va_copy(ap, *app);

    while ((selector = va_arg(ap, voidf)))
    {
        void *method = va_arg(ap, void*);
        if (selector == (voidf)MetaClassS.ctor)
            *(void**)&self->ctor = method;
        else if (selector == (voidf)MetaClassS.dtor)
            *(void**)&self->dtor = method;
        else if (selector == (voidf)MetaClassS.differ)
            *(void**)&self->differ = method;
        else if (selector == (voidf)MetaClassS.puto)
            *(void**)&self->puto = method;
        else if (selector == (voidf)MetaClassS.cast)
            *(void**)&self->cast = method;
        else if (selector == *(voidf*)&Selector)
            *(void**)&self->_.s = method;
    }
    va_end(ap);
    return _self;
}

static void *metaclass_dtor(void *_self)
{
    struct MetaClass *self = _self;
    fprintf(stderr, "%s: cannot destroy class\n", self->name);
    return 0;
}

const void *super(const void *_self)
{
    const struct MetaClass *self = _self;
    assert(self && self->super);
    return self->super;
}

/*
 *	object management and selectors
 */

void *new(const void *_class, ...)
{
    const struct MetaClass *class = _class;
    struct MetaObject *object;
    va_list ap;
    assert(class && class->size);
    object = calloc(1, class->size);
    assert(object);
    object->class = class;
    va_start(ap, _class);
    object = class->ctor(object, &ap);
    va_end(ap);
    return object;
}

void delete(void *_obj)
{
    const struct MetaClass *class = classOf(_obj);
    free(class->dtor(_obj));
}

void *construct_v(const void *_class, void *mem, va_list *app)
{
    assert(mem);
    const struct MetaClass *class = _class;
    struct MetaObject *object = mem;
    assert(class && class->size);
    assert(object);
    object->class = class;
    object = class->ctor(object, app);
    return object;
}

void *construct(const void *_class, void *mem, ...)
{
    va_list ap;
    va_start(ap, mem);
    struct MetaObject *object = construct_v(_class, mem, &ap);
    va_end(ap);
    return object;
}

void destroy(void *_obj)
{
    struct MetaObject *obj = _obj;
    obj->class->dtor(_obj);
}

static void *_ctor(void *mem, ...)
{
    void *_self = pop_this();
    const struct MetaClass *class = classOf(_self);
    assert(class->ctor);
    va_list ap;
    va_start(ap, mem);
    struct MetaObject *object = mem ? mem : calloc(1, class->size);
    assert(object);
    object->class = class;
    object = class->ctor(object, &ap);
    va_end(ap);
    return object;
}


static void *_dtor(void)
{
    void *_self = pop_this();
    const struct MetaClass *class = classOf(_self);
    assert(class->dtor);
    return class->dtor(_self);
}

static void *_cast(const void *_class)
{
    void *_self = pop_this();
    const struct MetaClass *class = classOf(_self);
    assert(class->cast);
    return class->cast(_self, _class);
}

static int _differ(const void *b)
{
    const void *_self = pop_this();
    const struct MetaClass *class = classOf(_self);
    assert(class->differ);
    return class->differ(_self, b);
}

static int _puto(FILE *fp)
{
    const void *_self = pop_this();
    const struct MetaClass *class = classOf(_self);
    assert(class->puto);
    return class->puto(_self, fp);
}

void *super_ctor(const void *_class, void *_self, va_list *app)
{
    const struct MetaClass *superclass = super(_class);
    assert(_self && superclass->ctor);
    return superclass->ctor(_self, app);
}

void *super_dtor(const void *_class, void *_self)
{
    const struct MetaClass *superclass = super(_class);
    assert(_self && superclass->dtor);
    return superclass->dtor(_self);
}

//this指针
static thread_key_t this_key;
static thread_once_t this_key_once = THREAD_ONCE_INIT;

struct this_node {
    const void *self;
    struct this_node *next;
};
struct this_stack {
    struct this_node *head;
};

static void free_this(void *p)
{
    deallocate(p, sizeof(struct this_stack));
}

static void make_this_key(void)
{
    thread_key_create(&this_key, free_this);
}

void push_this(const void *p)
{
    struct this_stack *ptr;
    thread_once(&this_key_once, make_this_key);
    if ((ptr = thread_getspecific(this_key)) == NULL) {
        ptr = allocate(sizeof(struct this_stack));
        ptr->head = NULL;
        thread_setspecific(this_key, ptr);
    }
    struct this_node *node = allocate(sizeof(struct this_node));
    node->self = p;
    node->next = ptr->head;
    ptr->head = node;
}

void *pop_this(void)
{
    struct this_stack *ptr;
    pthread_once(&this_key_once, make_this_key);
    if ((ptr = pthread_getspecific(this_key)) == NULL)
        return NULL;

    struct this_node *node = ptr->head;
    const void *p = node->self;
    ptr->head = node->next;
    deallocate(node, sizeof(struct this_node));
    return (void*)p;
}

