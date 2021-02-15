#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
/*
#include "include/tctl_allocator.h"
#include "include/tctl_object.h"
#include "include/tctl_common.h"
#include "include/tctl_iterator.h"
#include "include/tctl_unordered_set.h"
#include "include/tctl_hash_fun.h"
#include "include/tctl_unordered_multimap.h"*/
#include "../include/tctl_hashtable.h"
#include "../include/tctl_map.h"
#include "../include/tctl_set.h"
#include "../include/tctl_rb_tree.h"
#include "../include/tctl_slist.h"
#include "../include/tctl_priority_queue.h"
#include "../include/tctl_heap.h"
#include "../include/tctl_queue.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_uint.h"
#include "../include/tctl_char.h"
#include "../include/tctl_double.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_list.h"
#include "../include/tctl_deque.h"
#include "../include/tctl_stack.h"
#include "../include/tctl_metaclass.h"
#include "../include/tctl_utility.h"

#define Import INT, UINT, CHAR, DOUBLE, CLASS, VECTOR, ITERATOR, LIST, DEQUE, QUEUE, STACK, PRIORITY_QUEUE, SLIST, RB_TREE, SET, MAP, \
               PAIR, HASHTABLE
int cmp(const int *a, const int *b)
{
    if (*a > *b)
        return 1;
    else if (*a < *b)
        return -1;
    return 0;
}

int scmp(const void *x, const void *y)
{
    const char **a = (void*)x, **b = (void*)y;
    int res = strcmp(*a, *b);
    return res;
}

struct foo {
    int a;
    int b;
    double c;
    char d;
};

int ocmp(FormWO_t _a, FormWO_t _b)
{
    long int Pa = _a.mem;
    long int Pb = _b.mem;
    long int *Aa = _a.mem;
    long int *Ab = _b.mem;
    Int Oa = _a.mem;
    Int Ob = _b.mem;
    if (_a._.f == POD)
        return Pa - Pb;
    else if (_a._.f == ADDR)
        return *Aa - *Ab;
    else
        return Oa->val - Ob->val;
}

int pcmp(FormWO_t _a, FormWO_t _b)
{
    Pair a = _a.mem;
    Pair b = _b.mem;
    return strcmp(a->first, b->first);
}
//class测试
/*int main(void)
{
    ARP_CreatePool();
    Deque v = new(T(Deque), VA(T(Int)));
    Int i;
    for (i = new(T(Int), VA(20)); i->val > 0; THIS(i).dec()) {
        Int a = i;
        THIS(v).push_back(VA(a));
    }
    for (i = new(T(Int), VA(20)); i->val > 0; THIS(i).dec()) {
        Int a = i;
        THIS(v).push_front(VA(a));
    }
    for (i = new(T(Int), VA(5)); i->val > 0; THIS(i).dec()) {
        THIS(v).pop_back();
    }
    for (i = new(T(Int), VA(5)); i->val > 0; THIS(i).dec()) {
        THIS(v).pop_front();
    }
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(VA(THIS(v).end())); THIS(it).inc()) {
        Int a = THIS(it).derefer();
        printf("%lld\n", (a)->val);
    }
    putchar('\n');
    printf("%ld\n", THIS(v).size());
    THIS(v).clear();
    printf("%ld\n", THIS(v).size());
    Form_t ttt = ARRAY_T(int, 100);
    delete(v);
    ARP_FreePool();
}*/

//
//unordered_multimap测试
/*int main(void)
{
    ARP_CreatePool();
    unordered_multimap mm = creat_unordered_multimap(sizeof(char*), sizeof(int), scmp, hash_str);
    pair(char*, int) p;
    p.first = "hello";
    p.second = 1;
    THIS(&mm).insert(&p);
    p.second = 2;
    THIS(&mm).insert(&p);
    p.second = 3;
    THIS(&mm).insert(&p);
    p.first = "world";
    p.second = 4;
    THIS(&mm).insert(&p);
    p.second = 5;
    THIS(&mm).insert(&p);
    p.second = 6;
    THIS(&mm).insert(&p);
    iterator(pair(char*, int)) fit = THIS(&mm).find(&p.first);
    printf("%s:%d\n", fit->val->first, fit->val->second);
    for (iterator(pair(char*, int)) it = THIS(&mm).begin(); !ITER(it).equal(THIS(&mm).end()); ITER(it).inc())
        printf("%s:%d\n", it->val->first, it->val->second);
    destory_unordered_multimap(&mm);
    ARP_FreePool();
}*/

