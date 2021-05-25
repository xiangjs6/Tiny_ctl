//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algo.h"
#include "../include/tctl_algobase.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>

#define Import ITERATOR, ANY
#define ALLOC(size) ARP_MallocARelDtor(size, destroy)

inline static int CompareOpt(const void *a, const void *b, void *op)
{
    if (op == VAEND) {
        Object obj = (void*)a;
        return THIS(obj).cmp(b);
    } else {
        Compare cmp = *(Compare*)&op;
        return cmp(a, b);
    }
}

inline static bool EqualOpt(const void *a, const void *b, const void *op)
{
    if (op == VAEND) {
        Object obj = (void*)a;
        return THIS(obj).equal(b);
    } else {
        Equal eq = *(Equal*)&op;
        return eq(a, b);
    }
}

static inline void AssignOpt(void *left, const void *right)
{
    Object obj = left;
    THIS(obj).assign(right);
}

static unsigned int __rand_s(unsigned int *seed) //glibc的线程安全随机数产生函数
{
    unsigned int next = *seed;
    unsigned int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int) (next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    *seed = next;

    return result;
}

//Set Operation
Iterator set_union(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);

    void *a = THIS(first1).derefer();
    void *b = THIS(first2).derefer();
    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        int res = CompareOpt(a, b, op);
        if (res < 0) {
            AssignOpt(THIS(result).derefer(), a);
            THIS(first1).inc();
            a = THIS(first1).derefer();
        } else if (res > 0) {
            AssignOpt(THIS(result).derefer(), b);
            THIS(first2).inc();
            b = THIS(first2).derefer();
        } else {
            AssignOpt(THIS(result).derefer(), a);
            THIS(first1).inc();
            THIS(first2).inc();
            a = THIS(first1).derefer();
            b = THIS(first2).derefer();
        }
        THIS(result).inc();
    }
    Iterator out = copy(first1, _last1, result);
    out = copy(first2, _last2, out);
    return ARP_Return(out);
}

Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    void *a = THIS(first1).derefer();
    void *b = THIS(first2).derefer();
    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        int res = CompareOpt(a, b, op);
        if (res < 0) {
            THIS(first1).inc();
            a = THIS(first1).derefer();
        } else if (res > 0) {
            THIS(first2).inc();
            b = THIS(first2).derefer();
        } else {
            AssignOpt(THIS(result).derefer(), a);
            THIS(first1).inc();
            THIS(first2).inc();
            THIS(result).inc();
            a = THIS(first1).derefer();
            b = THIS(first2).derefer();
        }
    }
    return ARP_Return(result);
}

Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    void *a = THIS(first1).derefer();
    void *b = THIS(first2).derefer();
    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        int res = CompareOpt(a, b, op);
        if (res < 0) {
            AssignOpt(THIS(result).derefer(), a);
            THIS(result).inc();
            THIS(first1).inc();
            a = THIS(first1).derefer();
        } else if (res > 0) {
            THIS(first2).inc();
            b = THIS(first2).derefer();
        } else {
            THIS(first1).inc();
            THIS(first2).inc();
            a = THIS(first1).derefer();
            b = THIS(first2).derefer();
        }
    }
    Iterator out = copy(first1, _last1, result);
    return ARP_Return(out);
}

Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    void *a = THIS(first1).derefer();
    void *b = THIS(first2).derefer();
    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        int res = CompareOpt(a, b, op);
        if (res < 0) {
            AssignOpt(THIS(result).derefer(), a);
            THIS(result).inc();
            THIS(first1).inc();
            a = THIS(first1).derefer();
        } else if (res > 0) {
            AssignOpt(THIS(result).derefer(), b);
            THIS(result).inc();
            THIS(first2).inc();
            b = THIS(first2).derefer();
        } else {
            THIS(first1).inc();
            THIS(first2).inc();
            a = THIS(first1).derefer();
            b = THIS(first2).derefer();
        }
    }
    Iterator out = copy(first1, _last1, result);
    out = copy(first2, _last2, out);
    return ARP_Return(out);
}

//Data Processing 
//adjacent_find
Iterator adjacent_find(Iterator _first, Iterator _last, .../*Equal*/)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    if (THIS(first).equal(_last))
        return first;

    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator next = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (THIS(next).inc(); !THIS(next).equal(_last); THIS(first).inc(), THIS(next).inc()) {
        if (EqualOpt(THIS(next).derefer(), THIS(first).derefer(), op))
            break;
    }
    return ARP_Return(first);
}

//count
size_t count(Iterator _first, Iterator _last, const void *val, .../*Equal*/)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    size_t n = 0;
    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        if (EqualOpt(val, THIS(first).derefer(), op))
            n++;
    }
    ARP_FreePool();
    return n;
}

size_t count_if(Iterator _first, Iterator _last, Predicate pred)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    size_t n = 0;
    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        if (pred(THIS(first).derefer()))
            n++;
    }
    ARP_FreePool();
    return n;
}

//find
Iterator find(Iterator _first, Iterator _last, const void *val, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    while (!THIS(first).equal(_last) && !EqualOpt(val, THIS(first).derefer(), op))
        THIS(first).inc();
    return ARP_Return(first);
}

