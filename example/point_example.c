#include <stdio.h>
#include "../include/tctl_pointer_iterator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"

#define Import ITERATOR, INT
int main(void)
{

    ARP_CreatePool();
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Iterator start = oriPointIter(array);
    for (; !THIS(start).equal(VA(oriPointIter(array, sizeof(array) / sizeof(int)))); THIS(start).inc())
        printf("%d ", *(int*)THIS(start).derefer());
    putchar('\n');
    char mem[classSz(T(Int).class) * 10];
    for (int i = 0; i < 10; i++) {
        construct(T(Int), mem + i * classSz(T(Int).class), VA(i), VAEND);
    }
    Int obj = (void*)mem;
    start = oriPointIter(obj);
    for (; !THIS(start).equal(VA(oriPointIter(obj, 10))); THIS(start).inc())
        printf("%lld ", ((Int)THIS(start).derefer())->val);
    putchar('\n');

    Iterator r_it = THIS(start).reverse_iterator();
    do
    {
        THIS(r_it).inc();
        printf("%lld ", ((Int)THIS(r_it).derefer())->val);
    } while(!THIS(r_it).equal(VA(oriPointIter(obj))));
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        Int I = (void*)(mem + i * classSz(T(Int).class));
        destroy(I);
    }
    ARP_FreePool();
    return 0;
}
