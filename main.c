#include <stdio.h>
#include "tctl_allocator.h"
#include "tctl_object.h"
#include "tctl_common.h"
#include "tctl_iterator.h"
#include "tctl_vector.h"
#include <stdlib.h>


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
    THIS(&v).insert(*THIS(&v).begin(), &temp);
    temp = 101;
    THIS(&v).insert(*THIS(&v).end(), &temp);
    int *arr = *THIS(&v).end();
    temp = 102;
    ITER_TYPE(int) ii = NEW_ITER(THIS(&v).begin());
    //*ii = ITER(ii).add(2);
    THIS(&v).insert(*ii, &temp);
    arr = *THIS(&v).end();
    //*ii = ITER(ii).add(3);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    *ii = THIS(&v).erase(*ii);
    printf("%ddddd\n", **ii);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    arr = *THIS(&v).end();
    THIS(&v).resize(100);
    iter_ptr haha = *THIS(&v).begin();
    haha += 5 * 4;
    temp = 1000;
    iter_ptr tt = THIS(&v).insert(haha, &temp);
    printf("aaaa%d\n", *(int*)tt);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    *ii = *THIS(&v).begin();
    printf("%d %d\n", **(int**)THIS(&v).begin(), **(int**)THIS(&v).end());
    THIS(&v).clear();
    printf("%d\n", THIS(&v).capacity());
    printf("%d\n", THIS(&v).size());
    destory_vector(&v);
    return 0;
}