Iterator find_if(Iterator _first, Iterator _last, Predicate pred)
{
    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    while (!THIS(first).equal(_last) && !pred(THIS(first).derefer()))
        THIS(first).inc();
    return ARP_Return(first);
}

//find_end
static Iterator _find_end(Iterator first1, Iterator last1,
                          Iterator first2, Iterator last2,
                          void *op)
{
    if (THIS(first1).equal(last1))
        return THIS(last1).ctor(ALLOC(sizeOf(last1)), last1, VAEND);
    Iterator result = last1; //result是引用
    if (last1->rank < BidirectionalIter && last2->rank < BidirectionalIter) {
        while (true)
        {
            Iterator new_result = search(first1, last1, first2, last2, op);
            if (THIS(new_result).equal(last1))
                return result;
            result = new_result;
            THIS(first1).assign(new_result); //first1是赋值
            THIS(first1).inc();
        }
    } else {
        THIS(first1).dec();
        THIS(last1).dec();
        THIS(first2).dec();
        THIS(last2).dec();
        Iterator r_first1 = THIS(last1).reverse_iterator();
        Iterator r_last1 = THIS(first1).reverse_iterator();
        Iterator r_first2 = THIS(last2).reverse_iterator();
        Iterator r_last2 = THIS(first2).reverse_iterator();
        Iterator r_result = search(r_first1, r_last1, r_first2, r_last2, op);
        if (THIS(r_result).equal(r_last1)) {
            result = THIS(last1).ctor(ALLOC(sizeOf(last1)), last1, VAEND);
            THIS(result).inc();
        } else {
            result = THIS(r_result).reverse_iterator();
            advance(result, -distance(first2, last2) + 1); //这一段还有问题，看完逆向迭代器再改
        }
        return result;
    }
}

Iterator find_end(Iterator _first1, Iterator _last1,
                  Iterator _first2, Iterator _last2, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last2);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    Iterator result = _find_end(first1, _last1, first2, _last2, op);
    return ARP_Return(result);
}

Iterator find_first_of(Iterator _first1, Iterator _last1,
                       Iterator _first2, Iterator _last2, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last2);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    for (; !THIS(first1).equal(_last1); THIS(first1).inc()) {
        Iterator iter = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
        for (; !THIS(iter).equal(_last2); THIS(iter).inc())
            if (EqualOpt(THIS(first1).derefer(), THIS(iter).derefer(), op)) {
                return ARP_Return(first1);
            }
    }
    return ARP_Return(first1);
}

UnaryFunc for_each(Iterator _first, Iterator _last, UnaryFunc f)
{
    ARP_CreatePool();
    Iterator iter = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(iter).equal(_last); THIS(iter).inc())
        f(THIS(iter).derefer());
    ARP_FreePool();
    return f;
}

void generate(Iterator _first, Iterator _last, Generator gen)
{
    ARP_CreatePool();

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc())
        AssignOpt(THIS(first).derefer(), gen());
    ARP_FreePool();
}

Iterator generate_n(Iterator _first, size_t n, Generator gen)
{
    ARP_CreatePool();

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; n--; THIS(first).inc())
        AssignOpt(THIS(first).derefer(), gen());
    return ARP_Return(first);
}

bool includes(Iterator _first1, Iterator _last1,
              Iterator _first2, Iterator _last2, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last2);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        int res = CompareOpt(THIS(first1).derefer(),
                             THIS(first2).derefer(), op);
        if (res > 0) {
            ARP_FreePool();
            return false;
        } else if (res < 0)
            THIS(first1).inc();
        else {
            THIS(first1).inc();
            THIS(first2).inc();
        }
    }
    ARP_FreePool();
    return true;
}

Iterator max_element(Iterator _first, Iterator _last, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    if (THIS(_first).equal(_last)) return ARP_Return(result);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (THIS(first).inc(); !THIS(first).equal(_last); THIS(first).inc())
        if (CompareOpt(THIS(result).derefer(),THIS(first).derefer(), op) < 0)
            THIS(result).assign(first);
    return ARP_Return(result);
}

Iterator merge(Iterator _first1, Iterator _last1,
               Iterator _first2, Iterator _last2,
               Iterator _result, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    while (!THIS(first1).equal(_last1) && !THIS(first2).equal(_last2))
    {
        if (CompareOpt(THIS(first1).derefer(), THIS(first2).derefer(), op) < 0) {
            AssignOpt(THIS(result).derefer(), THIS(first1).derefer());
            THIS(first1).inc();
        } else {
            AssignOpt(THIS(result).derefer(),THIS(first2).derefer());
            THIS(first2).inc();
        }
        THIS(result).inc();
    }
    result = copy(first2, _last2, copy(first1, _last1, result));
    return ARP_Return(result);
}

Iterator min_element(Iterator _first, Iterator _last, ...)
{
    if (THIS(_first).equal(_last)) return THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (THIS(first).inc(); !THIS(first).equal(_last); THIS(first).inc())
        if (CompareOpt(THIS(result).derefer(),THIS(first).derefer(), op) > 0)
            THIS(result).assign(first);
    return ARP_Return(result);
}

