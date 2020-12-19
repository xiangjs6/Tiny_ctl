#include <stdio.h>
#include "auto_release_pool.h"
#include "tctl_allocator.h"
#include "tctl_object.h"
#include "tctl_common.h"
#include "tctl_iterator.h"
#include "tctl_vector.h"
#include "tctl_list.h"
#include "tctl_deque.h"
#include "tctl_queue.h"
#include <stdlib.h>
#include "tctl_rb_tree.h"
#include "tctl_stack.h"
#include "tctl_heap.h"
#include "tctl_priority_queue.h"
#include "tctl_slist.h"
#include "tctl_set.h"
#include "tctl_map.h"
#include <pthread.h>
#include <string.h>

int cmp(const int *a, const int *b)
{
    if (*a > *b)
        return 1;
    else if (*a < *b)
        return -1;
    return 0;
}

int scmp(const char**a, const char**b)
{
    int res = strcmp(*a, *b);
    return res;
}

//map测试
/*
int main(void)
{
    ARP_CreatePool();
    map simap = creat_map(sizeof(char*), sizeof(long long), scmp);
    pair(char*, long long) p;
    p.first = allocate(10);
    strcpy(p.first, "jjhou");
    p.second = 1;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jerry");
    p.second = 2;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jason");
    p.second = 3;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "jimmy");
    p.second = 4;
    THIS(&simap).insert(&p);
    p.first = allocate(10);
    strcpy(p.first, "david");
    p.second = 5;
    THIS(&simap).insert(&p);
    for (iterator(pair(char*, long long)) it = THIS(&simap).begin(); !ITER(it).equal(THIS(&simap).end()); ITER(it).increment())
        printf("key:%s val:%lld\n", it->val->first, it->val->second);
    char key[10] = {"jjhou"};
    char *p_key = key;
    iterator(pair(char*, long long)) f_it = THIS(&simap).find(&p_key);
    printf("%lld\n", f_it->val->second);
    iterator(pair(char*, long long)) ite1 = INIT_ITERATOR;
    strcpy(key, "mchen");
    ITER(ite1).copy(THIS(&simap).find(&p_key));
    if (ITER(ite1).equal(THIS(&simap).end()))
        printf("mchen not found\n");
    strcpy(key, "jerry");
    ITER(ite1).copy(THIS(&simap).find(&p_key));
    if (!ITER(ite1).equal(THIS(&simap).end()))
        printf("jerry found\n");
    ite1->val->second = 9;
    iterator(pair(char*, long long)) ite2 = THIS(&simap).find(&p_key);
    printf("%lld f_it:%lld\n", ite2->val->second, f_it->val->second);
    ARP_FreePool();
    return 0;
}
*/

//set测试
/*int main(void)
{
    ARP_CreatePool();
    int i;
    int ia[5] = {0, 1, 2, 3, 4};
    set s = creat_set(sizeof(int), cmp);
    for (i = 0; i < 5; i++)
        THIS(&s).insert(&ia[i]);
    printf("size:%ld\n", THIS(&s).size());
    printf("3 count=%ld\n", THIS(&s).count(&ia[4]));
    i = 3;
    THIS(&s).insert(&i);
    printf("size:%ld\n", THIS(&s).size());
    printf("3 count=%ld\n", THIS(&s).count(&ia[4]));
    i = 5;
    THIS(&s).insert(&i);
    printf("size:%ld\n", THIS(&s).size());
    printf("3 count=%ld\n", THIS(&s).count(&ia[4]));
    i = 1;
    THIS(&s).erase(THIS(&s).find(&i));
    THIS(&s).erase(THIS(&s).end());
    THIS(&s).erase(THIS(&s).begin());
    for (iterator(int) it = THIS(&s).begin(); !ITER(it).equal(THIS(&s).end()); ITER(it).increment())
        printf("%d ", *it->val);
    putchar('\n');
    printf("size:%ld\n", THIS(&s).size());
    printf("3 count=%ld\n", THIS(&s).count(&ia[4]));
    printf("1 count=%ld\n", THIS(&s).count(&ia[1]));
    iterator(int) ite1 = THIS(&s).begin();
    iterator(int) ite2 = THIS(&s).end();
    for (; !ITER(ite1).equal(ite2); ITER(ite1).increment())
        printf("%d ", *ite1->val);
    putchar('\n');
    ITER(ite1).copy(THIS(&s).find(&ia[3]));
    if (!ITER(ite1).equal(ite2))
        printf("3 found\n");
    ITER(ite1).copy(THIS(&s).find(&ia[1]));
    if (ITER(ite1).equal(ite2))
        printf("1 not found\n");
    destory_set(&s);
    ARP_FreePool();
    return 0;
}*/

