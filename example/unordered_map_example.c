#include "../include/tctl_unordered_map.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_any.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include <stdio.h>
#include <string.h>
#define Import UNORDERED_MAP, PAIR, ANY, INT
int scmp(void *_x, void *_y)
{
    Any x = _x, y = _y;
    char *a = THIS(x).value(), *b = THIS(y).value();
    int res = strcmp(a, b);
    return res;
}

void lookup(Unordered_Map s, Any any)
{
    char *w = THIS(any).value();
    Iterator it = THIS(s).find(any);
    if (THIS(it).equal(THIS(s).end()))
        puts("not present");
    else {
        Pair p = THIS(it).derefer();
        printf("%s:%s:%lld\n", w, (char*)THIS((Any) p->first).value(), ((Int)p->second)->val);
    }
}

int main(void)
{
	ARP_CreatePool();
	Unordered_Map m = new(T(Unordered_Map), T(Any), T(Int), VA(scmp, FUNC),
                          VA(hash_str, FUNC), VAEND);

    Pair p = tmpPair(T(Any), T(Int), VA("january", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("february", ANYONE), VA(28), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("march", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("april", ANYONE), VA(30), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("may", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("june", ANYONE), VA(30), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("july", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("august", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("september", ANYONE), VA(30), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("october", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("november", ANYONE), VA(30), VAEND);
	THIS(m).insert(p);

    p = tmpPair(T(Any), T(Int), VA("december", ANYONE), VA(31), VAEND);
	THIS(m).insert(p);

    Any any = VA("september", ANYONE);
    lookup(m, any);
    any = VA("june", ANYONE);
    lookup(m, any);
    any = VA("february", ANYONE);
    lookup(m, any);
    any = VA("december", ANYONE);
    lookup(m, any);

	printf("--------------------\n");
	for (Iterator it = THIS(m).begin(); !THIS(it).equal(THIS(m).end()); THIS(it).inc()) {
	    Pair pp = THIS(it).derefer();
	    Any f = pp->first;
        printf("%s:%lld\n", (char*)THIS(f).value(), ((Int)pp->second)->val);
    }
	delete(m);
	ARP_FreePool();
	return 0;
}
