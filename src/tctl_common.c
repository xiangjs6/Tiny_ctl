//
// Created by xjs on 2020/9/1.
//

#include "../include/tctl_common.h"
#include "../include/tctl_allocator.h"
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/tctl_char.h"
#include "../include/tctl_any.h"

#define Import INT, CHAR, DOUBLE

size_t hash_numeric(const void *x)
{
    MetaObject obj = (MetaObject)x;
    Int i = classOf(obj) == T(Int) ? obj : THIS(obj).cast(T(Int));
    return i->val;
}

size_t hash_str(const void *x)
{
    char *str = THIS((Any)x).value();
    size_t res = 0;
    for (int i = 0; str[i]; i++)
        res = 5 * res + str[i];
    return res;
}