//unordered_set测试
/*
void lookup(unordered_set *s, char *w)
{
    iterator(char*) it = THIS(s).find(&w);
    printf("%s:%s\n", w, it ? *it->val : "not present");
}
int main(void)
{
    ARP_CreatePool();
    unordered_set s = creat_unordered_set(sizeof(char*), scmp, hash_str);
    char *str = "kiwi";
    THIS(&s).insert(&str);
    str = "plum";
    THIS(&s).insert(&str);
    str = "apple";
    THIS(&s).insert(&str);
    str = "mango";
    THIS(&s).insert(&str);
    str = "apricot";
    THIS(&s).insert(&str);
    str = "banana";
    THIS(&s).insert(&str);
    lookup(&s, "mango");
    lookup(&s, "apple");
    lookup(&s, "durian");
    for (iterator(char*) it = THIS(&s).begin(); !ITER(it).equal(THIS(&s).end()); ITER(it).increment())
        printf("%s ", *it->val);
    putchar('\n');
    ARP_FreePool();
}*/

//map测试
/*int main(void)
{
    ARP_CreatePool();
    Map simap = new(T(Map), VA(ARRAY_T(char, 10), T(int), VA_FUNC(pcmp)));
    Pair p = tmpPair(ARRAY_T(char, 10), T(int), VA(VA_ADDR("jjhou"), 1), VAEND);//new(T(Pair), VA(ARRAY_T(char, 10), T(int), VA_ADDR("jjhou"), 1));
    THIS(simap).insert(p);

    strcpy(p->first, "jerry");
    *(int*)p->second = 2;
    THIS(simap).insert(p);

    strcpy(p->first, "jason");
    *(int*)p->second = 3;
    THIS(simap).insert(p);

    strcpy(p->first, "jimmy");
    *(int*)p->second = 4;
    THIS(simap).insert(p);

    strcpy(p->first, "david");
    *(int*)p->second = 5;
    THIS(simap).insert(p);
    for (Iterator it = THIS(simap).begin(); !THIS(it).equal(VA(THIS(simap).end())); THIS(it).inc()) {
        Pair pp = THIS(it).derefer();
        printf("key:%s val:%d\n", (char*)pp->first, *(int*)pp->second);
    }
    char key[10] = {"jjhou"};
    Iterator f_it = THIS(simap).find(VA(VA_ADDR(key)));
    //delete(p);
    p = THIS(f_it).derefer();
    printf("%d\n", *(int*)p->second);
    Iterator ite1;
    strcpy(key, "mchen");
    ite1 = THIS(simap).find(VA(VA_ADDR(key)));
    if (THIS(ite1).equal(VA(THIS(simap).end())))
        printf("mchen not found\n");
    strcpy(key, "jerry");
    ite1 = THIS(simap).find(VA(VA_ADDR(key)));
    if (!THIS(ite1).equal(VA(THIS(simap).end())))
        printf("jerry found\n");
    p = THIS(ite1).derefer();
    *(int*)p->second = 9;
    Iterator ite2 = THIS(simap).find(VA(VA_ADDR(key)));
    Pair pp = THIS(ite2).derefer();
    printf("%s %d f_it:%d count:%ld\n", (char*)p->first, *(int*)p->second, *(int*)pp->second, THIS(simap).count(VA(VA_ADDR(key))));
    ARP_FreePool();
    return 0;
}*/

//set测试
/*int main(void)
{
    ARP_CreatePool();
    int i;
    int ia[5] = {0, 1, 2, 3, 4};
    Set s = new(T(Set), VA(T(int), VA_FUNC(ocmp)));
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
    Set s1 = new(T(Set), VA(T(int), VA_FUNC(ocmp), THIS(v).begin(), THIS(v).end()));
    for (Iterator it = THIS(s1).begin(); !THIS(it).equal(VA(THIS(s1).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    ARP_FreePool();
    return 0;
}*/