//红黑树
/*int main(void)
{
    ARP_CreatePool();
    rb_tree tree = creat_rb_tree(sizeof(int), cmp);
    int n = 100;
    THIS(&tree).insert_equal(&n);
    n = 95;
    THIS(&tree).insert_equal(&n);
    n = 96;
    //THIS(&tree).insert_equal(&n);
    THIS(&tree).insert_equal(&n);
    n = 97;
    THIS(&tree).insert_equal(&n);
    n = 105;
    THIS(&tree).insert_equal(&n);
    n = 96;
    THIS(&tree).insert_equal(&n);
    n = 101;
    THIS(&tree).insert_equal(&n);
    n = 99;
    THIS(&tree).insert_unique(&n);
    //THIS(&tree).insert_unique(&n);
    n = 98;
    THIS(&tree).insert_unique(&n);
    //n = 200;
    //THIS(&tree).insert_equal(&n);
    //n = 1;
    //THIS(&tree).insert_equal(&n);
    __private_rb_tree *watch = tree.__obj_private;
    iterator(int) it = INIT_ITERATOR;
    for (ITER(it).copy(THIS(&tree).begin()); !ITER(it).equal(THIS(&tree).end()); ITER(it).increment()) {
        printf("%d ", *it->val);
    }
    putchar('\n');
    for (ITER(it).decrement(); !ITER(it).equal(THIS(&tree).end()); ITER(it).decrement()) {
        printf("%d ", *it->val);
    }
    putchar('\n');
    n = 96;
    printf("%ld\n", THIS(&tree).count(&n));
    n = 98;
    iterator(int) f_it = INIT_ITERATOR;
    ITER(f_it).copy(THIS(&tree).find(&n));
    THIS(&tree).erase(f_it);
    n = 96;
    ITER(f_it).copy(THIS(&tree).find(&n));
    THIS(&tree).erase(f_it);
    n = 95;
    ITER(f_it).copy(THIS(&tree).find(&n));
    THIS(&tree).erase(f_it);
    n = 97;
    ITER(f_it).copy(THIS(&tree).find(&n));
    THIS(&tree).erase(f_it);
    n = 101;
    ITER(f_it).copy(THIS(&tree).find(&n));
    THIS(&tree).erase(f_it);
    n = 99;
    ITER(f_it).copy(THIS(&tree).find(&n));
    printf("find:%d\n", *f_it->val);
    printf("%d %d\n", *(int*)watch->header->left->data, *(int*)watch->header->right->data);
    for (ITER(it).copy(THIS(&tree).begin()); !ITER(it).equal(THIS(&tree).end()); ITER(it).increment()) {
        printf("%d ", *it->val);
    }
    putchar('\n');
    destory_rb_tree(&tree);
    ARP_FreePool();
    return 0;
}*/

