//
// Created by xjs on 2021/3/7.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_list.h"
#include "../include/tctl_int.h"
#include "../include/tctl_double.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"

#define Import LIST, INT, DOUBLE, ITERATOR

int cmp(const void *_a, const void *_b)
{
    Int a = (void*)_a;
    Int b = (void*)_b;
    return (int)(a->val - b->val);
}

int main(void)
{
    ARP_CreatePool();
    List l = new(T(List), T(Int), VAEND);
    int temp = 100;
    for (int i = 0; i < 10; i++) {
        THIS(l).push_back(VA(i));
    }
    for (int i = 0; i < 5; i++) {
        THIS(l).pop_back();
    }
    int temp1 = 20;
    THIS(l).pop_front();
    THIS(l).push_front(VA(temp1));
    Iterator iter = THIS(l).begin();
    THIS(l).insert(iter, VA(temp));
    THIS(iter).inc();
    THIS(iter).inc();
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
        //THIS(&l).insert(*it, &temp);
    }
    fflush(stdout);
    printf("it:%lld\n", ((Int)THIS(iter).derefer())->val);
    THIS(l).erase(iter);
    printf("first\n");
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
        //THIS(&l).insert(*it, &temp);
    }
    THIS(l).remove(VA(temp1));
    temp1 = 4;
    THIS(l).push_back(VA(temp1));
    printf("second\n");
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    THIS(l).unique();
    printf("third\n");
    Iterator r_it = THIS(l).end();
    THIS(r_it).dec();
    printf("%lld\n", ((Int)THIS(r_it).derefer())->val);
    for (; !THIS(r_it).equal(VA(THIS(l).end())); THIS(r_it).dec()) {
        printf("%lld\n", ((Int)THIS(r_it).derefer())->val);
    }
    List l2 = new(T(List), T(Int), VAEND);
    //init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(l2).push_back(VA(i));
    printf("splice\n");
    THIS(l).splice(THIS(l).begin(), l2, THIS(l2).begin(), THIS(l2).end(), VAEND);
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    printf("reverse\n");
    THIS(l).reverse();
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    printf("front%lld\n", ((Int)THIS(l).front())->val);
    printf("back%lld\n", ((Int)THIS(l).back())->val);
    printf("size:%u\n", THIS(l).size());
    printf("merge\n");
    List l1 = new(T(List), T(Int), VAEND);
    List l3 = new(T(List), T(Int), VAEND);
    for (int i = 0; i < 10; i++) {
        THIS(l1).push_back(VA(i));
        THIS(l3).push_back(VA(i));
    }
    for (Iterator it = THIS(l1).begin(); !THIS(it).equal(VA(THIS(l1).end())); THIS(it).inc())
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    THIS(l1).merge(l3, cmp);
    printf("merge\n");
    for (Iterator it = THIS(l1).begin(); !THIS(it).equal(VA(THIS(l1).end())); THIS(it).inc())
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    printf("sort\n");
    THIS(l).clear();
    for (int i = 0; i < 10000; i++) {
        temp = (int)rand();
        THIS(l).push_back(VA(temp));
    }
    THIS(l).sort(cmp);
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    for (int i = 1; i < 10000; i++) {
        if (((Int)THIS(l).brackets(VA(i)))->val < ((Int)THIS(l).brackets(VA(i - 1)))->val) {
            printf("bad\n");
            return 0;
        }
    }
    long long dis = distance(THIS(l).begin(), THIS(l).end());
    printf("dist:%lld\n", dis);
    List l4 = new(T(List), T(Double), l1, VAEND);
    for (int i = 0; i < THIS(l4).size(); i++) {
        printf("%lf\n", ((Double)THIS(l4).brackets(VA(i)))->val);
    }
    delete(l);
    delete(l1);
    delete(l2);
    delete(l3);
    printf("%d\n", ARP_GetPoolsCount());
    printf("%d\n", ARP_GetPoolNodesCount());


    List l5 = new(T(List), T(Int), VA(3), VA(4), VAEND);
    List l6 = new(T(List), T(Int), THIS(l5).begin(), THIS(l5).end(), VAEND);
    for (Iterator it = THIS(l6).begin(); !THIS(it).equal(VA(THIS(l6).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    delete(l5);
    delete(l6);
    ARP_FreePool();
    return 0;
}