//红黑树
/*int main(void)
{
    ARP_CreatePool();
    RB_tree tree = new(T(RB_tree), VA(T(long int), VA_FUNC(ocmp)));
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
    printf("find:%d\n", *(long int*)THIS(f_it).derefer());
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
}*/

//slist测试
/*int main(void)
{
    ARP_CreatePool();
    Slist sl = new(T(Slist), VA(T(int)));
    for (int i = 0; i < 20; i++) {
        int temp = random() % 20;
        printf("%d ", temp);
        THIS(sl).push_front(VA(temp));
    }
    putchar('\n');
    THIS(sl).reverse();
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    THIS(sl).pop_front();
    Iterator in_it = THIS(sl).begin();
    THIS(in_it).inc();
    THIS(in_it).inc();
    int temp = 100;
    THIS(sl).insert_after(in_it, VA(temp));
    THIS(in_it).inc();
    THIS(in_it).inc();
    THIS(sl).erase_after(in_it);
    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    printf("size:%ld\n", THIS(sl).size());
    printf("front:%d\n", *(int*)THIS(sl).front());
    Slist sl2 = new(T(Slist), VA(T(int), 2, 100));
    for (int i = 0; i < 100; i++) {
        int temp = random() % 20;
        THIS(sl2).push_front(VA(temp));
    }
    THIS(sl).swap(sl2);

    for (Iterator it = THIS(sl).begin(); !THIS(it).equal(VA(THIS(sl).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    for (Iterator it = THIS(sl2).begin(); !THIS(it).equal(VA(THIS(sl2).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    printf("%c\n", *("YN" + !THIS(sl).empty()));
    THIS(sl).clear();
    printf("%c\n", *("YN" + !THIS(sl).empty()));
    delete(sl);
    delete(sl2);
    ARP_FreePool();
}*/

//priority_Queue测试
/*int main(void)
{
    ARP_CreatePool();
    Priority_Queue pri_que = new(T(Priority_Queue), VA(T(int), VAEND, VA_FUNC(ocmp)));
    for (int i = 0; i < 300000; i++) {
        int temp = random();
        THIS(pri_que).push(VA(temp));
    }
    int pre = *(int*)THIS(pri_que).top();
    THIS(pri_que).pop();
    printf("%d ", pre);
    while (!THIS(pri_que).empty()) {
        int temp = *(int*)THIS(pri_que).top();
        printf("%d ", temp);
        if (temp > pre) {
            puts("bad!!!!");
            break;
        }
        THIS(pri_que).pop();
    }
    putchar('\n');
    delete(pri_que);
    ARP_FreePool();
}*/

//heap测试
/*int main(void)
{
    ARP_CreatePool();
    int temp;
    Vector v = new(T(Vector), VA(T(Int)));
    for (int i = 0; i < 15; i++) {
        temp = random() % 10;
        printf("%d ", temp);
        THIS(v).push_back(VA(temp));
    }
    putchar('\n');
    make_heap(THIS(v).begin(), THIS(v).end(), ocmp);
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%d ", ((Int)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    temp = 100;
    THIS(v).push_back(VA(temp));
    push_heap(THIS(v).begin(), THIS(v).end(), ocmp);
    printf("%d\n", ((Int)THIS(v).front())->val);
    temp = 0;
    THIS(v).push_back(VA(temp));
    push_heap(THIS(v).begin(), THIS(v).end(), ocmp);
    printf("%d\n", ((Int)THIS(v).back())->val);
    pop_heap(THIS(v).begin(), THIS(v).end(), ocmp);
    THIS(v).pop_back();
    sort_heap(THIS(v).begin(), THIS(v).end(), ocmp);
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%d ", ((Int)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    Deque deq = new(T(Deque), VA(T(int)));
    for (int i = 0; i < 100000; i++)
        THIS(deq).push_back(VA(random()));
    sort_heap(THIS(deq).begin(), THIS(deq).end(), ocmp);
    for (int i = 1; i < 100000; i++) {
        if (*(int*)THIS(deq).brackets(VA(i)) < *(int*)THIS(deq).brackets(VA(i - 1))) {
        //if (((Int)THIS(deq).brackets(VA(i)))->val < ((Int)THIS(deq).brackets(VA(i - 1)))->val) {
            printf("bad\n");
            break;
        }
    }
    putchar('\n');
    delete(v);
    ARP_FreePool();
    return 0;
}*/