//slist测试
/*int main(void)
{
    ARP_CreatePool();
    slist sl = creat_slist(sizeof(int));
    for (int i = 0; i < 20; i++) {
        int temp = random() % 20;
        printf("%d ", temp);
        THIS(&sl).push_front(&temp);
    }
    putchar('\n');
    THIS(&sl).pop_front();
    iterator(int) in_it = THIS(&sl).begin();
    ITER(in_it).increment();
    ITER(in_it).increment();
    int temp = 100;
    THIS(&sl).insert_after(in_it, &temp);
    ITER(in_it).increment();
    ITER(in_it).increment();
    THIS(&sl).erase_after(in_it);
    printf("size:%d\n", THIS(&sl).size());
    printf("front:%d\n", *(int*)THIS(&sl).front());
    slist sl2 = creat_slist(sizeof(int));
    for (int i = 0; i < 100; i++) {
        int temp = random() % 20;
        THIS(&sl2).push_front(&temp);
    }
    THIS(&sl).swap(&sl2);

    for (iterator(int) it = THIS(&sl).begin(); !ITER(it).equal(THIS(&sl).end()); ITER(it).increment())
        printf("%d ", *(int*)it->val);
    putchar('\n');
    for (iterator(int) it = THIS(&sl2).begin(); !ITER(it).equal(THIS(&sl2).end()); ITER(it).increment())
        printf("%d ", *(int*)it->val);
    putchar('\n');
    printf("%c\n", *("YN" + !THIS(&sl).empty()));
    destory_slist(&sl);
    destory_slist(&sl2);
    printf("%c\n", *("YN" + !THIS(&sl).empty()));
    ARP_FreePool();
}*/

//priority_queue测试
/*int main(void)
{
    ARP_CreatePool();
    priority_queue pri_que = creat_priority_queue(sizeof(int), cmp);
    for (int i = 0; i < 30; i++) {
        int temp = random() % 100;
        THIS(&pri_que).push(&temp);
    }
    while (!THIS(&pri_que).empty()) {
        printf("%d ", *(int*)THIS(&pri_que).top());
        THIS(&pri_que).pop();
    }
    putchar('\n');
    destory_priority_queue(&pri_que);
    ARP_FreePool();
}*/

//heap测试
int main(void)
{
    ARP_CreatePool();
    int temp;
    vector v = creat_vector(sizeof(int));
    for (int i = 0; i < 10; i++) {
        temp = random() % 10;
        printf("%d ", temp);
        THIS(&v).push_back(&temp);
    }
    putchar('\n');
    temp = 100;
    make_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    THIS(&v).push_back(&temp);
    push_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    printf("%d\n", *(int*)THIS(&v).front());
    temp = 0;
    THIS(&v).push_back(&temp);
    push_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    printf("%d\n", *(int*)THIS(&v).front());
    //pop_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    //THIS(&v).pop_back();
    //sort_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    destory_vector(&v);
    ARP_FreePool();
    return 0;
}

//stack测试
/*int main(void)
{
    ARP_CreatePool();
    stack st = creat_stack(sizeof(int));
    for (int i = 0; i < 10; i++)
        THIS(&st).push(&i);
    printf("top:%d\n", *(int*)THIS(&st).top());
    printf("size:%d\n", THIS(&st).size());
    printf("empty:%c\n", *("YN" + THIS(&st).empty()));
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(&st).top();
        THIS(&st).pop();
        printf("%d ", temp);
    }
    putchar('\n');
    printf("empty:%c\n", *("YN" + THIS(&st).empty()));
    ARP_FreePool();
    return 0;
}*/

//queue测试
/*
int main(void)
{
    ARP_CreatePool();
    queue que;// = creat_queue(sizeof(int));
    init_queue(&que, sizeof(int));
    for (int i = 0; i < 10; i++)
        THIS(&que).push(&i);
    printf("front:%d\n", *(int*)THIS(&que).front());
    printf("back:%d\n", *(int*)THIS(&que).back());
    printf("size:%d\n", THIS(&que).size());
    printf("empty:%c\n", *("YN" + THIS(&que).empty()));
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(&que).front();
        printf("%d ", temp);
        THIS(&que).pop();
    }
    putchar('\n');
    printf("empty:%c\n", *("YN" + THIS(&que).empty()));
    destory_queue(&que);
    ARP_FreePool();
}
*/

