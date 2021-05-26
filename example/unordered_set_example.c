#include <stdio.h>
#include <string.h>
#include "../include/tctl_unordered_set.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#define Import UNORDERED_SET, ANY
int scmp(void *_x, void *_y)
{
    Any x = _x;
    Any y = _y;
    char *a = *(void**)THIS(x).value();
    char *b = *(void**)THIS(y).value();
    int res = strcmp(a, b);
    return res;
}

void lookup(Unordered_Set s, Any any)
{
    char *w = *(void**)THIS(any).value();
    Iterator it = THIS(s).find(any);
    if (THIS(it).equal(THIS(s).end()))
        puts("not present");
    else
        printf("%s:%s\n", w, *(char**)THIS((Any)THIS(it).derefer()).value());
}
//unordered_set测试
int main(void)
{
    ARP_CreatePool();
    Unordered_Set s = new(T(Unordered_Set), T(Any), VA(scmp, FUNC),
                          VA(hash_str, FUNC), VAEND);
    char *str = "kiwi";
    Any any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "plum";
    any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "apple";
    any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "mango";
    any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "apricot";
    any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "banana";
    any = VA(str, ANYONE);
    THIS(s).insert(any);
    str = "mango";
    any = VA(str, ANYONE);
    lookup(s, any);
    str = "apple";
    any = VA(str, ANYONE);
    lookup(s, any);
    str = "durian";
    any = VA(str, ANYONE);
    lookup(s, any);
    for (Iterator it = THIS(s).begin(); !THIS(it).equal(THIS(s).end()); THIS(it).inc())
        printf("%s ", *(char**)THIS((Any)THIS(it).derefer()).value());
    putchar('\n');
    ARP_FreePool();
}
