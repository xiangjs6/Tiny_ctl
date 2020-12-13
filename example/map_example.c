#include <stdio.h>
#include "tctl_iterator.h"
#include "tctl_map.h"
#include "tctl_allocator.c"
#include <string.h>

byte scmp(const char**a, const char**b)
{
    byte res = strcmp(*a, *b);
    return res;
}
//map测试
int main(void)
{
    ARP_CreatePool();
    map simap = creat_map(sizeof(char*), sizeof(long long), scmp);
    pair(char*, long long) p;
    p.first = allocate(10);
    strcpy(p.first, "jjhou");
    p.second = 1;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jerry");
    p.second = 2;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jason");
    p.second = 3;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jimmy");
    p.second = 4;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "david");
    p.second = 5;
    THIS(&simap).insert(&p);
    for (iterator(pair(char*, long long)) it = THIS(&simap).begin(); !ITER(it).equal(THIS(&simap).end()); ITER(it).increment())
        printf("key:%s val:%lld\n", it->val->first, it->val->second);
    char key[10] = {"jjhou"};
    char *p_key = key;
    iterator(pair(char*, long long)) f_it = THIS(&simap).find(&p_key);
    printf("%lld\n", f_it->val->second);
    iterator(pair(char*, long long)) ite1 = INIT_ITERATOR;
    strcpy(key, "mchen");
    ITER(ite1).copy(THIS(&simap).find(&p_key));
    if (ITER(ite1).equal(THIS(&simap).end()))
        printf("mchen not found\n");
    strcpy(key, "jerry");
    ITER(ite1).copy(THIS(&simap).find(&p_key));
    if (!ITER(ite1).equal(THIS(&simap).end()))
        printf("jerry found\n");
    ite1->val->second = 9;
    iterator(pair(char*, long long)) ite2 = THIS(&simap).find(&p_key);
    printf("%lld f_it:%lld\n", ite2->val->second, f_it->val->second);
    ARP_FreePool();
    return 0;
}
