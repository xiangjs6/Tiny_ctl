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

void *_valueAux(int t, ...)
{
    static size_t any_size = 0;
    if (!any_size)
        any_size = classSz(T(Any));

    void *ret;
    char any[any_size];
    float f;
    double d;
    unsigned char c;
    unsigned long long i;
    va_list ap;
    va_start(ap, t);
    switch (t)
    {
        case 'f':
            f = va_arg(ap, double);
            construct(T(Any), any, POD, &f, sizeof(f), VAEND);
            ret = ARP_MallocARel(classSz(T(Double)));
            construct(T(Double), ret, any, VAEND);
            destroy(any);
            break;
        case 'F':
            d = va_arg(ap, double);
            construct(T(Any), any, POD, &d, sizeof(d), VAEND);
            ret = ARP_MallocARel(classSz(T(Double)));
            construct(T(Double), ret, any, VAEND);
            destroy(any);
            break;
        case 'c':
            c = va_arg(ap, unsigned int);
            construct(T(Any), any, POD, &c, sizeof(c), VAEND);
            ret = ARP_MallocARel(classSz(T(Char)));
            construct(T(Double), ret, any, VAEND);
            destroy(any);
            break;
        case 's':
        case 'i':
            i = va_arg(ap, unsigned int);
            construct(T(Any), any, POD, &i, sizeof(i), VAEND);
            ret = ARP_MallocARel(classSz(T(Int)));
            construct(T(Int), ret, any, VAEND);
            destroy(any);
            break;
        case 'l':
        case 'L':
            i = va_arg(ap, unsigned long);
            construct(T(Any), any, POD, &i, sizeof(i), VAEND);
            ret = ARP_MallocARel(classSz(T(Int)));
            construct(T(Int), ret, any, VAEND);
            destroy(any);
            break;
        case 'S':
            ret = ARP_MallocARel(any_size);
            construct(T(Any), ret, POD, va_arg(ap, void*), va_arg(ap, size_t), VAEND);
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