Iterator partition(Iterator _first, Iterator _last, Predicate pred)
{
    ARP_CreatePool();

    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);

    while (true)
    {
        while (true)
            if (THIS(first).equal(last)) {
                return ARP_Return(first);
            } else if (pred(THIS(first).derefer()))
                THIS(first).inc();
            else
                break;
        THIS(last).dec();
        while (true)
            if (THIS(first).equal(last)) {
                return ARP_Return(first);
            } else if (!pred(THIS(last).derefer()))
                THIS(last).dec();
            else
                break;
        iter_swap(first, last);
        THIS(first).inc();
    }
}

Iterator remove_element(Iterator _first, Iterator _last, const void *val, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, val);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator first = find(_first, _last, val, op);
    if (THIS(first).equal(_last))
        return ARP_Return(first);
    Iterator next = THIS(first).ctor(ALLOC(sizeOf(first)), first, VAEND);
    THIS(next).inc();
    first = remove_copy(next, _last, first, val, op);
    return ARP_Return(first);
}

Iterator remove_copy(Iterator _first, Iterator _last,
                     Iterator _result, const void *val, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, val);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        void *v = THIS(first).derefer();
        if (!EqualOpt(v, val, op)) {
            AssignOpt(THIS(result).derefer(), v);
            THIS(result).inc();
        }
    }
    return ARP_Return(result);
}

Iterator remove_if(Iterator _first, Iterator _last, Predicate pred)
{
    ARP_CreatePool();

    Iterator first = find_if(_first, _last, pred);
    if (THIS(first).equal(_last))
        return ARP_Return(first);
    Iterator next = THIS(first).ctor(ALLOC(sizeOf(first)), first, VAEND);
    THIS(next).inc();
    first = remove_copy_if(next, _last, first, pred);
    return ARP_Return(first);
}

Iterator remove_copy_if(Iterator _first, Iterator _last,
                        Iterator _result, Predicate pred)
{
    ARP_CreatePool();

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        void *v = THIS(first).derefer();
        if (!pred(v)) {
            AssignOpt(THIS(result).derefer(), v);
            THIS(result).inc();
        }
    }
    return ARP_Return(result);
}

void replace(Iterator _first, Iterator _last, const void *old_val, const void *new_val, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, new_val);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        void *v = THIS(first).derefer();
        if (EqualOpt(v, old_val, op))
            AssignOpt(v, new_val);
    }
    ARP_FreePool();
}

Iterator replace_copy(Iterator _first, Iterator _last,
                      Iterator _result, const void *old_val,
                      const void *new_val, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, new_val);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc(), THIS(result).inc()) {
        void *v = THIS(first).derefer();
        if (EqualOpt(v, old_val, op))
            AssignOpt(THIS(result).derefer(), new_val);
        else
            AssignOpt(THIS(result).derefer(), v);
    }
    return ARP_Return(result);
}

void replace_if(Iterator _first, Iterator _last, Predicate pred, const void *new_val)
{
    ARP_CreatePool();

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        void *v = THIS(first).derefer();
        if (pred(v))
            AssignOpt(v, new_val);
    }
    ARP_FreePool();
}

Iterator replace_copy_if(Iterator _first, Iterator _last, Iterator _result, Predicate pred, const void *new_val)
{
    ARP_CreatePool();

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc(), THIS(result).inc()) {
        void *v = THIS(first).derefer();
        if (pred(v))
            AssignOpt(THIS(result).derefer(), new_val);
        else
            AssignOpt(THIS(result).derefer(), v);
    }
    return ARP_Return(result);
}

static void __reverse(Iterator _first, Iterator _last)
{
    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);
    if (first->rank >= RandomAccessIter && last->rank >= RandomAccessIter) {
        while (THIS(first).cmp(last) < 0)
        {
            THIS(last).dec();
            iter_swap(first, last);
            THIS(first).inc();
        }
    } else {
        while (true)
        {
            if (THIS(first).equal(last))
                return;
            THIS(last).dec();
            if (THIS(first).equal(last))
                return;
            iter_swap(first, last);
            THIS(first).inc();
        }
    }
}

void reverse(Iterator _first, Iterator _last)
{
    ARP_CreatePool();
    __reverse(_first, _last);
    ARP_FreePool();
}

Iterator reverse_copy(Iterator _first, Iterator _last, Iterator _result)
{
    ARP_CreatePool();

    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    Iterator last = THIS(_last).ctor(ALLOC(sizeOf(_last)), _last, VAEND);
    while (!THIS(_first).equal(last))
    {
        THIS(last).dec();
        AssignOpt(THIS(result).derefer(), THIS(last).derefer());
        THIS(result).inc();
    }
    return ARP_Return(result);
}

static size_t __gcd(size_t m, size_t n)
{
    while (n != 0)
    {
        size_t t = m % n;
        m = n;
        n = t;
    }
    return m;
}

static void __rotate_cycle(Iterator _first, Iterator _last, Iterator initial, size_t shift)
{
    void *val = construct(_first->class, ALLOC(classSz(_first->class)), VAEND);
    AssignOpt(val, THIS(initial).derefer());
    Iterator ptr1 = THIS(initial).ctor(ALLOC(sizeOf(initial)), initial, VAEND);
    Iterator ptr2 = THIS(ptr1).add(VA(shift));
    while (!THIS(ptr2).equal(initial))
    {
        AssignOpt(THIS(ptr1).derefer(), THIS(ptr2).derefer());
        THIS(ptr1).assign(ptr2);
        if (distance(ptr2, _last) > shift)
            THIS(ptr2).self_add(VA(shift));
        else
            ptr2 = THIS(_first).add(VA(shift - distance(ptr2, _last)));
    }
    AssignOpt(THIS(ptr1).derefer(), val);
}