//stack测试
/*int main(void)
{
    ARP_CreatePool();
    Stack st = new(T(Stack), VA(T(int)));
    for (int i = 0; i < 10; i++)
        THIS(st).push(VA(i));
    printf("top:%d\n", *(int*)THIS(st).top());
    printf("size:%ld\n", THIS(st).size());
    printf("empty:%c\n", *("YN" + !THIS(st).empty()));
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(st).top();
        THIS(st).pop();
        printf("%d ", temp);
    }
    putchar('\n');
    printf("empty:%c\n", *("YN" + !THIS(st).empty()));
    ARP_FreePool();
    return 0;
}*/

//queue测试
/*int main(void)
{
    ARP_CreatePool();
    Queue que = new(T(Queue), VA(T(int)));
    for (int i = 0; i < 10; i++)
        THIS(que).push(VA(i));
    printf("front:%d\n", *(int*)THIS(que).front());
    printf("back:%d\n", *(int*)THIS(que).back());
    printf("size:%ld\n", THIS(que).size());
    printf("empty:%c\n", *("YN" + !THIS(que).empty()));
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(que).front();
        printf("%d ", temp);
        THIS(que).pop();
    }
    putchar('\n');
    printf("empty:%c\n", *("YN" + !THIS(que).empty()));
    delete(que);
    ARP_FreePool();
}*/

//deque测试
/*int main(void)
{
    ARP_CreatePool();
    Deque deq = new(T(Deque), VA(T(int)));
    //init_deque(&deq, sizeof(int), 9);
    for (int i = 0; i < 10; i++)
        THIS(deq).push_back(VA(i));
    for (int i = 10; i < 20; i++)
        THIS(deq).push_front(VA(i));
    for (int i = 0; i < 5; i++)
        THIS(deq).pop_front();
    for (int i = 0; i < 5; i++)
        THIS(deq).pop_back();
    for (int i = 30; i < 40; i++)
        THIS(deq).push_back(VA(i));
    for (int i = 40; i < 50; i++)
        THIS(deq).push_front(VA(i));
    for (int i = 0; i < THIS(deq).size(); i++) {
        int *temp = (int *) THIS(deq).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    Iterator it_it = THIS(deq).begin();
    THIS(it_it).inc();
    //THIS(&deq).erase(&er_it);
    THIS(deq).erase(it_it);
    THIS(deq).erase(it_it);
    for (int i = 0; i < THIS(deq).size(); i++) {
        int *temp = (int *) THIS(deq).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    int t = 100;
    THIS(deq).insert(it_it, VA(t));
    for (int i = 0; i < THIS(deq).size(); i++) {
        int *temp = (int *) THIS(deq).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    t = 101;
    THIS(deq).insert(it_it, VA(t));
    t = 102;
    THIS(deq).insert(THIS(deq).begin(), VA(t));
    t = 103;
    THIS(deq).insert(THIS(deq).end(), VA(t));

    for (int i = 0; i < THIS(deq).size(); i++) {
        int *temp = (int *) THIS(deq).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    Iterator end_it = THIS(deq).end();
    THIS(end_it).dec();
    t = 104;
    THIS(deq).insert(end_it, VA(t));
    //THIS(&deq).clear();
    printf("front:%d\n", *(int*)THIS(deq).front());
    printf("back:%d\n", *(int*)THIS(deq).back());
    for (int i = 0; i < THIS(deq).size(); i++) {
        int *temp = (int *) THIS(deq).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    for (Iterator it = THIS(deq).begin(); !THIS(it).equal(VA(THIS(deq).end())); THIS(it).inc()) {
        int *a = THIS(it).derefer();
        printf("%d ", *a);
        long long r = THIS(it).dist(THIS(deq).end());
        r = THIS(it).dist(THIS(deq).begin());
        r = 0;
    }
    putchar('\n');
    Deque d = new(T(Deque), VA(T(int)));
    for (int i = 0; i < 10000; i++) {
        THIS(d).push_front(VA(i));
        THIS(d).push_back(VA(i));
    }
    Iterator xx = THIS(d).begin();
    THIS(xx).self_add(VA(10000));
    THIS(d).insert(xx, VA(30000));
    //THIS(xx).dec();
    THIS(xx).inc();
    THIS(xx).inc();
    THIS(d).erase(xx);
    for (int i = 0; i < THIS(d).size(); i++) {
        int *temp = THIS(d).brackets(VA(i));
        printf("%d ", *temp);
    }
    putchar('\n');
    delete(deq);
    ARP_FreePool();
}*/

