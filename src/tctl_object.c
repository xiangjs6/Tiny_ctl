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
static void *Object_ctor(void *_this, va_list *app)
{
    struct Object *this = _this;
    this->s = ((struct Object*)classOf(_this))->s;
    return _this;
}

static void *Object_dtor(void *_this)
{
    return _this;
}

static int Object_differ(const void *_this, const void *b)
{
    return _this != b;
}

static int Object_puto(const void *_this, FILE *fp)
{
    const struct MetaClass *class = classOf(_this);
    return fprintf(fp, "%s at %p\n", class -> name, _this);
}

const void *classOf(const void *_this)
{
    const struct Object *this = _this;
    assert(this && this->class);
    return this->class;
}

size_t sizeOf(const void *_this)
{
    const struct MetaClass *class = classOf(_this);
    return class->size;
}

/*
 *	MetaClass
 */
static void _push_class(const char*, void*);
static const void *_find_class(const char*);


static void *MetaClass_ctor(void *_this, va_list *app)
{
    struct MetaClass *this = _this;
    const size_t offset = offsetof(struct MetaClass, ctor);

    this->name = va_arg(*app, char*);
    _push_class(this->name, _this);
    this->super = va_arg(*app, struct MetaClass*);
    this->size = va_arg(*app, size_t);

    assert(this->super);
    memcpy((char *) this + offset, (char *) this->super + offset, sizeOf(this->super) - offset);
    *(void **) &this->_.s = this->super->_.s;
    voidf selector;
    va_list ap;
    va_copy(ap, *app);

    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf) MetaClassS->ctor)
            *(voidf *) &this->ctor = method;
        else if (selector == (voidf) MetaClassS->dtor)
            *(voidf *) &this->dtor = method;
        else if (selector == (voidf) MetaClassS->differ)
            *(voidf *) &this->differ = method;
        else if (selector == (voidf) MetaClassS->puto)
            *(voidf *) &this->puto = method;
        else if (selector == SelectorF)
            *(void **) &this->_.s = method;
    }
    va_end(ap);

    return this;
}

static void *MetaClass_dtor(void *_this)
{
    struct MetaClass *this = _this;
    fprintf(stderr, "%s: cannot destroy class\n", this->name);
    return 0;
}

const void *super(const void *_this)
{
    const struct MetaClass *this = _this;
    assert(this && this->super);
    return this->super;
}

/*
 *	object management and selectors
 */

void *_new(const char *class_name, ...)
{
    const struct MetaClass *class = _find_class(class_name);
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

void delete(void *_this)
{
    const struct MetaClass *class = classOf(_this);
    if (_this)
        free(class->dtor(_this));
}

static void *ctor(va_list *app)
{
    void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
    assert(class->ctor);
    return class->ctor(_this, app);
}

void *super_ctor(const void *_class, void *_this, va_list *app)
{
    const struct MetaClass *superclass = super(_class);
    assert(_this && superclass->ctor);
    return superclass->ctor(_this, app);
}

static void *dtor(void)
{
    void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
    assert(class->dtor);
    return class->dtor(_this);
}

void *super_dtor(const void *_class, void *_this)
{
    const struct MetaClass *superclass = super(_class);
    assert(_this && superclass->dtor);
    return superclass->dtor(_this);
}

static int differ(const void *b)
{
    const void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
    assert(class->differ);
    return class->differ(_this, b);
}

int super_differ(const void *_class, const void *_this, const void *b)
{
    const struct MetaClass *superclass = super(_class);
    assert(_this && superclass->differ);
    return superclass->differ(_this, b);
}

static int puto (FILE *fp)
{
    const void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
    assert(class->puto);
    return class->puto(_this, fp);
}

int super_puto(const void *_class, const void *_this, FILE *fp)
{
    const struct MetaClass *superclass = super(_class);
    assert(_this && superclass->puto);
    return superclass->puto(_this, fp);
}

/*
 *	initialization
 */

typeof(*MetaClassS) _MetaClassS = {ctor, dtor, differ, puto};
typeof(MetaClassS) MetaClassS = &_MetaClassS;
const void *SelectorF = &_MetaClassS;
static const struct MetaClass object [] = {
        {{&_MetaClassS, object + 1},
                "Object", object, sizeof(struct Object),
                Object_ctor, Object_dtor, Object_differ, Object_puto
        },
        {{&_MetaClassS, object + 1},
                "MetaClass",  object, sizeof(struct MetaClass),
                MetaClass_ctor,  MetaClass_dtor,  Object_differ, Object_puto
        }
};

const void * _Object = object;
const void * _MetaClass = object + 1;

struct MetaClass_node {
    const char *c_name;
    const void *c;
    struct MetaClass_node *next;
};

struct MetaClass_node _head[] = {
        {"Object", object, NULL},
        {"MetaClass", object + 1, _head}
};
static struct MetaClass_node *head = _head + 1;

static void _push_class(const char *name, void *_this)
{
    struct MetaClass_node *node = head;
    while (node)
    {
        assert(strcmp(node->c_name, name));
        node = node->next;
    }
    node = malloc(sizeof(struct MetaClass_node));
    node->next = head;
    node->c_name = name;
    node->c = _this;
    head = node;
}

static const void *_find_class(const char *name)
{
    struct MetaClass_node *node = head;
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
