//
// Created by xjs on 2020/8/31.
//

#include "../include/tctl_allocator.h"
#include "../include/tctl_portable.h"
#include "include/_tctl_object.h"
#include <memory.h>
#include <assert.h>
#include <stdlib.h>


struct Object {
    const void *s;
    const struct MetaClass *class;	/* object's description */
};

struct MetaClass {
    const struct Object _;			/* class' description */
    const char *name;				/* class' name */
    const struct MetaClass *super;		/* class' super class */
    size_t size;					/* class' object's size */
    void *(*ctor)(void *this, va_list *app);
    void *(*dtor)(void *this);
    int (*differ)(const void *this, const void *b);
    int (*puto)(const void *this, FILE *fp);
};
/*
 *	initialization
 */

static void *ctor(void *mem, ...);
static void *dtor(void);
static int differ(const void *b);
static int puto (FILE *fp);
static void *Object_ctor(void *_this, va_list *app);
static void *Object_dtor(void *_this);
static int Object_differ(const void *_this, const void *b);
static int Object_puto(const void *_this, FILE *fp);
static void *MetaClass_ctor(void *_this, va_list *app);
static void *MetaClass_dtor(void *_this);

static struct MetaClassSelector MetaClassS = {ctor, dtor, differ, puto};
const struct MetaClassSelector *_MetaClassS = &MetaClassS;
const void *Selector = &MetaClassS;
static const struct MetaClass _object[] = {
        {{&MetaClassS, _object + 1},
                "Object", _object, sizeof(struct Object),
                Object_ctor, Object_dtor, Object_differ, Object_puto
        },
        {{&MetaClassS, _object + 1},
                "MetaClass",  _object, sizeof(struct MetaClass),
                MetaClass_ctor,  MetaClass_dtor,  Object_differ, Object_puto
        }
};

static const void *__Object = _object;
static const void *__MetaClass = _object + 1;

Form_t _Object(void)
{
    Form_t t = {OBJ , {.class = __Object}};
    return t;
}

Form_t _MetaClass(void)
{
    Form_t t = {OBJ , {.class = __MetaClass}};
    return t;
}
/*
 *	Object
 */
static void *Object_ctor(void *_this, va_list *app)
{
    struct Object *this = _this;
    this->s = ((struct Object*)classOf(_this))->s;
    return _this + sizeof(struct Object);
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

size_t classSz(const void *this)
{
    const struct MetaClass *class = this;
    return class->size;
}

void *offsetOf(const void *this, const void *_class)
{
    const struct MetaClass *class = _class;
    return (void*)this + classSz(class->super);
}
/*
 *	MetaClass
 */

static void *MetaClass_ctor(void *_this, va_list *app)
{
    struct MetaClass *this = _this;
    const size_t offset = offsetof(struct MetaClass, ctor);

    this->name = va_arg(*app, char*);
    this->super = va_arg(*app, Form_t).class;
    assert(this->super);
    this->size = va_arg(*app, size_t);

    assert(this->super);
    memcpy((char*)this + offset, (char*)this->super + offset, sizeOf(this->super) - offset);
    *(void const **)&this->_.s = this->super->_.s;
    voidf selector;
    va_list ap;
    va_copy(ap, *app);

    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf) MetaClassS.ctor)
            *(voidf *) &this->ctor = method;
        else if (selector == (voidf) MetaClassS.dtor)
            *(voidf *) &this->dtor = method;
        else if (selector == (voidf) MetaClassS.differ)
            *(voidf *) &this->differ = method;
        else if (selector == (voidf) MetaClassS.puto)
            *(voidf *) &this->puto = method;
        else if (selector == Selector)
            *(void **) &this->_.s = method;
    }
    va_end(ap);
    return _this + sizeof(struct MetaClass);
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

void *_new(FormWO_t t, ...)
{
    if (t._.f == POD) {
        if (t.mem)
            return t.mem;
        else
            return malloc(t._.size);
    }
    const struct MetaClass *class = t._.class;
    struct Object *object;
    va_list ap;
    assert(class && class->size);
    if (t.mem)
        object = t.mem;
    else
        object = calloc(1, class->size);
    assert(object);
    object->class = class;
    va_start(ap, t);
    class->ctor(object, &ap);
    va_end(ap);
    return object;
}

void _delete(Form_t t, void *_this)
{
    if (t.f == POD || !_this) {
        free(_this);
        return;
    }
    const struct MetaClass *class = classOf(_this);
    free(class->dtor(_this));
}

void construct(Form_t t, void *mem, const void *x)
{
    new(compose(t, mem), x);
}

void destroy(void *_obj)
{
    struct Object *obj = _obj;
    obj->class->dtor(_obj);
}

static void *ctor(void *mem, ...)
{
    void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
    assert(class->ctor);
    va_list ap;
    va_start(ap, mem);
    struct Object *object = mem ? mem : calloc(1, class->size);
    assert(object);
    object->class = class;
    class->ctor(object, &ap);
    va_end(ap);
    return object;
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

//this指针
static thread_key_t this_key;
static thread_once_t this_key_once = THREAD_ONCE_INIT;

struct this_node {
    const void *this;
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
    node->this = p;
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
    const void *p = node->this;
    ptr->head = node->next;
    deallocate(node, sizeof(struct this_node));
    return (void*)p;
}
