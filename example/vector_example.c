#include <stdio.h>
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_char.h"
#include "../include/auto_release_pool.h"

#define Import VECTOR, INT, DOUBLE, CHAR, ITERATOR

int main(void)
{
    ARP_CreatePool();
    Vector v = new(T(Vector), VA(T(Double), 10, 3.20));
    printf("nmemb:%ld\n", THIS(v).size());
    Int i_t = new(T(Int));
    for (int i = 0; i < 10; i++) {
        THIS(v).push_back(VA(i + 1.0));
    }
    for (int i = 0; i < 5; i++)
        THIS(v).pop_back();
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(VA(THIS(v).end())); THIS(it).inc()) {
        printf("%lf ", ((Double)THIS(it).derefer())->val);
    }
    putchar('\n');
    putchar('\n');
    printf("insert\n");
    Iterator in_it = THIS(v).begin();
    Int temp = new(T(Int), VA(1000));
    THIS(v).insert(in_it, VA(temp));
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    THIS(in_it).inc();
    THIS(in_it).inc();
    temp->val = 101;
    THIS(v).insert(in_it, VA(temp));
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    THIS(in_it).self_add(VA(3));
    temp->val = 102;
    THIS(v).insert(in_it, VA(temp));
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    THIS(in_it).self_sub(VA(2));
    temp->val = 103;
    THIS(v).insert(in_it, VA(temp));
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    THIS(in_it).dec();
    THIS(in_it).dec();
    temp->val = 104;
    THIS(v).insert(in_it, VA(temp));
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    THIS(in_it).inc();
    THIS(v).erase(in_it);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(VA(THIS(v).end())); THIS(it).inc()) {
        printf("%lf ", ((Double)THIS(it).derefer())->val);
        Iterator itt = THIS(v).begin();
        int t = THIS(it).dist(itt);
        t = distance(itt, it);
        t = 0;
    }
    putchar('\n');
    putchar('\n');
    printf("front:%lf\n", ((Double)THIS(v).front())->val);
    printf("back:%lf\n", ((Double)THIS(v).back())->val);
    printf("empty:%c\n", *("NY" + THIS(v).empty()));
    //THIS(v).clear();
    printf("empty:%c\n", *("NY" + THIS(v).empty()));
    printf("capacoty:%lu\n", THIS(v).capacity());
    THIS(v).resize(9);
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lf ", ((Double)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    putchar('\n');
    Vector v1 = new(T(Vector), VA(T(Int), THIS(v).begin(), THIS(v).end()));
    for (Iterator it = THIS(v1).begin(); !THIS(it).equal(VA(THIS(v1).end())); THIS(it).inc()) {
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    }
    putchar('\n');
    putchar('\n');
    Vector v2 = new(T(Vector), VA(T(Char), v));
    for (Iterator it = THIS(v2).begin(); !THIS(it).equal(VA(THIS(v2).end())); THIS(it).inc()) {
        printf("%d ", ((Char)THIS(it).derefer())->val);
    }
    putchar('\n');
    putchar('\n');
    delete(v1);
    delete(v);
    Vector v3 = new(T(Vector), VA(T(int), 3, 4));
    Vector v4 = new(T(Vector), VA(T(int), v3));
    for (Iterator it = THIS(v4).begin(); !THIS(it).equal(VA(THIS(v4).end())); THIS(it).inc()) {
        printf("%d ", *(int*)THIS(it).derefer());
    }
    ARP_FreePool();
    return 0;
}