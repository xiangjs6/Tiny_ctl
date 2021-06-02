//
// Created by xjs on 2021/4/23.
//

#include <assert.h>
#include <stdarg.h>
#include "../include/tctl_arg.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/tctl_char.h"

#define Import ANY, INT, DOUBLE, CHAR

void *VAEND = &VAEND;

void *_valueAux(int t, ...)
{

    void *ret;
    double d;
    char c;
    long long i;
    void *p_val;
    size_t val_size;
    void *p_cast;

    va_list ap;
    va_start(ap, t);
    switch (t)
    {
        case 'f':
        case 'F':
            d = va_arg(ap, double);
            ret = ARP_MallocARelDtor(classSz(T(Any)), delete);
            ret = construct(T(Any), ret, &d, POD, sizeof(d), &to_Double, VAEND);
            break;
        case 'c':
            c = va_arg(ap, int);
            ret = ARP_MallocARelDtor(classSz(T(Any)), delete);
            ret = construct(T(Any), ret, &c, POD, sizeof(c), &to_Char, VAEND);
            break;
        case 's':
        case 'i':
        case 'l':
        case 'L':
            switch (t)
            {
                case 's':
                case 'i':
                    i = va_arg(ap, int);
                    break;
                case 'l':
                    i = va_arg(ap, long);
                    break;
                case 'L':
                    i = va_arg(ap, long long);
                    break;
                default:
                    assert(0);
            }
            ret = ARP_MallocARelDtor(classSz(T(Any)), delete);
            ret = construct(T(Any), ret, &i, POD, sizeof(i), &to_Int, VAEND);
            break;
        case 'A':
            p_val = va_arg(ap, void*);
            val_size = va_arg(ap, size_t);
            p_cast = va_arg(ap, void*);
            ret = ARP_MallocARelDtor(classSz(T(Any)), delete);
            ret = construct(T(Any), ret, p_val, POD, val_size, p_cast, VAEND);
            break;
        case 'M':
            ret = ARP_MallocARelDtor(classSz(T(Any)), delete);
            ret = construct(T(Any), ret, va_arg(ap, void*), FUNC, VAEND);
            break;
        default:
            assert(0);
    }
    va_end(ap);
    return ret;
}
