#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_rb_tree.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_iterator.h"
#include "../include/tctl_int.h"

#define Import RB_TREE, ITERATOR

int cmp(FormWO_t _a, FormWO_t _b)
{
    long long a = toInt(_a);
    long long b = toInt(_b);
    return a - b;
}

int main(void)
{
    ARP_CreatePool();
    RB_tree tree = new(T(RB_tree), VA(T(long int), VA_FUNC(cmp)));
    int n = 100;
    THIS(tree).insert_equal(VA(n));
    n = 95;
    THIS(tree).insert_equal(VA(n));
    n = 96;
    //THIS(&tree).insert_equal(&n);
    THIS(tree).insert_equal(VA(n));
    n = 97;
    THIS(tree).insert_equal(VA(n));
    n = 105;
    THIS(tree).insert_equal(VA(n));
    n = 96;
    THIS(tree).insert_equal(VA(n));
    n = 101;
    THIS(tree).insert_equal(VA(n));
    n = 99;
    THIS(tree).insert_unique(VA(n));
    THIS(tree).insert_unique(VA(n));
    n = 98;
    THIS(tree).insert_unique(VA(n));
    //n = 200;
    //THIS(&tree).insert_equal(&n);
    //n = 1;
    //THIS(&tree).insert_equal(&n);
    Iterator it = THIS(tree).begin();
    for (; !THIS(it).equal(VA(THIS(tree).end())); THIS(it).inc()) {
        printf("%d ", *(int*)THIS(it).derefer());
    }
    putchar('\n');
    for (THIS(it).dec(); !THIS(it).equal(VA(THIS(tree).end())); THIS(it).dec()) {
        printf("%d ", *(int*)THIS(it).derefer());
    }
    putchar('\n');
    n = 96;
    printf("%ld\n", THIS(tree).count(VA(n)));
    n = 98;
    Iterator f_it = THIS(tree).find(VA(n));
    THIS(tree).erase(f_it);
    n = 96;
    FormWO_t t = VA(n);
    THIS(f_it).assign(VA(THIS(tree).find(t)));
    THIS(tree).erase(f_it);
    n = 95;
    t = VA(n);
    THIS(f_it).assign(VA(THIS(tree).find(t)));
    THIS(tree).erase(f_it);
    n = 97;
    t = VA(n);
    THIS(f_it).assign(VA(THIS(tree).find(t)));
    THIS(tree).erase(f_it);
    n = 101;
    t = VA(n);
    THIS(f_it).assign(VA(THIS(tree).find(t)));
    THIS(tree).erase(f_it);
    n = 99;
    t = VA(n);
    THIS(f_it).assign(VA(THIS(tree).find(t)));
    printf("find:%ld\n", *(long int*)THIS(f_it).derefer());
    for (THIS(it).assign(VA(THIS(tree).begin())); !THIS(it).equal(VA(THIS(tree).end())); THIS(it).inc()) {
        printf("%ld ", *(long int*)THIS(it).derefer());
    }
    putchar('\n');

    for (int i = 0; i < 3000; i++) {
        int temp = random();
        THIS(tree).insert_equal(VA(temp));
    }
    it = THIS(tree).begin();
    Iterator pre = THIS(tree).begin();
    printf("%ld ", *(long int*)THIS(it).derefer());
    for (THIS(it).inc(); !THIS(it).equal(VA(THIS(tree).end())); THIS(it).inc(), THIS(pre).inc()) {
        printf("%ld ", *(long int*)THIS(it).derefer());
        if (*(long int*)THIS(it).derefer() < *(long int*)THIS(pre).derefer()) {
            puts("fuck bad!!!");
            return 0;
        }
    }
    putchar('\n');

    RB_tree t2 = new(T(RB_tree), VA(T(long int), tree));
    n = 95;
    THIS(t2).insert_equal(VA(n));
    n = 96;
    THIS(t2).insert_equal(VA(n));
    n = 97;
    THIS(t2).insert_equal(VA(n));
    it = THIS(t2).begin();
    pre = THIS(t2).begin();
    printf("%ld ", *(long int*)THIS(it).derefer());
    for (THIS(it).inc(); !THIS(it).equal(VA(THIS(t2).end())); THIS(it).inc(), THIS(pre).inc()) {
        printf("%ld ", *(long int*)THIS(it).derefer());
        if (*(long int*)THIS(it).derefer() < *(long int*)THIS(pre).derefer()) {
            puts("fuck bad!!!");
            return 0;
        }
    }
    putchar('\n');

    delete(tree);
    ARP_FreePool();
    return 0;
}
