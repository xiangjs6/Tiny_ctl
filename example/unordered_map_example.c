#include "../include/tctl_unordered_map.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_common.h"
#include <stdio.h>
#include <string.h>
#define Import UNORDERED_MAP, PAIR
int scmp(FormWO_t _x, FormWO_t _y)
{
    char *a = *(char**)_x.mem, *b = *(char**)_y.mem;
    int res = strcmp(a, b);
    return res;
}

void lookup(Unordered_Map m, char *w)
{
    Iterator it = THIS(m).find(VA(w));
    if (THIS(it).equal(VA(THIS(m).end())))
        printf("%s:%d\n", w, -1);
    else {
        Pair p = THIS(it).derefer();
        printf("%s:%d\n", w, *(int*)p->second);
    }
}
int main(void)
{
	ARP_CreatePool();
	Unordered_Map m = new(T(Unordered_Map), VA(T(char*), T(int), VA_FUNC(scmp), VA_FUNC(hash_str)));
	Pair p = new(T(Pair), VA(T(char*), T(int)));
	*(char**)p->first = "january";
	*(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "february";
	*(int*)p->second = 28;
	THIS(m).insert(p);
    *(char**)p->first = "march";
	*(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "april";
	*(int*)p->second = 30;
	THIS(m).insert(p);
    *(char**)p->first = "may";
	*(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "june";
	*(int*)p->second = 30;
	THIS(m).insert(p);
    *(char**)p->first = "july";
    *(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "august";
	*(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "september";
	*(int*)p->second = 30;
	THIS(m).insert(p);
    *(char**)p->first = "october";
	*(int*)p->second = 31;
	THIS(m).insert(p);
    *(char**)p->first = "november";
	*(int*)p->second = 30;
	THIS(m).insert(p);
    *(char**)p->first = "december";
	*(int*)p->second = 31;
	THIS(m).insert(p);
	lookup(m, "september");
	lookup(m, "june");
	lookup(m, "february");
	lookup(m, "december");
	printf("--------------------\n");
	for (Iterator it = THIS(m).begin(); !THIS(it).equal(VA(THIS(m).end())); THIS(it).inc()) {
	    Pair pp = THIS(it).derefer();
        printf("%s:%d\n", *(char**)pp->first, *(int*)pp->second);
    }
	delete(m);
    delete(p);
	ARP_FreePool();
	return 0;
}
