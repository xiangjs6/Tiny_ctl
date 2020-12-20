#include "tctl_unordered_map.h"
#include "tctl_iterator.h"
#include "tctl_hash_fun.h"
#include "tctl_object.h"
#include "auto_release_pool.h"
#include <stdio.h>
int scmp(const void *x, const void *y)
{
    const char **a = (void*)x, **b = (void*)y;
    int res = strcmp(*a, *b);
    return res;
}
void lookup(unordered_map *m, char *w)
{
    iterator(pair(char*, int)) it = THIS(m).find(&w);
    printf("%s:%d\n", w, it ? it->val->second : -1);
}
int main(void)
{
	ARP_CreatePool();
	unordered_map m = creat_unordered_map(sizeof(char*), sizeof(int), scmp, hash_str);
	pair(char*, int) p;
	p.first = "january";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "february";
	p.second = 28;
	THIS(&m).insert(&p);
	p.first = "march";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "april";
	p.second = 30;
	THIS(&m).insert(&p);
	p.first = "may";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "june";
	p.second = 30;
	THIS(&m).insert(&p);
	p.first = "july";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "august";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "september";
	p.second = 30;
	THIS(&m).insert(&p);
	p.first = "october";
	p.second = 31;
	THIS(&m).insert(&p);
	p.first = "november";
	p.second = 30;
	THIS(&m).insert(&p);
	p.first = "december";
	p.second = 31;
	THIS(&m).insert(&p);
	lookup(&m, "september");
	lookup(&m, "june");
	lookup(&m, "february");
	lookup(&m, "december");
	printf("--------------------\n");
	for (iterator(pair(char*, int)) it = THIS(&m).begin(); !ITER(it).equal(THIS(&m).end()); ITER(it).inc())
		printf("%s:%d\n", it->val->first, it->val->second);
	destory_unordered_map(&m);
	ARP_FreePool();
	return 0;
}
