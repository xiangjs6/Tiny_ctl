#include <stdio.h>
#include <string.h>
#include "../include/tctl_unordered_set.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_iterator.h"
//#include "tctl_hash_fun.h"
#define Import UNORDERED_SET
int scmp(FormWO_t _x, FormWO_t _y)
{
    const char *a = *(char**)_x.mem, *b = *(char**)_y.mem;
    int res = strcmp(a, b);
    return res;
}

size_t hash_str(FormWO_t x)
{
    char *str = *(char**)x.mem;
    size_t res = 0;
    for (int i = 0; str[i]; i++)
        res = 5 * res + str[i];
    return res;
}

void lookup(Unordered_Set s, char *w)
{
    Iterator it = THIS(s).find(VA(w));
    if (THIS(it).equal(VA(THIS(s).end())))
        puts("not present");
    else
        printf("%s:%s\n", w, *(char**)THIS(it).derefer());
}
//unordered_set测试
int main(void)
{
    ARP_CreatePool();
    Unordered_Set s = new(T(Unordered_Set), VA(T(char*), VA_FUNC(scmp), VA_FUNC(hash_str)));
    char *str = "kiwi";
    THIS(s).insert(VA(str));
    str = "plum";
    THIS(s).insert(VA(str));
    str = "apple";
    THIS(s).insert(VA(str));
    str = "mango";
    THIS(s).insert(VA(str));
    str = "apricot";
    THIS(s).insert(VA(str));
    str = "banana";
    THIS(s).insert(VA(str));
    lookup(s, "mango");
    lookup(s, "apple");
    lookup(s, "durian");
    for (Iterator it = THIS(s).begin(); !THIS(it).equal(VA(THIS(s).end())); THIS(it).inc())
        printf("%s ", *(char**)THIS(it).derefer());
    putchar('\n');
    ARP_FreePool();
}
