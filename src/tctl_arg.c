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

    void *ret;
    double d;
    char c;
    long long i;
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
            ret = ARP_MallocARel(classSz(T(Any)));
            construct(T(Any), ret, &d, sizeof(d), &to_Double, VAEND);
            //ret = to_Double(&d, T(Double));
            break;
        case 'c':
            c = va_arg(ap, int);
            ret = ARP_MallocARel(classSz(T(Any)));
            construct(T(Any), ret, &c, sizeof(c), &to_Char, VAEND);
            //ret = to_Char(&c, T(Char));
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
            ret = ARP_MallocARel(classSz(T(Any)));
            construct(T(Any), ret, &i, sizeof(i), &to_Int, VAEND);
            //ret = to_Int(&i, T(Int));
            break;
        case 'S':
            ret = ARP_MallocARel(classSz(T(Any)));
            p_st = va_arg(ap, void*);
            st_size = va_arg(ap, size_t);
            p_cast = va_arg(ap, void*);
            construct(T(Any), ret, p_st, st_size, p_cast, VAEND);
            break;
        case 'O':
            ret = va_arg(ap, void*);
            break;
        case 'o':
        default:
            assert(0);
    }
    va_end(ap);
    return ret;
}