static void __rotate(Iterator _first, Iterator _middle, Iterator _last)
{
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator middle = THIS(_middle).ctor(ALLOC(sizeOf(_middle)), _middle, VAEND);
    Iterator i = THIS(_middle).ctor(ALLOC(sizeOf(_middle)), _middle, VAEND);
    if (first->rank == ForwardIter) {
        while (true)
        {
            iter_swap(first, i);
            THIS(first).inc();
            THIS(i).inc();
            if (THIS(first).equal(middle)) {
                if (THIS(i).equal(_last)) return;
                THIS(middle).assign(i);
            } else if (THIS(i).equal(_last))
                THIS(i).assign(middle);
        }
    } else if (first->rank == BidirectionalIter) {
        reverse(first, middle);
        reverse(middle, _last);
        reverse(first, _last);
    } else {
        size_t n = __gcd(distance(first, _last), distance(first, middle));
        while (n--)
            __rotate_cycle(first, _last, THIS(first).add(VA(n)), distance(first, middle));
    }
}

void rotate(Iterator _first, Iterator _middle, Iterator _last)
{
    if (THIS(_first).equal(_middle) || THIS(_middle).equal(_last)) return;

    ARP_CreatePool();

    __rotate(_first, _middle, _last);
    ARP_FreePool();
}

Iterator rotate_copy(Iterator _first, Iterator _middle, Iterator _last, Iterator _result)
{
    ARP_CreatePool();
    return ARP_Return(copy(_first, _middle, copy(_middle, _last, _result)));
}

static Iterator __search(Iterator _first1, Iterator _last1,
                  Iterator _first2, Iterator _last2, void *op)
{
    size_t d1 = distance(_first1, _last1);
    size_t d2 = distance(_first2, _last2);
    Iterator last1 = THIS(_last1).ctor(ALLOC(sizeOf(_last1)), _last1, VAEND);
    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    if (d1 < d2) return last1;

    Iterator current1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator current2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);

    while (!THIS(current2).equal(_last2))
        if (EqualOpt(THIS(current1).derefer(), THIS(current2).derefer(), op)) {
            THIS(current1).inc();
            THIS(current2).inc();
        } else {
            if (d1 == d2)
                return ARP_Return(last1);
            else {
                THIS(first1).inc();
                THIS(current1).assign(first1);
                THIS(current2).assign(_first2);
                --d1;
            }
        }
    return first1;
}

Iterator search(Iterator _first1, Iterator _last1,
                Iterator _first2, Iterator _last2, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, _last2);
    void *op = va_arg(ap, void*);
    va_end(ap);

    return ARP_Return(__search(_first1, _last1, _first2, _last2, op));
}

Iterator search_n(Iterator _first, Iterator _last, long long count, const void *val, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void *);
    va_end(ap);

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator i = THIS(first).ctor(ALLOC(sizeOf(_first)), first, VAEND);
    if (count <= 0)
        return ARP_Return(first);
    else {
        first = find(first, _last, val, op);
        while (!THIS(first).equal(VA(_last)))
        {
            long long n = count - 1;
            THIS(i).assign(VA(first));
            THIS(i).inc();
            while (!THIS(i).equal(VA(_last)) && n != 0 && EqualOpt(THIS(i).derefer(), val, op))
            {
                THIS(i).inc();
                --n;
            }
            if (!n)
                break;
            else
                first = find(i, _last, val, op);
        }
        return ARP_Return(first);
    }
}

Iterator swap_ranges(Iterator _first1, Iterator _last1, Iterator _first2)
{
    ARP_CreatePool();

    Iterator first1 = THIS(_first1).ctor(ALLOC(sizeOf(_first1)), _first1, VAEND);
    Iterator first2 = THIS(_first2).ctor(ALLOC(sizeOf(_first2)), _first2, VAEND);
    for (; !THIS(first1).equal(_last1); THIS(first1).inc(), THIS(first2).inc())
        iter_swap(first1, first2);
    return ARP_Return(first2);
}

Iterator transform(Iterator _first, Iterator _last, Iterator _result, UnaryOperation unary_op)
{
    ARP_CreatePool();

    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_first)), _result, VAEND);
    for (; !THIS(first).equal(_last); THIS(first).inc(), THIS(result).inc())
        AssignOpt(THIS(result).derefer(), unary_op(THIS(first).derefer()));
    return ARP_Return(result);
}

Iterator unique(Iterator _first, Iterator _last, ...)
{
    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _last);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator first = adjacent_find(_first, _last, op);
    return ARP_Return(unique_copy(first, _last, first, op));
}

static Iterator __unique_copy(Iterator _first, Iterator _last, Iterator _result, void *equal)
{
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);

    void *val = THIS(first).derefer();
    AssignOpt(THIS(result).derefer(), val);
    for (THIS(first).inc(); !THIS(first).equal(_last); THIS(first).inc()) {
        val = THIS(first).derefer();
        if (!EqualOpt(THIS(result).derefer(), val, equal)) {
            THIS(result).inc();
            AssignOpt(THIS(result).derefer(), val);
        }
    }
    THIS(result).inc();
    return result;
}

