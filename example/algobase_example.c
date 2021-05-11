#include <stdio.h>
#include <string.h>
#include "../include/tctl_vector.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_algobase.h"
#include "../include/tctl_int.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"

#define Import VECTOR, ITERATOR, INT, ANY

int less(void *a, void *b)
{
    Int x = classOf(a) == T(Int) ? a : THIS((MetaObject)a).cast(T(Int));
    Int y = classOf(b) == T(Int) ? b : THIS((MetaObject)b).cast(T(Int));
    return x->val > y->val;
}

int strCmp(void *a, void *b)
{
    Any x = a;
    Any y = b;
    char *s1 = *(char**)THIS(x).value();
    char *s2 = *(char**)THIS(y).value();
    return strcmp(s1, s2);
}
int main(void)
{
    ARP_CreatePool();
    //init
    Int ia[9];
    for (int i = 0; i < 9; i++)
        ia[i] = new(T(Int), VA(i), VAEND);
    Vector iv1 = new(T(Vector), T(Int), oriPointerIter(ia), oriPointerIter(ia, 5), VAEND);
    Vector iv2 = new(T(Vector), T(Int), oriPointerIter(ia), oriPointerIter(ia, 9), VAEND);
    for (Iterator i = THIS(iv2).begin(); !THIS(i).equal(THIS(iv2).end()); THIS(i).inc()) //0 1 2 3 4 5 6 7 8
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //mismatch
    Iterator it;
    it = mismatch(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND)->first;
    if (THIS(it).equal(THIS(iv1).end())) //iv1 end
        printf("iv1 end\n");
    it = mismatch(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND)->second;
    printf("%lld\n", ((Int)THIS(it).derefer())->val); //5

    //equal
    //true false true
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND) ? "true" : "false");
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), oriPointerIter(ia, 3), VAEND) ? "true" : "false");
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), oriPointerIter(ia, 3), less) ? "true" : "false");

    //fill
    fill(THIS(iv1).begin(), THIS(iv1).end(), VA(9));
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //9 9 9 9 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //fill_n
    fill_n(THIS(iv1).begin(), VA(3), VA(7));
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //7 7 7 9 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //iter_swap
    Iterator ite1 = THIS(iv1).begin();
    Iterator ite2 = THIS(ite1).add(VA(3));
    iter_swap(ite1, ite2);
    printf("ite1:%lld ite2:%lld\n", ((Int)THIS(ite1).derefer())->val, ((Int)THIS(ite2).derefer())->val); //ite1:9 ite2:7
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //9 7 7 7 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //max min
    printf("max:%lld\n", ((Int)(max(THIS(ite1).derefer(), //max:9
                             THIS(ite2).derefer(), VAEND)))->val);
    printf("max:%lld\n", ((Int)(min(THIS(ite1).derefer(), //max:7
                                    THIS(ite2).derefer(), VAEND)))->val);

    //swap
    ite2 = THIS(iv2).begin();
    swap(THIS(ite1).derefer(), THIS(ite2).derefer());
    printf("ite1:%lld ite2:%lld\n", ((Int)THIS(ite1).derefer())->val, ((Int)THIS(ite2).derefer())->val); //ite1:0 ite2:9
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //0 7 7 7 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');
    for (Iterator i = THIS(iv2).begin(); !THIS(i).equal(VA(THIS(iv2).end())); THIS(i).inc()) //9 1 2 3 4 5 6 7 8
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //lexicographical_compare
    char *strs[4] = {"Jamie", "JJHou", "Jason", "Jerry"};
    Any anys[4];
    for (int i = 0; i < 4; i++)
        anys[i] = VA_ANY(strs[i], NULL);
    Any stra1[] = {anys[0], anys[1], anys[2]};
    Any stra2[] = {anys[0], anys[1], anys[3]};
    int res  = lexicographical_compare(oriPointerIter(stra1), oriPointerIter(stra1, 3),
                                       oriPointerIter(stra2), oriPointerIter(stra2, 3), strCmp);
    printf("%d\n", res); //1
    for (int i = 0; i < 9; i++)
        delete(ia[i]);
    delete(iv1);
    delete(iv2);
    ARP_FreePool();
    return 0;
}
