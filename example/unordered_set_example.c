#include <stdio.h>
#include <string.h>
#include "../include/tctl_unordered_set.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_iterator.h"
#include "../include/tctl_common.h"
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
    Unordered_Set s = new(T(Unordered_Set), T(Any), VA_ANY(TEMP_VAR(void*, scmp), NULL),
                          VA_ANY(TEMP_VAR(void*, hash_str), NULL), VAEND);
    char *str = "kiwi";
    Any any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "plum";
    any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "apple";
    any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "mango";
    any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "apricot";
    any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "banana";
    any = VA_ANY(str, NULL);
    THIS(s).insert(any);
    str = "mango";
    any = VA_ANY(str, NULL);
    lookup(s, any);
    str = "apple";
    any = VA_ANY(str, NULL);
    lookup(s, any);
    str = "durian";
    any = VA_ANY(str, NULL);
    lookup(s, any);
    for (Iterator it = THIS(s).begin(); !THIS(it).equal(THIS(s).end()); THIS(it).inc())
        printf("%s ", *(char**)THIS((Any)THIS(it).derefer()).value());
    putchar('\n');
    ARP_FreePool();
}
