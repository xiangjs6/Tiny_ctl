#include <stdio.h>
#include <string.h>
#include "../include/tctl_vector.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_algobase.h"
#include "../include/tctl_int.h"
#include "../include/auto_release_pool.h"

#define Import VECTOR, ITERATOR, INT

int less(FormWO_t a, FormWO_t b)
{
    return toInt(a) > toInt(b);
}

int strCmp(FormWO_t a, FormWO_t b)
{
    char *s1 = *(char**)a.mem;
    char *s2 = *(char**)b.mem;
    return strcmp(s1, s2);
}
int main(void)
{
    ARP_CreatePool();
    //init
    int ia[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    Vector iv1 = new(T(Vector), VA(T(Int), oriPointIter(ia), oriPointIter(ia, 5)));
    Vector iv2 = new(T(Vector), VA(T(Int), oriPointIter(ia), oriPointIter(ia, 9)));
    for (Iterator i = THIS(iv2).begin(); !THIS(i).equal(VA(THIS(iv2).end())); THIS(i).inc()) //0 1 2 3 4 5 6 7 8
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //mismatch
    Iterator it;
    it = mismatch(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND)->first;
    if (THIS(it).equal(VA(THIS(iv1).end()))) //iv1 end
        printf("iv1 end\n");
    it = mismatch(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND)->second;
    printf("%lld\n", ((Int)THIS(it).derefer())->val); //5

    //equal
    //true false true
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), THIS(iv2).begin(), VAEND) ? "true" : "false");
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), oriPointIter(ia, 3), VAEND) ? "true" : "false");
    printf("%s\n", equal(THIS(iv1).begin(), THIS(iv1).end(), oriPointIter(ia, 3), VA_FUNC(less)) ? "true" : "false");

    //fill
    fill(THIS(iv1).begin(), THIS(iv1).end(), VA(9), VAEND);
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //9 9 9 9 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //fill_n
    fill_n(THIS(iv1).begin(), 3, VA(7), VAEND);
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //7 7 7 9 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //iter_swap
    Iterator ite1 = THIS(iv1).begin();
    Iterator ite2 = THIS(ite1).add(VA(3));
    iter_swap(ite1, ite2, VAEND);
    printf("ite1:%lld ite2:%lld\n", ((Int)THIS(ite1).derefer())->val, ((Int)THIS(ite2).derefer())->val); //ite1:9 ite2:7
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //9 7 7 7 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');

    //max min
    Form_t t = THIS(ite1).type();
    printf("max:%lld\n", toInt(max(FORM_WITH_OBJ(t, THIS(ite1).derefer()), //max:9
                             FORM_WITH_OBJ(t, THIS(ite2).derefer()), VAEND)));
    printf("min:%lld\n", toInt(min(FORM_WITH_OBJ(t, THIS(ite1).derefer()), //min:7
                             FORM_WITH_OBJ(t, THIS(ite2).derefer()), VAEND)));

    //swap
    ite2 = THIS(iv2).begin();
    swap(FORM_WITH_OBJ(t, THIS(ite1).derefer()), FORM_WITH_OBJ(t, THIS(ite2).derefer()), VAEND);
    printf("ite1:%lld ite2:%lld\n", ((Int)THIS(ite1).derefer())->val, ((Int)THIS(ite2).derefer())->val); //ite1:0 ite2:9
    for (Iterator i = THIS(iv1).begin(); !THIS(i).equal(VA(THIS(iv1).end())); THIS(i).inc()) //0 7 7 7 9
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');
    for (Iterator i = THIS(iv2).begin(); !THIS(i).equal(VA(THIS(iv2).end())); THIS(i).inc()) //9 1 2 3 4 5 6 7 8
        printf("%lld ", ((Int)THIS(i).derefer())->val);
    putchar('\n');
    
    //lexicographical_compare
    char *stra1[] = {"Jamie", "JJHou", "Jason"};
    char *stra2[] = {"Jamie", "JJHou", "Jerry"};
    int res  = lexicographical_compare(oriPointIter(stra1), oriPointIter(stra1, 3),
                                       oriPointIter(stra2), oriPointIter(stra2, 3), VA_FUNC(strCmp));
    printf("%d\n", res); //1
    ARP_FreePool();
    return 0;
}
