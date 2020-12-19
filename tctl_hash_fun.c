//
// Created by xjs on 2020/12/19.
//

#include "tctl_hash_fun.h"
size_t hash_char(const void *x)
{
    char _x = *(char*)x;
    return _x;
}
size_t hash_short(const void *x)
{
    short _x = *(short*)x;
    return _x;
}
size_t hash_int(const void *x)
{
    int _x = *(int*)x;
    return _x;
}
size_t hash_long(const void *x)
{
    long _x = *(long*)x;
    return _x;
}
size_t hash_llong(const void *x)
{
    long long _x = *(long long*)x;
    return _x;
}
size_t hash_uchar(const void *x)
{
    unsigned char _x = *(unsigned char*)x;
    return _x;
}
size_t hash_ushort(const void *x)
{
    unsigned short _x = *(unsigned short*)x;
    return _x;
}
size_t hash_uint(const void *x)
{
    unsigned int _x = *(unsigned int*)x;
    return _x;
}
size_t hash_ulong(const void *x)
{
    unsigned long _x = *(unsigned long*)x;
    return _x;
}
size_t hash_ullong(const void *x)
{
    unsigned long long _x = *(unsigned long long*)x;
    return _x;
}
size_t hash_str(const void *x)
{
    char *str = *(char**)x;
    size_t res = 0;
    for (int i = 0; str[i]; i++)
        res = 5 * res + str[i];
    return res;
}
