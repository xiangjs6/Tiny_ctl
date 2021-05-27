#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_rb_tree.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"

#define Import RB_TREE, ITERATOR, INT

int cmp(void *_a, void *_b)
{
    Int a = THIS((MetaObject)_a).cast(T(Int));
    Int b = THIS((MetaObject)_b).cast(T(Int));
    return (int)(a->val - b->val);
}

int main(void)
{
    ARP_CreatePool();
    RB_tree tree = new(T(RB_tree), T(Int), VA(cmp, FUNC), VAEND);
    long int n = 100;
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
    for (; !THIS(it).equal(THIS(tree).end()); THIS(it).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');
    for (THIS(it).dec(); !THIS(it).equal(THIS(tree).end()); THIS(it).dec()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');
    fflush(stdout);
    n = 96;
    printf("%d\n", THIS(tree).count(VA(n)));
    n = 98;
    Iterator f_it = THIS(tree).find(VA(n));
    printf("find:%lld\n", ((Int)THIS(f_it).derefer())->val);
    THIS(tree).erase(f_it);
    n = 96;
    Any t = VA(n);
    THIS(f_it).assign(THIS(tree).find(t));
    THIS(tree).erase(f_it);
    n = 95;
    t = VA(n);
    THIS(f_it).assign(THIS(tree).find(t));
    THIS(tree).erase(f_it);
    n = 97;
    t = VA(n);
    THIS(f_it).assign(THIS(tree).find(t));
    THIS(tree).erase(f_it);
    n = 101;
    t = VA(n);
    THIS(f_it).assign(THIS(tree).find(t));
    THIS(tree).erase(f_it);
    n = 99;
    t = VA(n);
    THIS(f_it).assign(THIS(tree).find(t));
    printf("find:%lld\n", ((Int)THIS(f_it).derefer())->val);
    for (THIS(it).assign(THIS(tree).begin()); !THIS(it).equal(THIS(tree).end()); THIS(it).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');

    for (int i = 0; i < 3000; i++) {
        long int temp = (long int)rand();
        THIS(tree).insert_equal(VA(temp));
    }
    it = THIS(tree).begin();
    Iterator pre = THIS(tree).begin();
    printf("%lld ", ((Int)THIS(it).derefer())->val);
    for (THIS(it).inc(); !THIS(it).equal(THIS(tree).end()); THIS(it).inc(), THIS(pre).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
        if (((Int)THIS(it).derefer())->val < ((Int)THIS(pre).derefer())->val) {
            puts("fuck bad!!!");
            return 0;
        }
    }
    putchar('\n');

    RB_tree t2 = new(T(RB_tree), T(Int), tree, VAEND);
    n = 95;
    THIS(t2).insert_equal(VA(n));
    n = 96;
    THIS(t2).insert_equal(VA(n));
    n = 97;
    THIS(t2).insert_equal(VA(n));
    it = THIS(t2).begin();
    pre = THIS(t2).begin();
    printf("%lld ", ((Int)THIS(it).derefer())->val);
    for (THIS(it).inc(); !THIS(it).equal(THIS(t2).end()); THIS(it).inc(), THIS(pre).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
        if (((Int)THIS(it).derefer())->val < ((Int)THIS(pre).derefer())->val) {
            puts("fuck bad!!!");
            return 0;
        }
    }
    putchar('\n');

    delete(tree);
    ARP_FreePool();
    return 0;
}