//deque测试
/*int main(void)
{
    ARP_CreatePool();
    deque deq = creat_deque(sizeof(int), 9);
    __private_deque *watch = deq.__obj_private;
    //init_deque(&deq, sizeof(int), 9);
    for (int i = 0; i < 10; i++)
        THIS(&deq).push_back(&i);
    for (int i = 10; i < 20; i++)
        THIS(&deq).push_front(&i);
    for (int i = 0; i < 5; i++)
        THIS(&deq).pop_front();
    for (int i = 0; i < 5; i++)
        THIS(&deq).pop_back();
    for (int i = 30; i < 40; i++)
        THIS(&deq).push_back(&i);
    for (int i = 40; i < 50; i++)
        THIS(&deq).push_front(&i);
    iterator(int) it_it = THIS(&deq).begin();
    __deque_iter *er_it = it_it->__inner.__address;
    er_it->cur += 4;
    //THIS(&deq).erase(&er_it);
    er_it->map_node++;
    er_it->first = *er_it->map_node;
    er_it->last = *er_it->map_node + 9 * sizeof(int);
    er_it->cur = er_it->first + 3 * sizeof(int);
    THIS(&deq).erase(it_it);
    er_it->map_node += 1;
    er_it->first = *er_it->map_node;
    er_it->last = *er_it->map_node + 9 * sizeof(int);
    er_it->cur = er_it->first + 3 * sizeof(int);
    THIS(&deq).erase(it_it);
    int t = 100;
    THIS(&deq).insert(it_it, &t);
    er_it->map_node -= 2;
    er_it->first = *er_it->map_node;
    er_it->last = *er_it->map_node + 9 * sizeof(int);
    er_it->cur = er_it->first + 5 * sizeof(int);
    t = 101;
    THIS(&deq).insert(it_it, &t);
    t = 102;
    THIS(&deq).insert(THIS(&deq).begin(), &t);
    //THIS(&deq).clear();
    for (int i = 0; i < THIS(&deq).size(); i++) {
        int *temp = (int *) THIS(&deq).at(i);
        printf("%d ", *temp);
    }
    putchar('\n');
    printf("front:%d\n", *(int*)THIS(&deq).front());
    printf("back:%d\n", *(int*)THIS(&deq).back());
    for (int i = 0; i < THIS(&deq).size(); i++) {
        int *temp = (int *) THIS(&deq).at(i);
        printf("%d ", *temp);
    }
    putchar('\n');
    for (iterator(int) it = THIS(&deq).begin(); !ITER(it).equal(THIS(&deq).end()); ITER(it).increment()) {
        printf("%d ", *it->val);
        long long r = ITER(it).diff(THIS(&deq).end());
        r = ITER(it).diff(THIS(&deq).begin());
        r = 0;
    }
    putchar('\n');
    destory_deque(&deq);
    ARP_FreePool();
}*/

//vector测试
/*
void *thread(void *arg)
{
    ARP_CreatePool();
	vector *v = arg;
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
    vector v;// = creat_vector(sizeof(int));
    init_vector(&v, sizeof(int));
    __private_vector *a = v.__obj_private;
    //printf("%d %d %d\n", sizeof(__iterator), sizeof(__private_vector), sizeof(a->start_ptr) + sizeof(a->start_iter));
    //init_vector(&v, 0, sizeof(int), NULL);
    //printf("%p\n", &v);
    pthread_t p1;
    pthread_t p2;
    pthread_create(&p1, NULL, thread, &v);
    pthread_create(&p2, NULL, thread, &v);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("thread ok\n");
    for (int i = 0; i < 10; i++) {
        int temp = i;
        THIS(&v).push_back(&temp);
    }
    for (int i = 0; i < 5; i++)
        THIS(&v).pop_back();
    for (iterator(int) it = THIS(&v).begin(); !ITER(it).equal(THIS(&v).end()); ITER(it).increment()) {
        printf("%d ", *it->val);
    }
    putchar('\n');
    printf("insert\n");
    iterator(int) in_it = THIS(&v).begin();
    int temp = 1000;
    THIS(&v).insert(in_it, &temp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    ITER(in_it).increment();
    ITER(in_it).increment();
    temp = 101;
    THIS(&v).insert(in_it, &temp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    ITER(in_it).add(3);
    temp = 102;
    THIS(&v).insert(in_it, &temp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    ITER(in_it).sub(2);
    temp = 103;
    THIS(&v).insert(in_it, &temp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    ITER(in_it).decrement();
    ITER(in_it).decrement();
    temp = 104;
    THIS(&v).insert(in_it, &temp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    ITER(in_it).increment();
    THIS(&v).erase(in_it);
    for (iterator(int) it = THIS(&v).begin(); !ITER(it).equal(THIS(&v).end()); ITER(it).increment()) {
        printf("%d ", *(int*)it->val);
        iterator(int) itt = THIS(&v).begin();
        int t = ITER(it).diff(itt);
        t = 0;
    }
    putchar('\n');
    printf("front:%d\n", *(int*)THIS(&v).front());
    printf("back:%d\n", *(int*)THIS(&v).back());
    printf("empty:%c\n", *("NY" + THIS(&v).empty()));
    THIS(&v).clear();
    printf("empty:%c\n", *("NY" + THIS(&v).empty()));
    printf("capacoty:%d\n", THIS(&v).capacity());
    THIS(&v).resize(9);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    destory_vector(&v);
    ARP_FreePool();
}*/

