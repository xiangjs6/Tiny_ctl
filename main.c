#include <stdio.h>
#include "allocator.h"
#include "tctl_object.h"
#include "tctl_common.h"
#include "iterator.h"
#include "tctl_vector.h"
#include <stdlib.h>

void f(int *a)
{
    printf("%d\n", *a);
}

struct foo{
    int a;
    int b;
    iterator start;
    iterator finish;
    __iterator_obj_func iter_func;
    iter_ptr (*begin)(void);
    iter_ptr (*end)(void);
};

void *incress(void *p)
{
    struct foo *this = *pthis();
    return &this->b;
}

iter_ptr begin(void)
{
    struct foo *this = *pthis();
    this->start = init_iter(this, &this->a, &this->iter_func);
    return (void**)&this->start;
}

iter_ptr end(void)
{
    struct foo *this = *pthis();
    this->finish = init_iter(this, &this->b, &this->iter_func);
    return (void**)&this->finish;
}

int main(void)
{
    vector v;
    init_vector(&v, 0, sizeof(int), NULL);
    //*(int*)THIS(&v).at(0) = 1;
    int temp = 2;
    THIS(&v).push_back(&temp);
    temp = 3;
    THIS(&v).push_back(&temp);
    temp = 4;
    THIS(&v).push_back(&temp);
    temp = 5;
    THIS(&v).push_back(&temp);
    temp = 6;
    THIS(&v).push_back(&temp);
    THIS(&v).pop_back();
    temp = 100;
    THIS(&v).insert(v.begin(), &temp);
    temp = 101;
    THIS(&v).insert(v.end(), &temp);
    int *arr = *v.end();
    temp = 102;
    ITER_TYPE(int) ii = NEW_ITER(THIS(&v).begin());
    *ii = ITER(ii).add(2);
    THIS(&v).insert(ii, &temp);
    arr = *v.end();
    *ii = ITER(ii).add(3);
    THIS(&v).erase(ii);
    arr = *v.end();
    THIS(&v).resize(100);
    iter_ptr haha = *v.begin();
    haha += 5 * 4;
    temp = 1000;
    iter_ptr tt = v.insert(haha, &temp);
    printf("aaaa%d\n", *(int*)tt);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    *ii = *THIS(&v).begin();
    printf("%d %d\n", **(int**)v.begin(), **(int**)v.end());
    v.clear();
    printf("%d\n", v.capacity());
    printf("%d\n", v.size());
    destory_vector(&v);
    return 0;
}