Iterator unique_copy(Iterator _first, Iterator _last, Iterator _result, ...)
{
    if (THIS(_first).equal(_last))
        return THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);

    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _result);
    op = va_arg(ap, void*);
    va_end(ap);

    return ARP_Return(__unique_copy(_first, _last, _result, op));
}

static Iterator __lower_bound(Iterator _first, Iterator _last, const void *val, void *op)
{
    long long len = distance(_first, _last);
    long long half;
    Iterator middle = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);

    while (len > 0)
    {
        half = len >> 1;
        THIS(middle).assign(first);
        advance(middle, half);
        void *v = THIS(middle).derefer();
        if (CompareOpt(v, val, op) < 0) {
            THIS(first).assign(middle);
            THIS(first).inc();
            len = len - half - 1;
        } else 
            len = half;
    }
    return first;
}

Iterator lower_bound(Iterator _first, Iterator _last, const void *val, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);
    return ARP_Return(__lower_bound(_first, _last, val, op));
}

static Iterator __upper_bound(Iterator _first, Iterator _last, const void *val, void *op)
{
    size_t len = distance(_first, _last);
    size_t half;
    Iterator middle = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);

    while (len > 0)
    {
        half = len >> 1;
        THIS(middle).assign(first);
        advance(middle, half);
        void *v = THIS(middle).derefer();
        if (CompareOpt(val, v, op) < 0)
            len = half;
        else {
            THIS(first).assign(middle);
            THIS(first).inc();
            len = len - half - 1;
        }
    }
    return first;
}

Iterator upper_bound(Iterator _first, Iterator _last, const void *val, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);
    return ARP_Return(__upper_bound(_first, _last, val, op));
}

bool binary_search(Iterator _first, Iterator _last, const void *val, ...)
{
    ARP_CreatePool();
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);
    Iterator i = lower_bound(_first, _last, val, op);
    bool res = !THIS(i).equal(_last);
    res = res && !CompareOpt(val, THIS(i).derefer(), op);
    ARP_FreePool();
    return res;
}

bool next_permutation(Iterator _first, Iterator _last, ...)
{
    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    if (THIS(_first).equal(_last)) return false;

    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _last);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator i = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    THIS(i).inc();
    if (THIS(i).equal(_last)) {
        ARP_FreePool();
        return false;
    }
    THIS(i).assign(_last);
    THIS(i).dec();
    Iterator ii = THIS(i).ctor(ALLOC(sizeOf(i)), i, VAEND);
    Iterator j = THIS(i).ctor(ALLOC(sizeOf(i)), i, VAEND);

    for (;;) {
        THIS(ii).assign(i);
        THIS(i).dec();
        void *i_v = THIS(i).derefer();
        void *ii_v = THIS(ii).derefer();
        if (CompareOpt(i_v, ii_v, op) < 0) {
            THIS(j).assign(_last);
            THIS(j).dec();
            void *j_v = THIS(j).derefer();
            while (CompareOpt(i_v, j_v, op) >= 0)
            {
                THIS(j).dec();
                j_v = THIS(j).derefer();
            }
            iter_swap(i, j);
            reverse(ii, _last);
            ARP_FreePool();
            return true;
        }
        if (THIS(i).equal(_first)) {
            reverse(_first, _last);
            ARP_FreePool();
            return false;
        }
    }
}

bool prev_permutation(Iterator _first, Iterator _last, ...)
{
    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    if (THIS(_first).equal(_last)) return false;

    ARP_CreatePool();
    void *op;
    va_list ap;
    va_start(ap, _last);
    op = va_arg(ap, void*);
    va_end(ap);

    Iterator i = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    THIS(i).inc();
    if (THIS(i).equal(_last)) {
        ARP_FreePool();
        return false;
    }
    THIS(i).assign(_last);
    THIS(i).dec();
    Iterator ii = THIS(i).ctor(ALLOC(sizeOf(i)), i, VAEND);
    Iterator j = THIS(i).ctor(ALLOC(sizeOf(i)), i, VAEND);

    for (;;) {
        THIS(ii).assign(i);
        THIS(i).dec();
        void *i_v = THIS(i).derefer();
        void *ii_v = THIS(ii).derefer();
        if (CompareOpt(i_v, ii_v, op) > 0) {
            THIS(j).assign(_last);
            THIS(j).dec();
            void *j_v = THIS(j).derefer();
            while (CompareOpt(i_v, j_v, op) <= 0)
            {
                THIS(j).dec();
                j_v = THIS(j).derefer();
            }
            iter_swap(i, j);
            reverse(ii, _last);
            ARP_FreePool();
            return true;
        }
        if (THIS(i).equal(_first)) {
            reverse(_first, _last);
            ARP_FreePool();
            return false;
        }
    }
}

