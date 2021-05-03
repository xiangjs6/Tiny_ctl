#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_slist.h"
#include "../include/tctl_int.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"

#define Import SLIST, ITERATOR, INT

int cmp(const void *_a, const void *_b)
{
    Int a = (void*)_a;
    Int b = (void*)_b;
    return (int)(a->val - b->val);
}

int main(void)
{
    ARP_CreatePool();
    Slist sl = new(T(Slist), T(Int), VAEND);
    for (int i = 0; i < 20; i++) {
        int temp = (int)rand() % 20;
        printf("%d ", temp);
        THIS(sl).push_front(VA(temp));
    }
    putchar('\n');
    THIS(sl).reverse();
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(THIS(sl).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    THIS(sl).pop_front();
    Iterator in_it = THIS(sl).begin();
    THIS(in_it).inc();
    THIS(in_it).inc();
    int temp = 100;
    THIS(sl).insert_after(in_it, VA(temp));
    THIS(in_it).inc();
    THIS(in_it).inc();
    THIS(sl).erase_after(in_it);
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(THIS(sl).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    printf("size:%u\n", THIS(sl).size());
    printf("front:%lld\n", ((Int)THIS(sl).front())->val);
    Slist sl2 = new(T(Slist), T(Int), VA(2, 100), VAEND);
    for (int i = 0; i < 100; i++) {
        int temp = (int)rand() % 20;
        THIS(sl2).push_front(VA(temp));
    }
    THIS(sl).swap(sl2);

    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(THIS(sl).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    for (Iterator it = THIS(sl2).begin(); !THIS(it).equal(THIS(sl2).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    printf("splice\n");
    Iterator last = THIS(sl2).begin();
    advance(last, 10);
    THIS(sl).splice_after(THIS(sl).begin(), sl2, THIS(sl2).begin(), last, VAEND);
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(THIS(sl).end()); THIS(it).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');
    for (Iterator it = THIS(sl2).begin(); !THIS(it).equal(THIS(sl2).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    putchar('\n');

    printf("%c\n", "YN"[!THIS(sl).empty()]);
    THIS(sl).clear();
    printf("%c\n", "YN"[!THIS(sl).empty()]);

    Slist sl3 = new(T(Slist), T(Int), VAEND);
    for (int i = 0; i < 1000; i++) {
        int temp = (int)rand() % 1000;
        THIS(sl3).push_front(VA(temp));
    }
    THIS(sl3).sort(cmp);

    for (Iterator it = THIS(sl3).begin(); !THIS(it).equal(THIS(sl3).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    fflush(stdout);

    for (Iterator it = THIS(sl3).begin(); !THIS(it).equal(THIS(sl3).end()); THIS(it).inc()) {
        static long long pre = 0;
        if (pre > ((Int)THIS(it).derefer())->val) {
            printf("bad\n");
            return -1;
        }
        pre = ((Int)THIS(it).derefer())->val;
    }


    printf("unique\n");
    THIS(sl2).sort(cmp);
    THIS(sl2).unique();
    for (Iterator it = THIS(sl2).begin(); !THIS(it).equal(THIS(sl2).end()); THIS(it).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');

    delete(sl);
    delete(sl2);
    delete(sl3);
    ARP_FreePool();
}