// list测试
/*int main(void)
{
    ARP_CreatePool();
    list l = creat_list(sizeof(int));
    //init_list(&l, sizeof(int));
    int temp = 1;
    for (int i = 0; i < 10; i++) {
        THIS(&l).push_back(&i);
    }
    for (int i = 0; i < 5; i++) {
        THIS(&l).pop_back();
    }
    int temp1 = 20;
    THIS(&l).pop_front();
    THIS(&l).push_front(&temp1);
    iterator(int) iter = THIS(&l).begin();
    THIS(&l).insert(iter, &temp);
    ITER(iter).increment();
    ITER(iter).increment();
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *it->val);
        //THIS(&l).insert(*it, &temp);
    }
    printf("it:%d\n", *(int*)iter->val);
    THIS(&l).erase(iter);
    printf("first\n");
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
        //THIS(&l).insert(*it, &temp);
    }
    THIS(&l).remove(&temp1);
    temp1 = 4;
    THIS(&l).push_back(&temp1);
    printf("second\n");
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    THIS(&l).unique();
    printf("third\n");
    iterator(int) r_it = THIS(&l).end();
    ITER(r_it).decrement();
    printf("%d\n", *(int*)ITER(r_it).decrement());
    for (; ITER(r_it).equal(THIS(&l).end()); !ITER(r_it).decrement()) {
        printf("%d\n", *(int*)r_it->val);
    }
    list l2 = creat_list(sizeof(int));
    //init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(&l2).push_back(&i);
    printf("splice\n");
    THIS(&l).splice(THIS(&l).begin(), &l2, THIS(&l2).begin(), THIS(&l2).end());
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    printf("reverse\n");
    THIS(&l).reverse();
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    printf("front%d\n", *(int*)THIS(&l).front());
    printf("back%d\n", *(int*)THIS(&l).back());
    printf("size:%d\n", THIS(&l).size());
    printf("merge\n");
    list l1 = creat_list(sizeof(int));
    list l3 = creat_list(sizeof(int));
    //init_list(&l1, sizeof(int));
    //init_list(&l3, sizeof(int));
    for (int i = 0; i < 10; i++) {
        THIS(&l1).push_back(&i);
        THIS(&l3).push_back(&i);
    }
    for (iterator(int) it = THIS(&l1).begin(); !ITER(it).equal(THIS(&l1).end()); ITER(it).increment())
        printf("%d\n", *(int*)it->val);
    THIS(&l1).merge(&l3, cmp);
    for (iterator(int) it = THIS(&l1).begin(); !ITER(it).equal(THIS(&l1).end()); ITER(it).increment())
        printf("%d\n", *(int*)it->val);
    printf("sort\n");
    THIS(&l).clear();
    for (int i = 0; i < 10000; i++) {
        temp = (int)random();
        THIS(&l).push_back(&temp);
    }
    THIS(&l).sort(cmp);
    for (iterator(int) it = THIS(&l).begin(); !ITER(it).equal(THIS(&l).end()); ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    for (int i = 1; i < 10000; i++) {
        if (*(int*)THIS(&l).at(i) < *(int*)THIS(&l).at(i - 1)) {
            printf("bad\n");
            break;
        }
    }
    destory_list(&l);
    printf("%d\n", ARP_GetPoolNodesCount());
    ARP_FreePool();
    return 0;
}*/