void random_shuffle(Iterator _first, Iterator _last, unsigned int *seed)
{
    assert(_first->rank >= RandomAccessIter && _last->rank >= RandomAccessIter);
    if (THIS(_first).equal(_last)) return;
    ARP_CreatePool();
    if (!seed) {
        for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(_last); THIS(i).inc()) {
            size_t n = distance(_first, i) + 1;
            iter_swap(i, THIS(_first).add(VA(rand() % n)));
        }
    } else {
        for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(_last); THIS(i).inc()) {
            size_t n = distance(_first, i) + 1;
            iter_swap(i, THIS(_first).add(VA(__rand_s(seed) % n)));
        }
    }
    ARP_FreePool();
}

void partial_sort(Iterator _first, Iterator _middle, Iterator _last, Compare cmp)
{
    assert(_first->rank >= RandomAccessIter && _middle->rank >= RandomAccessIter && _last->rank >= RandomAccessIter);
    ARP_CreatePool();
    make_heap(_first, _middle, cmp);
    Iterator i = THIS(_middle).ctor(ALLOC(sizeOf(_middle)), _middle, VAEND);
    for (; !THIS(i).equal(_last); THIS(i).inc()) {
        void *i_v = THIS(i).derefer();
        void *first_v = THIS(_first).derefer();
        if (cmp(i_v, first_v) < 0) {
            iter_swap(_first, i);
            pop_heap(_first, _middle, cmp);
            push_heap(_first, _middle, cmp);
        }
    }
    sort_heap(_first, _middle, cmp);
    ARP_FreePool();
}

void partial_sort_copy(Iterator _first, Iterator _last,
                       Iterator _res_first, Iterator _res_last, Compare cmp)
{
    assert(_res_first->rank >= RandomAccessIter && _res_last->rank >= RandomAccessIter);

    ARP_CreatePool();
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator res_real_last = THIS(_res_first).ctor(ALLOC(sizeOf(_res_first)), _res_first, VAEND);
    while (!THIS(first).equal(_last) && !THIS(res_real_last).equal(_res_first))
    {
        AssignOpt(THIS(res_real_last).derefer(), THIS(first).derefer());
        THIS(res_real_last).inc();
        THIS(first).inc();
    }

    make_heap(_res_first, res_real_last, cmp);

    for (; !THIS(first).equal(_last); THIS(first).inc()) {
        void *first_v = THIS(first).derefer();
        void *res_v = THIS(_res_first).derefer();
        if (cmp(first_v, res_v) < 0) {
            AssignOpt(res_v, first_v);
            pop_heap(_res_first, res_real_last, cmp);
            push_heap(_res_first, res_real_last, cmp);
        }
    }
    sort_heap(_res_first, res_real_last, cmp);
    ARP_FreePool();
}

//sort
const int __sort_threshold = 16;

static void __unguarded_linear_insert(Iterator _last, const void *val, void *op)
{
    Iterator last = THIS(_last).sub(VA(0)); //copy出一个迭代器
    Iterator next = THIS(_last).sub(VA(1));

    void *v_n = THIS(next).derefer();
    while (CompareOpt(val, v_n, op) < 0)
    {
        AssignOpt(THIS(last).derefer(), v_n);
        THIS(last).dec();
        THIS(next).dec();
        v_n = THIS(next).derefer();
    }
    AssignOpt(THIS(last).derefer(), val);
}

static void __linear_insert(Iterator _first, Iterator _last, void *op)
{
    void *val = construct(_first->class, ALLOC(classSz(_first->class)), VAEND);
    AssignOpt(val, THIS(_last).derefer());
    if (CompareOpt(THIS(_first).derefer(), val, op) > 0) {
        copy_backward(_first, _last, THIS(_last).add(VA(1)));
        AssignOpt(THIS(_first).derefer(), val);
    } else
        __unguarded_linear_insert(_last, val, op);
}

static void __insertion_sort(Iterator _first, Iterator _last, void *op)
{
    if (THIS(_first).equal(_last)) return;
    for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(_last); THIS(i).inc())
        __linear_insert(_first, i, op);
}

static inline void *__median(void *a, void *b, void *c, void *op)
{
    if (CompareOpt(a, b, op) < 0)
        if (CompareOpt(b, c, op) < 0)
            return b;
        else if (CompareOpt(a, c, op) < 0)
            return c;
        else
            return a;
    else if (CompareOpt(a, c, op) < 0)
        return a;
    else if (CompareOpt(b, c, op) < 0)
        return c;
    else
        return b;
}

static Iterator __unguarded_partition(Iterator _first, Iterator _last, const void *pivot, void *op)
{
    Iterator first = THIS(_first).add(VA(0));
    Iterator last = THIS(_last).add(VA(0));
    while (true)
    {
        while (CompareOpt(THIS(first).derefer(), pivot, op) < 0) THIS(first).inc();
        THIS(last).dec();
        while (CompareOpt(THIS(last).derefer(), pivot, op) > 0) THIS(last).dec();
        if (THIS(first).cmp(last) >= 0) return first;
        iter_swap(first, last);
        THIS(first).inc();
    }
}

static inline size_t __lg(size_t n)
{
    size_t k;
    for (k = 0; n > 1; n >>= 1) ++k;
    return k;
}

