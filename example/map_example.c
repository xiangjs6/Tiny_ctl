#include "../include/tctl_iterator.h"
#include "../include/tctl_map.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include <string.h>
#include <stdio.h>

#define Import MAP, ANY, INT

int scmp(const char**a, const char**b)
{
    int res = strcmp(*a, *b);
    return res;
}
int pcmp(void *_a, void *_b)
{
    Pair a = _a;
    Pair b = _b;
    Any a_a = a->first;
    Any a_b = b->first;
    return strcmp(THIS(a_a).value(), THIS(a_b).value());
}
//map测试
int main(void)
{
    ARP_CreatePool();
    Map simap = new(T(Map), T(Any), T(Int), VA(pcmp, FUNC), VAEND);
    Pair p = tmpPair(T(Any), T(Int), VA("jjhou", ANYONE), VA(1), VAEND);
    THIS(simap).insert(p);

    p = tmpPair(T(Any), T(Int), VA("jerry", ANYONE), VA(2), VAEND);
    //strcpy(p->first, "jerry");
    //*(int*)p->second = 2;
    THIS(simap).insert(p);

    p = tmpPair(T(Any), T(Int), VA("jason", ANYONE), VA(3), VAEND);
    //strcpy(p->first, "jason");
    //*(int*)p->second = 3;
    THIS(simap).insert(p);

    p = tmpPair(T(Any), T(Int), VA("jimmy", ANYONE), VA(4), VAEND);
    //strcpy(p->first, "jimmy");
    //*(int*)p->second = 4;
    THIS(simap).insert(p);

    p = tmpPair(T(Any), T(Int), VA("david", ANYONE), VA(5), VAEND);
    //strcpy(p->first, "david");
    //*(int*)p->second = 5;
    THIS(simap).insert(p);
    for (Iterator it = THIS(simap).begin(); !THIS(it).equal(THIS(simap).end()); THIS(it).inc()) {
        Pair pp = THIS(it).derefer();
        Any f = pp->first;
        Int s = pp->second;
        printf("key:%s val:%lld\n", (char*)THIS(f).value(), s->val);
    }
    Any key = VA("jjhou", ANYONE);
    Iterator f_it = THIS(simap).find(key);
    //delete(p);
    p = THIS(f_it).derefer();
    printf("%lld\n", ((Int)p->second)->val);
    fflush(stdout);
    Iterator ite1;
    key = VA("mchen", ANYONE);
    //strcpy(key, "mchen");
    ite1 = THIS(simap).find(key);
    if (THIS(ite1).equal(THIS(simap).end()))
        printf("mchen not found\n");
    key = VA("jerry", ANYONE);
    //strcpy(key, "jerry");
    ite1 = THIS(simap).find(key);
    if (!THIS(ite1).equal(THIS(simap).end()))
        printf("jerry found\n");
    p = THIS(ite1).derefer();
    ((Int)p->second)->val = 9;
    Iterator ite2 = THIS(simap).find(key);
    Pair pp = THIS(ite2).derefer();
    printf("%s %lld f_it:%lld count:%u\n", (char*)(THIS((Any)p->first).value()), ((Int)p->second)->val, ((Int)pp->second)->val, THIS(simap).count(key));
    ARP_FreePool();
    return 0;
}
