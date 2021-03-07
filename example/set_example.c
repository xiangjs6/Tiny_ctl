#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_int.h"
#include "../include/tctl_set.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_iterator.h"

#define Import SET, VECTOR, ITERATOR

int cmp(FormWO_t _a, FormWO_t _b)
{
    long long a = toInt(_a);
    long long b = toInt(_b);
    return a - b;
}

int main(void)
{
    ARP_CreatePool();
    int i;
    int ia[5] = {0, 1, 2, 3, 4};
    Set s = new(T(Set), VA(T(int), VA_FUNC(cmp)));
    for (i = 0; i < 5; i++)
        THIS(s).insert(VA(ia[i]));
    printf("size:%ld\n", THIS(s).size());
    printf("3 count=%ld\n", THIS(s).count(VA(ia[4])));
    i = 3;
    THIS(s).insert(VA(i));
    printf("size:%ld\n", THIS(s).size());
    printf("3 count=%ld\n", THIS(s).count(VA(ia[4])));
    i = 5;
    THIS(s).insert(VA(i));
    printf("size:%ld\n", THIS(s).size());
    printf("3 count=%ld\n", THIS(s).count(VA(ia[4])));
    i = 1;
    THIS(s).erase(THIS(s).find(VA(i)));
    THIS(s).erase(THIS(s).end());
    THIS(s).erase(THIS(s).begin());
    for (Iterator it = THIS(s).begin(); !THIS(it).equal(VA(THIS(s).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    printf("size:%ld\n", THIS(s).size());
    printf("3 count=%ld\n", THIS(s).count(VA(ia[4])));
    printf("1 count=%ld\n", THIS(s).count(VA(ia[1])));
    Iterator ite1 = THIS(s).begin();
    Iterator ite2 = THIS(s).end();
    for (; !THIS(ite1).equal(VA(ite2)); THIS(ite1).inc())
        printf("%d ", *(int*)THIS(ite1).derefer());
    putchar('\n');
    FormWO_t o = VA(ia[3]);
    THIS(ite1).assign(VA(THIS(s).find(o)));
    if (!THIS(ite1).equal(VA(ite2)))
        printf("3 found\n");
    o = VA(ia[1]);
    THIS(ite1).assign(VA(THIS(s).find(o)));
    if (THIS(ite1).equal(VA(ite2)))
        printf("1 not found\n");
    delete(s);

    Vector v = new(T(Vector), VA(T(int)));
    for (int i = 0; i < 10; i++) {
        int temp = random() % 10;
        printf("%d ", temp);
        THIS(v).push_back(VA(temp));
    }
    putchar('\n');
    Set s1 = new(T(Set), VA(T(int), VA_FUNC(cmp), THIS(v).begin(), THIS(v).end()));
    for (Iterator it = THIS(s1).begin(); !THIS(it).equal(VA(THIS(s1).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');

    delete(s1);
    delete(v);
    ARP_FreePool();
    return 0;
}