static void __introsort_loop(Iterator _first, Iterator _last, size_t depth_limit, void *op)
{
    long long dis = distance(_first, _last);
    while (dis > __sort_threshold)
    {
        if (depth_limit == 0) {
            partial_sort(_first, _last, _last, *(Compare*)&op); //这个cmp有点混乱
            return;
        }
        --depth_limit;
        Iterator m = THIS(_first).add(VA(dis / 2));
        Iterator last = THIS(_last).sub(VA(1));
        void *pivot = __median(THIS(_first).derefer(), THIS(m).derefer(), THIS(last).derefer(), op);
        Iterator cut = __unguarded_partition(_first, _last, pivot, op);
        __introsort_loop(cut, _last, depth_limit, op);
        _last = cut; //直接得到引用就行
    }
}

static void __unguarded_insertion_sort(Iterator _first, Iterator _last, void *op)
{
    for (Iterator i = THIS(_first).add(VA(0)); !THIS(i).equal(_last); THIS(i).inc())
        __unguarded_linear_insert(i, THIS(i).derefer(), op);
}

static void __final_insertion_sort(Iterator _first, Iterator _last, void *op)
{
    long long dis = distance(_first, _last);
    if (dis > __sort_threshold) {
        Iterator m = THIS(_first).add(VA(__sort_threshold));
        __insertion_sort(_first, m, op);
        __unguarded_insertion_sort(m, _last, op);
    } else
        __insertion_sort(_first, _last, op);
}

void sort(Iterator _first, Iterator _last, ...)
{
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);
    ARP_CreatePool();

    if (!THIS(_first).equal(_last)) {
        size_t n = distance(_first, _last);
        __introsort_loop(_first, _last, __lg(n) * 2, op);
        __final_insertion_sort(_first, _last, op);
    }
    ARP_FreePool();
}

//equal_range
Pair equal_range(Iterator _first, Iterator _last, const void *val, ...)
{
    va_list ap;
    va_start(ap, val);
    void *op = va_arg(ap, void*);
    va_end(ap);
    ARP_CreatePool();

    long long len = distance(_first, _last);
    size_t half;
    Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator middle = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator left, right;
    while (len > 0)
    {
        half = len >> 1;
        THIS(middle).assign(first);
        advance(middle, half);
        int res = CompareOpt(THIS(middle).derefer(), val, op);
        if (res < 0) {
            THIS(first).assign(middle);
            THIS(first).inc();
            len = len - half - 1;
        } else if (res > 0)
            len = half;
        else {
            left = lower_bound(first, middle, val, op);
            advance(first, len);
            THIS(middle).inc();
            right = upper_bound(middle, first, val, op);
            return ARP_Return(tmpPair(classOf(first), classOf(first), left, right, VAEND));
        }
    }
    Pair p = tmpPair(classOf(first), classOf(first), first, first, VAEND);
    return ARP_Return(p);
}

//inplace_merge
static Iterator __merge_backward(Iterator _first1, Iterator _last1, Iterator _first2, Iterator _last2, Iterator _result, void *op)
{
    if (THIS(_first1).equal(_last1))
        return copy_backward(_first2, _last2, _result);
    if (THIS(_first2).equal(_last2))
        return copy_backward(_first1, _last1, _result);
    Iterator last1 = THIS(_last1).ctor(ALLOC(sizeOf(_last1)), _last1, VAEND);
    Iterator last2 = THIS(_last2).ctor(ALLOC(sizeOf(_last2)), _last2, VAEND);
    Iterator result = THIS(_result).ctor(ALLOC(sizeOf(_result)), _result, VAEND);
    THIS(last1).dec();
    THIS(last2).dec();
    while (true)
    {
        void *v1 = THIS(last1).derefer();
        void *v2 = THIS(last2).derefer();
        int res = CompareOpt(v1, v2, op);
        if (res > 0) {
            AssignOpt(THIS(result).derefer(), v1);
            THIS(result).dec();
            if (THIS(_first1).equal(last1)) {
                THIS(last2).inc();
                return copy_backward(_first2, last2, result);
            }
            THIS(last1).dec();
        } else {
            AssignOpt(THIS(result).derefer(), v2);
            THIS(result).dec();
            if (THIS(_first2).equal(last2)) {
                THIS(last1).inc();
                return copy_backward(_first1, last1, result);
            }
            THIS(last2).dec();
        }
    }
}

static void *get_temporary_buffer(size_t *len)
{
    while (*len > 0)
    {
        void *tmp = malloc(*len);
        if (tmp)
            return tmp;
        *len >>= 1;
    }
    return NULL;
}

static Iterator __rotate_adaptive(Iterator _first, Iterator _middle, Iterator _last, size_t len1, size_t len2, Iterator _buff, size_t buff_size)
{
    Iterator buff_end;
    if (len1 > len2 && len2 <= buff_size)
    {
        buff_end = copy(_middle, _last, _buff);
        copy_backward(_first, _middle, _last);
        return copy_backward(_buff, buff_end, _last);
    } else if (len1 <= buff_size) {
        buff_end = copy(_first, _middle, _buff);
        copy(_middle, _last, _first);
        return copy_backward(_buff, buff_end, _last);
    } else {
        rotate(_first, _middle, _last);
        Iterator first = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
        advance(first, len2);
        return first;
    }
}

