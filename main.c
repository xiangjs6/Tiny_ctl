#include <stdio.h>
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

byte cmp(const int *a, const int *b)
{
    if (*a > *b)
        return 1;
    else if (*a < *b)
        return -1;
    return 0;
}

int main(void)
{
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
    n = 98;
    iterator *f_it = THIS(&tree).find(&n);
    THIS(&tree).erase(f_it);
    n = 96;
    f_it = THIS(&tree).find(&n);
    THIS(&tree).erase(f_it);
    n = 95;
    f_it = THIS(&tree).find(&n);
    THIS(&tree).erase(f_it);
    n = 97;
    f_it = THIS(&tree).find(&n);
    THIS(&tree).erase(f_it);
    n = 101;
    f_it = THIS(&tree).find(&n);
    THIS(&tree).erase(f_it);
    f_it = THIS(&tree).find(&n);
    printf("find:%d\n", *(int*)f_it->val);
    printf("%d %d\n", *(int*)watch->header->left->data, *(int*)watch->header->right->data);
    return 0;
}

/*
int main(void)
{
    slist sl = creat_slist(sizeof(int));
    for (int i = 0; i < 20; i++) {
        int temp = random() % 20;
        printf("%d ", temp);
        THIS(&sl).push_front(&temp);
    }
    putchar('\n');
    THIS(&sl).pop_front();
    iterator *in_it = NEW_ITER(THIS(&sl).begin());
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

    for (iterator *it = NEW_ITER(THIS(&sl).begin()); !ITER(it).equal(THIS(&sl).end()); ITER(it).increment())
        printf("%d ", *(int*)it->val);
    putchar('\n');
    for (iterator *it = NEW_ITER(THIS(&sl2).begin()); !ITER(it).equal(THIS(&sl2).end()); ITER(it).increment())
        printf("%d ", *(int*)it->val);
    putchar('\n');
    printf("%c\n", *("YN" + !THIS(&sl).empty()));
    destory_slist(&sl);
    destory_slist(&sl2);
    printf("%c\n", *("YN" + !THIS(&sl).empty()));
}
*/

//priority_queue测试
/*int main(void)
{
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
}*/

//heap测试
/*int main(void)
{
    int temp;
    deque v = creat_deque(sizeof(int), 1);
    for (int i = 0; i < 10; i++) {
        temp = random() % 10;
        printf("%d ", temp);
        THIS(&v).push_back(&temp);
    }
    putchar('\n');
//    temp = 100;
//    make_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
//    THIS(&v).push_back(&temp);
//    push_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
//    temp = 0;
//    THIS(&v).push_back(&temp);
//    push_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
//    pop_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
//    THIS(&v).pop_back();
    sort_heap(THIS(&v).begin(), THIS(&v).end(), cmp);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    destory_deque(&v);
    return 0;
}*/

//stack测试
/*int main(void)
{
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
    return 0;
}*/

//queue测试
/*int main(void)
{
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
}*/

//deque测试
/*
int main(void)
{
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
    iterator *it_it = NEW_ITER(THIS(&deq).begin());
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
    for (iterator *it = NEW_ITER(THIS(&deq).begin()); !ITER(it).equal(THIS(&deq).end()); ITER(it).increment()) {
        printf("%d ", *(int*)it->val);
        long long r = ITER(it).diff(THIS(&deq).end());
        r = ITER(it).diff(THIS(&deq).begin());
        r = 0;
    }
    putchar('\n');
    destory_deque(&deq);
}
*/

//vector测试
/*int main(void)
{
    vector v = creat_vector(0, sizeof(int), NULL);
    __private_vector *a = v.__obj_private;
    printf("%d %d %d\n", sizeof(iterator), sizeof(__private_vector), sizeof(a->start_ptr) + sizeof(a->start_iter));
    //init_vector(&v, 0, sizeof(int), NULL);
    for (int i = 0; i < 10; i++) {
        int temp = i;
        THIS(&v).push_back(&temp);
    }
    for (int i = 0; i < 5; i++)
        THIS(&v).pop_back();
    for (iterator *it = NEW_ITER(THIS(&v).begin()); it->val != THIS(&v).end()->val; ITER(it).increment()) {
        printf("%d ", *(int*)it->val);
    }
    putchar('\n');
    printf("insert\n");
    iterator *in_it = NEW_ITER(THIS(&v).begin());
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
    for (iterator *it = NEW_ITER(THIS(&v).begin()); it->val != THIS(&v).end()->val; ITER(it).increment()) {
        printf("%d ", *(int*)it->val);
        iterator *itt = NEW_ITER(THIS(&v).begin());
        int t = ITER(it).diff(itt);
        t = 0;
    }
    putchar('\n');
    printf("front:%d\n", *(int*)THIS(&v).front());
    printf("back:%d\n", *(int*)THIS(&v).back());
    printf("empty:%c\n", *("YN" + THIS(&v).empty()));
    THIS(&v).clear();
    printf("empty:%c\n", *("YN" + THIS(&v).empty()));
    printf("capacoty:%d\n", THIS(&v).capacity());
    THIS(&v).resize(9);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    destory_vector(&v);
}*/

// list测试
/*
int main(void)
{
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
    iterator *iter = NEW_ITER(THIS(&l).begin());
    THIS(&l).insert(iter, &temp);
    ITER(iter).increment();
    ITER(iter).increment();
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
        //THIS(&l).insert(*it, &temp);
    }
    printf("it:%d\n", *(int*)iter->val);
    THIS(&l).erase(iter);
    printf("first\n");
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
        //THIS(&l).insert(*it, &temp);
    }
    THIS(&l).remove(&temp1);
    temp1 = 4;
    THIS(&l).push_back(&temp1);
    printf("second\n");
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    THIS(&l).unique();
    printf("third\n");
    iterator *r_it = NEW_ITER(THIS(&l).end());
    ITER(r_it).decrement();
    printf("%d\n", *(int*)ITER(r_it).decrement());
    for (; r_it->val != THIS(&l).end()->val; ITER(r_it).decrement()) {
        printf("%d\n", *(int*)r_it->val);
    }
    list l2 = creat_list(sizeof(int));
    //init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(&l2).push_back(&i);
    printf("splice\n");
    THIS(&l).splice(THIS(&l).begin(), &l2, THIS(&l2).begin(), THIS(&l2).end());
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    printf("reverse\n");
    THIS(&l).reverse();
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
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
    for (iterator *it = NEW_ITER(THIS(&l1).begin()); it->val != THIS(&l1).end()->val; ITER(it).increment())
        printf("%d\n", *(int*)it->val);
    THIS(&l1).merge(&l3, cmp);
    for (iterator *it = NEW_ITER(THIS(&l1).begin()); it->val != THIS(&l1).end()->val; ITER(it).increment())
        printf("%d\n", *(int*)it->val);
    printf("sort\n");
    THIS(&l).clear();
    for (int i = 0; i < 10000; i++) {
        temp = (int)random();
        THIS(&l).push_back(&temp);
    }
    THIS(&l).sort(cmp);
    for (iterator *it = NEW_ITER(THIS(&l).begin()); it->val != THIS(&l).end()->val; ITER(it).increment()) {
        printf("%d\n", *(int*)it->val);
    }
    for (int i = 1; i < 10000; i++) {
        if (*(int*)THIS(&l).at(i) < *(int*)THIS(&l).at(i - 1)) {
            printf("bad\n");
            break;
        }
    }
    destory_list(&l);
    return 0;
}
*/