//vector测试
/*void *thread(void *arg)
{
    ARP_CreatePool();
	Vector v = arg;
    //printf("%p\n", v);
	for (int i = 0; i < 1000; i++)
		THIS(v).begin();
	printf("end\n");
	for (int i = 0; i < 1000; i++)
		THIS(v).end();
	printf("over\n");
	ARP_FreePool();
}
int main(void)
{
    ARP_CreatePool();
    Vector v = new(T(Vector), VA(T(Double), 10, 3.20));
    printf("nmemb:%ld\n", THIS(v).size());
    Int i_t = new(T(Int));
    //printf("%d %d %d\n", sizeof(__iterator), sizeof(__private_vector), sizeof(a->start_ptr) + sizeof(a->start_iter));
    //init_vector(&v, 0, sizeof(int), NULL);
    //printf("%p\n", &v);
    pthread_t p1;
    pthread_t p2;
    pthread_create(&p1, NULL, thread, v);
    pthread_create(&p2, NULL, thread, v);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("thread ok\n");
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
}*/

// list测试
int main(void)
{
    ARP_CreatePool();
    List l = new(T(List), VA(T(Int)));
    //init_list(&l, sizeof(int));
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
    List l2 = new(T(List), VA(T(Int)));
    //init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(l2).push_back(VA(i));
    printf("splice\n");
    THIS(l).splice(THIS(l).begin(), l2, VA(THIS(l2).begin(), THIS(l2).end()), VAEND);
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
    printf("size:%ld\n", THIS(l).size());
    printf("merge\n");
    List l1 = new(T(List), VA(T(Int)));
    List l3 = new(T(List), VA(T(Int)));
    for (int i = 0; i < 10; i++) {
        THIS(l1).push_back(VA(i));
        THIS(l3).push_back(VA(i));
    }
    for (Iterator it = THIS(l1).begin(); !THIS(it).equal(VA(THIS(l1).end())); THIS(it).inc())
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    THIS(l1).merge(l3, ocmp);
    printf("merge\n");
    for (Iterator it = THIS(l1).begin(); !THIS(it).equal(VA(THIS(l1).end())); THIS(it).inc())
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    printf("sort\n");
    THIS(l).clear();
    for (int i = 0; i < 10000; i++) {
        temp = (int)random();
        THIS(l).push_back(VA(temp));
    }
    THIS(l).sort(ocmp);
    for (Iterator it = THIS(l).begin(); !THIS(it).equal(VA(THIS(l).end())); THIS(it).inc()) {
        printf("%lld\n", ((Int)THIS(it).derefer())->val);
    }
    for (int i = 1; i < 10000; i++) {
        if (((Int)THIS(l).brackets(VA(i)))->val < ((Int)THIS(l).brackets(VA(i - 1)))->val) {
            printf("bad\n");
            return 0;
        }
    }
    int dis = distance(THIS(l).begin(), THIS(l).end());
    printf("dist:%d\n", dis);
    List l4 = new(T(List), VA(T(Double), l1));
    for (int i = 0; i < THIS(l4).size(); i++) {
        printf("%lf\n", ((Double)THIS(l4).brackets(VA(i)))->val);
    }
    delete(l);
    delete(l1);
    delete(l2);
    delete(l3);
    printf("%d\n", ARP_GetPoolNodesCount());


    List l5 = new(T(List), VA(T(int), 3, 4));
    List l6 = new(T(List), VA(T(int), THIS(l5).begin(), THIS(l5).end()));
    for (Iterator it = THIS(l6).begin(); !THIS(it).equal(VA(THIS(l6).end())); THIS(it).inc()) {
        printf("%d\n", *(int*)THIS(it).derefer());
    }
    delete(l5);
    delete(l6);
    ARP_FreePool();
    return 0;
}