static void __merge_adaptive(Iterator _first, Iterator _middle, Iterator _last, size_t len1, size_t len2, Iterator _buff, size_t buff_size, void *op)
{
    if (len1 <= len2 && len1 <= buff_size) {
        Iterator end_buff = copy(_first, _middle, _buff);
        merge(_buff, end_buff, _middle, _last, _first, VAEND, op);
    } else if (len2 <= buff_size) {
        Iterator end_buff = copy(_middle, _last, _buff);
        __merge_backward(_first, _middle, _buff, end_buff, _last, op);
    } else {
        Iterator first_cut = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
        Iterator second_cut = THIS(_middle).ctor(ALLOC(sizeOf(_middle)), _middle, VAEND);
        size_t len11 = 0, len22 = 0;
        if (len1 > len2) {
            len11 = len1 / 2;
            advance(first_cut, len11);
            second_cut = lower_bound(_middle, _last, THIS(first_cut).derefer(), op);
            len22 = distance(_middle, second_cut);
        } else {
            len22 = len2 / 2;
            advance(second_cut, len22);
            first_cut = upper_bound(_first, _middle, THIS(second_cut).derefer(), op);
            len11 = distance(_first, first_cut);
        }
        Iterator new_middle = __rotate_adaptive(first_cut, _middle, second_cut, len1 - len11, len22, _buff, buff_size);
        __merge_adaptive(_first, first_cut, new_middle, len11, len22, _buff, buff_size, op);
        __merge_adaptive(new_middle, second_cut, _last, len1 - len11, len2 - len22, _buff, buff_size, op);
    }
}

static void __merge_without_buff(Iterator _first, Iterator _middle, Iterator _last, size_t len1, size_t len2, void *op)
{
    if (len1 == 0 || len2 == 0)
        return;
    if (len1 + len2 == 2) {
        if (CompareOpt(THIS(_first).derefer(), THIS(_middle).derefer(), op) > 0)
            iter_swap(_first, _middle);
        return;
    }
    Iterator first_cut = THIS(_first).ctor(ALLOC(sizeOf(_first)), _first, VAEND);
    Iterator second_cut = THIS(_middle).ctor(ALLOC(sizeOf(_middle)), _middle, VAEND);
    size_t len11 = 0, len22 = 0;
    if (len1 > len2) {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(_middle, _last, THIS(first_cut).derefer(), op);
        len22 = distance(_middle, second_cut);
    } else {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(_first, _middle, THIS(second_cut).derefer(), op);
        len11 = distance(_first, first_cut);
    }
    rotate(first_cut, _middle, second_cut);
    Iterator new_middle = THIS(first_cut).ctor(ALLOC(sizeOf(first_cut)), first_cut, VAEND);
    advance(new_middle, len22);
    __merge_without_buff(_first, first_cut, new_middle, len11, len22, op);
    __merge_without_buff(new_middle, second_cut, _last, len1 - len11, len2 - len22, op);
}

void inplace_merge(Iterator _first, Iterator _middle, Iterator _last, ...)
{
    assert(_first->rank >= BidirectionalIter && _middle->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);

    ARP_CreatePool();
    size_t size = classSz(_first->class);
    size_t len1 = distance(_first, _middle);
    size_t len2 = distance(_middle, _last);
    size_t buff_size = (len1 > len2 ? len2 : len1) * size;
    void *buff = get_temporary_buffer(&buff_size);
    buff_size /= size;
    void **buff_node;
    if (buff) {
        buff_node = malloc(buff_size * sizeof(void *));
        if (buff_node)
            for (int i = 0; i < buff_size; i++)
                buff_node[i] = (char *) buff + i * size;
        else
            buff = (free(buff), NULL);
    }
    if (!buff)
        __merge_without_buff(_first, _middle, _last, len1, len2, op);
    else {
        __merge_adaptive(_first, _middle, _last, len1, len2, _oriPointerIter_aux(_first->class, buff_node, 0), buff_size, op);
        free(buff_node);
        free(buff);
    }
    ARP_FreePool();
}

void nth_element(Iterator _first, Iterator _nth, Iterator _last, ...)
{
    assert(_first->rank >= RandomAccessIter && _nth->rank >= RandomAccessIter && _last->rank >= RandomAccessIter);
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);

    ARP_CreatePool();
    size_t dis;
    while ((dis = distance(_first, _last)) > 3) {
        Iterator m = THIS(_first).add(VA(dis / 2));
        Iterator last = THIS(_last).sub(VA(1));
        void *pivot = __median(THIS(_first).derefer(), THIS(m).derefer(), THIS(last).derefer(), op);
        Iterator cut = __unguarded_partition(_first, _last, pivot, op);
        if (CompareOpt(VA(cut), VA(_nth), VAEND) <= 0)
            _first = cut;
        else
            _last = cut;
    }
    __insertion_sort(_first, _last, op);
    ARP_FreePool();
}

void mergesort(Iterator _first, Iterator _last, ...)
{
    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    va_list ap;
    va_start(ap, _last);
    void *op = va_arg(ap, void*);
    va_end(ap);
    ARP_CreatePool();
    size_t n = distance(_first, _last);
    if (n > 1){
        Iterator mid = THIS(_first).add(VA(n / 2));
        mergesort(_first, mid, op);
        mergesort(mid, _last, op);
        inplace_merge(_first, mid, _last, op);
    }
    ARP_FreePool();
}
