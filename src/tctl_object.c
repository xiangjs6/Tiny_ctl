//
// Created by xjs on 2020/8/31.
//

#include "../include/tctl_allocator.h"
#include "../include/tctl_portable.h"
#include "../include/tctl_object.h"
#include "_tctl_object.h"
#include <memory.h>
#include <assert.h>
#include <stdlib.h>

/*
 *	Object
 */
static void *Object_ctor(void *_self, va_list *app)
{
    struct Object *self = _self;
    self->s = ((struct Object*)classOf(_self))->s;
    return _self;
}

static void *Object_dtor(void *_self)
{
    return _self;
}

static int Object_differ(const void *_self, const void *b)
{
    return _self != b;
}

static int Object_puto(const void *_self, FILE *fp)
{
    const struct Class *class = classOf(_self);
    return fprintf(fp, "%s at %p\n", class -> name, _self);
}

const void *classOf(const void *_self)
{
    const struct Object *self = _self;
    assert(self && self->class);
    return self->class;
}

size_t sizeOf(const void *_self)
{
    const struct Class *class = classOf(_self);
    return class->size;
}

/*
 *	Class
 */
static void _push_class(const char*, void*);
static const void *_find_class(const char*);


static void *Class_ctor(void *_self, va_list *app)
{
    struct Class *self = _self;
    const size_t offset = offsetof(struct Class, ctor);

    self->name = va_arg(*app, char*);
    _push_class(self->name, _self);
    self->super = va_arg(*app, struct Class*);
    self->size = va_arg(*app, size_t);

    assert(self->super);
    memcpy((char *) self + offset, (char *) self->super + offset, sizeOf(self->super) - offset);
    *(void **) &self->_.s = self->super->_.s;
    voidf selector;
    va_list ap;
    va_copy(ap, *app);

    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf) ClassS->ctor)
            *(voidf *) &self->ctor = method;
        else if (selector == (voidf) ClassS->dtor)
            *(voidf *) &self->dtor = method;
        else if (selector == (voidf) ClassS->differ)
            *(voidf *) &self->differ = method;
        else if (selector == (voidf) ClassS->puto)
            *(voidf *) &self->puto = method;
        else if (selector == SelectorF)
            *(void **) &self->_.s = method;
    }
    va_end(ap);

    return self;
}

static void *Class_dtor(void *_self)
{
    struct Class *self = _self;
    fprintf(stderr, "%s: cannot destroy class\n", self->name);
    return 0;
}

const void *super(const void *_self)
{
    const struct Class *self = _self;
    assert(self && self->super);
    return self->super;
}

/*
 *	object management and selectors
 */

void *_new(const char *class_name, ...)
{
    const struct Class *class = _find_class(class_name);
    struct Object *object;
    va_list ap;

    assert(class && class->size);
    object = calloc(1, class->size);
    assert(object);
    object->class = class;
    va_start(ap, class_name);
    object = class->ctor(object, &ap);
    va_end(ap);
    return object;
}

void delete(void *_self)
{
    const struct Class *class = classOf(_self);
    if (_self)
        free(class->dtor(_self));
}

static void *ctor(void *_self, va_list *app)
{
    const struct Class * class = classOf(_self);
    assert(class->ctor);
    return class->ctor(_self, app);
}

void * super_ctor(const void *_class, void *_self, va_list *app)
{
    const struct Class *superclass = super(_class);
    assert(_self && superclass->ctor);
    return superclass->ctor(_self, app);
}

static void *dtor(void *_self)
{
    const struct Class *class = classOf(_self);
    assert(class->dtor);
    return class->dtor(_self);
}

void *super_dtor(const void *_class, void *_self)
{
    const struct Class *superclass = super(_class);
    assert(_self && superclass->dtor);
    return superclass->dtor(_self);
}

static int differ(const void *_self, const void *b)
{
    const struct Class *class = classOf(_self);
    assert(class->differ);
    return class->differ(_self, b);
}

int super_differ(const void *_class, const void *_self, const void *b)
{
    const struct Class *superclass = super(_class);
    assert(_self && superclass->differ);
    return superclass->differ(_self, b);
}

static int puto (const void *_self, FILE *fp)
{
    const struct Class *class = classOf(_self);
    assert(class->puto);
    return class->puto(_self, fp);
}

int super_puto(const void *_class, const void *_self, FILE *fp)
{
    const struct Class *superclass = super(_class);
    assert(_self && superclass->puto);
    return superclass->puto(_self, fp);
}

/*
 *	initialization
 */

typeof(*ClassS) _ClassS = {ctor, dtor, differ, puto};
typeof(ClassS) ClassS = &_ClassS;
const void *SelectorF = &_ClassS;
static const struct Class object [] = {
        {{&_ClassS, object + 1},
                "Object", object, sizeof(struct Object),
                Object_ctor, Object_dtor, Object_differ, Object_puto
        },
        {{&_ClassS, object + 1},
                "Class",  object, sizeof(struct Class),
                Class_ctor,  Class_dtor,  Object_differ, Object_puto
        }
};

const void * _Object = object;
const void * _Class = object + 1;

struct Class_node {
    const char *c_name;
    const void *c;
    struct Class_node *next;
};

struct Class_node _head[] = {
        {"Object", object, NULL},
        {"Class", object + 1, _head}
};
static struct Class_node *head = _head + 1;

static void _push_class(const char *name, void *_self)
{
    struct Class_node *node = head;
    while (node)
    {
        assert(strcmp(node->c_name, name));
        node = node->next;
    }
    node = malloc(sizeof(struct Class_node));
    node->next = head;
    node->c_name = name;
    node->c = _self;
    head = node;
}

static const void *_find_class(const char *name)
{
    struct Class_node *node = head;
    while (node)
    {
        if (!strcmp(node->c_name, name))
            break;
        node = node->next;
    }
    assert(node);
    return node->c;
}

//this指针
static thread_key_t this_key;
static thread_once_t this_key_once = THREAD_ONCE_INIT;

struct this_node {
    void *this;
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

void *push_this(void *p)
{
    struct this_stack *ptr;
    thread_once(&this_key_once, make_this_key);
    if ((ptr = thread_getspecific(this_key)) == NULL) {
        ptr = allocate(sizeof(struct this_stack));
        ptr->head = NULL;
        thread_setspecific(this_key, ptr);
    }
    struct this_node *node = allocate(sizeof(struct this_node));
    node->this = p;
    node->next = ptr->head;
    ptr->head = node;
    return p;
}

void *pop_this(void)
{
    struct this_stack *ptr;
    pthread_once(&this_key_once, make_this_key);
    if ((ptr = pthread_getspecific(this_key)) == NULL)
        return NULL;

    struct this_node *node = ptr->head;
    void *p = node->this;
    ptr->head = node->next;
    deallocate(node, sizeof(struct this_node));
    return p;
}
