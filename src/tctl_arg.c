//
// Created by xjs on 2021/4/23.
//

#include "../include/tctl_arg.h"
#include <assert.h>
#include <stdarg.h>
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/tctl_char.h"

#define Import ANY, INT, DOUBLE, CHAR

void *VAEND = &VAEND;

static void *to_Int(void *p, const void *class)
{
    void *mem = ARP_MallocARel(classSz(T(Int)));
    Int i = construct(T(Int), mem, VAEND);
    i->val = *(unsigned long long*)p;
    if (class != T(Int))
        return THIS(i).cast(class);
    return i;
}

static void *to_Char(void *p, const void *class)
{
    void *mem = ARP_MallocARel(classSz(T(Char)));
    Char c = construct(T(Char), mem, VAEND);
    c->val = *(char*)p;
    if (class != T(Char))
        return THIS(c).cast(class);
    return c;
}

static void *to_Double(void *p, const void *class)
{
    void *mem = ARP_MallocARel(classSz(T(Double)));
    Double d = construct(T(Double), mem, VAEND);
    d->val = *(double*)p;
    if (class != T(Double))
        return THIS(d).cast(class);
    return d;
}

void *_valueAux(int t, ...)
{

    void *ret;
    double d;
    unsigned char c;
    unsigned long long i;
    void *p_st;
    size_t st_size;
    void *p_cast;

    va_list ap;
    va_start(ap, t);
    switch (t)
    {
        case 'f':
        case 'F':
            d = va_arg(ap, double);
            ret = to_Double(&d, T(Double));
            break;
        case 'c':
            c = va_arg(ap, unsigned int);
            ret = to_Char(&c, T(Char));
            break;
        case 's':
        case 'i':
        case 'l':
        case 'L':
            switch (t)
            {
                case 's':
                case 'i':
                    i = va_arg(ap, unsigned int);
                    break;
                case 'l':
                    i = va_arg(ap, unsigned long);
                    break;
                case 'L':
                    i = va_arg(ap, unsigned long long);
                    break;
            }
            ret = to_Int(&i, T(Int));
            break;
        case 'S':
            ret = ARP_MallocARel(classSz(T(Any)));
            p_st = va_arg(ap, void*);
            st_size = va_arg(ap, size_t);
            p_cast = va_arg(ap, void*);
            construct(T(Any), ret, POD, p_st, st_size, p_cast, VAEND);
            break;
        case 'O':
            ret = va_arg(ap, void*);
        case 'o':
        default:
            assert(0);
    }
    va_end(ap);
    return ret;
}
