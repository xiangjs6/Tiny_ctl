#include <stdio.h>
#include "tctl_unordered_set.h"
#include "auto_release_pool.h"
#include "tctl_iterator.h"
#include "tctl_hash_fun.h"
int scmp(const void *x, const void *y)
{
    const char **a = (void*)x, **b = (void*)y;
    int res = strcmp(*a, *b);
    return res;
}

void lookup(unordered_set *s, char *w)
{
    iterator(char*) it = THIS(s).find(&w);
    printf("%s:%s\n", w, it ? *it->val : "not present");
}
//unordered_set测试
int main(void)
{
    ARP_CreatePool();
    unordered_set s = creat_unordered_set(sizeof(char*), scmp, hash_str);
    char *str = "kiwi";
    THIS(&s).insert(&str);
    str = "plum";
    THIS(&s).insert(&str);
    str = "apple";
    THIS(&s).insert(&str);
    str = "mango";
    THIS(&s).insert(&str);
    str = "apricot";
    THIS(&s).insert(&str);
    str = "banana";
    THIS(&s).insert(&str);
    lookup(&s, "mango");
    lookup(&s, "apple");
    lookup(&s, "durian");
    for (iterator(char*) it = THIS(&s).begin(); !ITER(it).equal(THIS(&s).end()); ITER(it).inc())
    printf("%s ", *it->val);
    putchar('\n');
    ARP_FreePool();
}
