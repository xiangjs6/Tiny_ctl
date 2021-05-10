#include <stdio.h>
#include "../include/tctl_pointer_iterator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"

#define Import ITERATOR, INT
int main(void)
{

    ARP_CreatePool();
    Int array[10];
    for (int i = 0; i < 10; i++)
        array[i] = new(T(Int), VA(i), VAEND);
    Iterator start = oriPointIter(array);
    for (; !THIS(start).equal(VA(oriPointIter(array, sizeof(array) / sizeof(int)))); THIS(start).inc())
        printf("%d ", *(int*)THIS(start).derefer());
    putchar('\n');
    char mem[classSz(T(Int)) * 10];
    for (int i = 0; i < 10; i++) {
        construct(T(Int), mem + i * classSz(T(Int)), VA(i), VAEND);
    }
    Int obj = (void*)mem;
    start = oriPointerIter(obj);
    for (; !THIS(start).equal(VA(oriPointerIter(obj, 10))); THIS(start).inc())
        printf("%lld ", ((Int)THIS(start).derefer())->val);
    putchar('\n');

    Iterator r_it = THIS(start).reverse_iterator();
    do
    {
        THIS(r_it).inc();
        printf("%lld ", ((Int)THIS(r_it).derefer())->val);
    } while(!THIS(r_it).equal(VA(oriPointerIter(obj))));
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        Int I = (void*)(mem + i * classSz(T(Int).class));
        destroy(I);
    }
    ARP_FreePool();
    return 0;
}
