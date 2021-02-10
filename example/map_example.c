#include <stdio.h>
#include "../include/tctl_iterator.h"
#include "../include/tctl_map.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include <string.h>

#define Import MAP

int scmp(const char**a, const char**b)
{
    int res = strcmp(*a, *b);
    return res;
}
int pcmp(FormWO_t _a, FormWO_t _b)
{
    Pair a = _a.mem;
    Pair b = _b.mem;
    return strcmp(a->first, b->first);
}
//map测试
int main(void)
{
    ARP_CreatePool();
    Map simap = new(T(Map), VA(ARRAY_T(char, 10), T(int), VA_FUNC(pcmp)));
    Pair p = tmpPair(ARRAY_T(char, 10), T(int), VA(VA_ADDR("jjhou"), 1), VAEND);//new(T(Pair), VA(ARRAY_T(char, 10), T(int), VA_ADDR("jjhou"), 1));
    THIS(simap).insert(p);

    strcpy(p->first, "jerry");
    *(int*)p->second = 2;
    THIS(simap).insert(p);

    strcpy(p->first, "jason");
    *(int*)p->second = 3;
    THIS(simap).insert(p);

    strcpy(p->first, "jimmy");
    *(int*)p->second = 4;
    THIS(simap).insert(p);

    strcpy(p->first, "david");
    *(int*)p->second = 5;
    THIS(simap).insert(p);
    for (Iterator it = THIS(simap).begin(); !THIS(it).equal(VA(THIS(simap).end())); THIS(it).inc()) {
        Pair pp = THIS(it).derefer();
        printf("key:%s val:%d\n", (char*)pp->first, *(int*)pp->second);
    }
    char key[10] = {"jjhou"};
    Iterator f_it = THIS(simap).find(VA(VA_ADDR(key)));
    //delete(p);
    p = THIS(f_it).derefer();
    printf("%d\n", *(int*)p->second);
    Iterator ite1;
    strcpy(key, "mchen");
    ite1 = THIS(simap).find(VA(VA_ADDR(key)));
    if (THIS(ite1).equal(VA(THIS(simap).end())))
        printf("mchen not found\n");
    strcpy(key, "jerry");
    ite1 = THIS(simap).find(VA(VA_ADDR(key)));
    if (!THIS(ite1).equal(VA(THIS(simap).end())))
        printf("jerry found\n");
    p = THIS(ite1).derefer();
    *(int*)p->second = 9;
    Iterator ite2 = THIS(simap).find(VA(VA_ADDR(key)));
    Pair pp = THIS(ite2).derefer();
    printf("%s %d f_it:%d count:%ld\n", (char*)p->first, *(int*)p->second, *(int*)pp->second, THIS(simap).count(VA(VA_ADDR(key))));
    ARP_FreePool();
    return 0;
}
