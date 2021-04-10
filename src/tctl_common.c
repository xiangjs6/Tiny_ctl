//
// Created by xjs on 2020/9/1.
//

#include "../include/tctl_common.h"
#include "../include/tctl_allocator.h"
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/tctl_char.h"
#include "../include/tctl_uint.h"

#define Import INT, UINT, CHAR, DOUBLE

size_t hash_numeric(FormWO_t x)
{
    if (x._.f == POD) {
        switch (x._.size)
        {
            case sizeof(char):
                return *(char*)x.mem;
            case sizeof(int):
                return *(int*)x.mem;
            case sizeof(short):
                return *(short*)x.mem;
            case sizeof(long long):
                return *(long long*)x.mem;
        }
    } else if (x._.f == OBJ) {
        if (x._.class == T(Int).class) {
            Int v = *(Int*)x.mem;
            return v->val;
        } else if (x._.class == T(UInt).class) {
            UInt v = *(UInt*)x.mem;
            return v->val;
        } else if (x._.class == T(Char).class) {
            Char v = *(Char*)x.mem;
            return v->val;
        } else if (x._.class == T(Double).class) {
            Double v = *(Double*)x.mem;
            return (size_t)v->val;
        }
    } else {
        switch (x._.size)
        {
            case sizeof(char):
                return **(char**)x.mem;
            case sizeof(int):
                return **(int**)x.mem;
            case sizeof(short):
                return **(short**)x.mem;
            case sizeof(long long):
                return **(long long**)x.mem;
        }
    }
    assert(0);
}

size_t hash_str(FormWO_t x)
{
    char *str;
    if (x._.f == POD)
        str = **(char***)x.mem; //存放了指向了char指针的指针
    else
        str = *(char**)x.mem;
    size_t res = 0;
    for (int i = 0; str[i]; i++)
        res = 5 * res + str[i];
    return res;
}
