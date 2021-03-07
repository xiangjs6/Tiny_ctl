#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_slist.h"
#include "../include/auto_release_pool.h"

#define Import SLIST

int main(void)
{
    ARP_CreatePool();
    Slist sl = new(T(Slist), VA(T(int)));
    for (int i = 0; i < 20; i++) {
        int temp = random() % 20;
        printf("%d ", temp);
        THIS(sl).push_front(VA(temp));
    }
    putchar('\n');
    THIS(sl).reverse();
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
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
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    printf("size:%ld\n", THIS(sl).size());
    printf("front:%d\n", *(int*)THIS(sl).front());
    Slist sl2 = new(T(Slist), VA(T(int), 2, 100));
    for (int i = 0; i < 100; i++) {
        int temp = random() % 20;
        THIS(sl2).push_front(VA(temp));
    }
    THIS(sl).swap(sl2);

    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    for (Iterator it = THIS(sl2).begin(); !THIS(it).equal(VA(THIS(sl2).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    printf("%c\n", "YN"[!THIS(sl).empty()]);
    THIS(sl).clear();
    printf("%c\n", "YN"[!THIS(sl).empty()]);
    delete(sl);
    delete(sl2);
    ARP_FreePool();
}
