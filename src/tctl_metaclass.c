//
// Created by xjs on 2020/8/31.
//

#include "../include/tctl_allocator.h"
#include "../include/tctl_portable.h"
#include "include/_tctl_metaclass.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


struct MetaObject {
    const void *s;
    const struct MetaClass *class;	/* object's description */
};

struct MetaClass {
    const struct MetaObject _;			/* class' description */
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
static void *metaobject_ctor(void *_this, va_list *app);
static void *metaobject_dtor(void *_this);
static int metaobject_differ(const void *_this, const void *b);
static int metaobject_puto(const void *_this, FILE *fp);
static void *metaclass_ctor(void *_this, va_list *app);
static void *metaclass_dtor(void *_this);

static struct MetaClassSelector MetaClassS = {ctor, dtor, differ, puto};
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

Form_t _MetaObject(void)
{
    Form_t t = {OBJ , {.class = __MetaObject}};
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
static void *metaobject_ctor(void *_this, va_list *app)
{
    struct MetaObject *this = _this;
    this->s = ((struct MetaObject*)classOf(_this))->s;
    return _this;
}

static void *metaobject_dtor(void *_this)
{
    return _this;
}

static int metaobject_differ(const void *_this, const void *b)
{
    return _this != b;
}

static int metaobject_puto(const void *_this, FILE *fp)
{
    const struct MetaClass *class = classOf(_this);
    return fprintf(fp, "%s at %p\n", class -> name, _this);
}

const void *classOf(const void *_this)
{
    const struct MetaObject *this = _this;
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

static void *metaclass_ctor(void *_this, va_list *app)
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
        if (selector == (voidf)MetaClassS.ctor)
            *(void**)&this->ctor = method;
        else if (selector == (voidf)MetaClassS.dtor)
            *(void**)&this->dtor = method;
        else if (selector == (voidf)MetaClassS.differ)
            *(void**)&this->differ = method;
        else if (selector == (voidf)MetaClassS.puto)
            *(void**)&this->puto = method;
        else if (selector == Selector)
            *(void**)&this->_.s = method;
    }
    va_end(ap);
    return _this;
}

static void *metaclass_dtor(void *_this)
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
    assert(t._.f != ADDR);
    if (t._.f == POD) {
        void *m;
        if (t.mem)
            m = t.mem;
        else
            m = malloc(t._.size);
        va_list ap;
        va_start(ap, t);
        FormWO_t o = va_arg(ap, FormWO_t); 
        assert(o._.f != OBJ);
        if (o._.f == END)
            memset(m, 0, t._.size);
        else if (o._.f == ADDR)
            memcpy(m, o.mem, t._.size);
        else if (o._.f == POD)
            memcpy(m, &o.mem, t._.size);
        va_end(ap);
        return m;
    }
    const struct MetaClass *class = t._.class;
    struct MetaObject *object;
    va_list ap;
    assert(class && class->size);
    if (t.mem)
        object = t.mem;
    else
        object = calloc(1, class->size);
    assert(object);
    object->class = class;
    va_start(ap, t);
    object = class->ctor(object, &ap);
    va_end(ap);
    return object;
}

void _delete(FormWO_t t)
{
    if (t._.f == POD || !t.mem) {
        free(t.mem);
        return;
    }
    const struct MetaClass *class = classOf(t.mem);
    free(class->dtor(t.mem));
}

void *construct_v(Form_t t, void *mem, va_list *app)
{
    assert(mem);
    const struct MetaClass *class = t.class;
    struct MetaObject *object = mem;
    assert(class && class->size);
    assert(object);
    object->class = class;
    object = class->ctor(object, app);
    return object;
}

void *construct(Form_t t, void *mem, ...)
{
    va_list ap;
    va_start(ap, mem);
    struct MetaObject *object = construct_v(t, mem, &ap);
    va_end(ap);
    return object;
}

void destroy(void *_obj)
{
    struct MetaObject *obj = _obj;
    obj->class->dtor(_obj);
}

static void *ctor(void *mem, ...)
{
    void *_this = pop_this();
    const struct MetaClass *class = classOf(_this);
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

static int puto(FILE *fp)
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

union _basic_val _valueAux(int t, ...)
{
    union _basic_val ret;
    va_list ap;
    va_start(ap, t);
    switch (t)
    {
        case 'f':
            ret.f = (float)va_arg(ap, double);
            break;
        case 'F':
            ret.lf = (double)va_arg(ap, double);
            break;
        case 'c':
            ret.c = (unsigned char)va_arg(ap, int);
            break;
        case 's':
            ret.s = (unsigned short)va_arg(ap, int);
            break;
        case 'i':
            ret.i = (unsigned int)va_arg(ap, int);
            break;
        case 'l':
            ret.l = (unsigned long)va_arg(ap, long);
            break;
        case 'L':
            ret.ll = (unsigned long long)va_arg(ap, long long);
            break;
        case 'p':
            ret.p = (void*)va_arg(ap, void*);
            break;
        case 't':
            ret.p = va_arg(ap, FormWO_t).mem;
            break;
        default:
            assert(0);
    }
    return ret;
}

Form_t _formAux(int t, ...)
{
    va_list ap;
    va_start(ap, t);
    Form_t f;
    if (t == 0) {
        f = va_arg(ap, Form_t);
        f.f += FORM;
    } else
        f = va_arg(ap, FormWO_t)._;
    va_end(ap);
    return f;
}
