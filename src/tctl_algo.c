//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algo.h"
#include "../include/tctl_algobase.h"
#include "../include/tctl_heap.h"
#include "../include/tctl_metaclass.h"
#include "../include/auto_release_pool.h"
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>

#define Import ITERATOR
#define ARR_LEN(a) (sizeof(a) / sizeof(*a))
#define SWAP_FormWO(a, b) \
do                        \
{                         \
    FormWO_t tmp##a = a;  \
    a = b;                \
    b = tmp##a;           \
} while(0)

inline static int CompareOpt(FormWO_t a, FormWO_t b, FormWO_t op)
{
    if (a._.f != OBJ)
        SWAP_FormWO(a, b);
    if (op._.f != FUNC) {
        if (a._.f == OBJ) {
            Object obj = a.mem;
            return THIS(obj).cmp(b);
        } else {
            if (op._.f == ADDR)
                return memcmp(a.mem, b.mem, a._.size);
            else
                return memcmp(&a.mem, &b.mem, a._.size);
        }
    } else {
        Compare cmp = op.mem;
        return cmp(a, b);
    }
}

inline static bool EqualOpt(FormWO_t a, FormWO_t b, FormWO_t op)
{
    if (a._.f != OBJ)
        SWAP_FormWO(a, b);
    if (op._.f != FUNC) {
        if (a._.f == OBJ) {
            Object obj = a.mem;
            return THIS(obj).equal(b);
        } else {
            if (op._.f == ADDR)
                return memcmp(a.mem, b.mem, a._.size);
            else
                return memcmp(&a.mem, &b.mem, a._.size);
        }
    } else {
        Equal eq = op.mem;
        return eq(a, b);
    }
}

static inline void AssignOpt(FormWO_t left, FormWO_t right, FormWO_t op)
{
    if (op._.f != FUNC) {
        if (left._.f == OBJ) {
            Object obj = left.mem;
            THIS(obj).assign(right);
        } else {
            if (op._.f == ADDR)
                memcpy(left.mem, right.mem, left._.size);
            else
                memcpy(&left.mem, &right.mem, left._.size);
        }
    } else {
        AssignOperation func = op.mem;
        func(left, right);
    }
}

static int __rand_s(unsigned int *seed) //glibc的线程安全随机数产生函数
{
    unsigned int next = *seed;
    int result;

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
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Iterator result = THIS(_result).ctor(NULL, VA(_result), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    Form_t f3 = THIS(result).type();

    FormWO_t a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
    FormWO_t b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        int res = CompareOpt(a, b, op[Compare]);
        if (res < 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), a, op[Assign]);
            THIS(first1).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
        } else if (res > 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), b, op[Assign]);
            THIS(first2).inc();
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        } else {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), a, op[Assign]);
            THIS(first1).inc();
            THIS(first2).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        }
        THIS(result).inc();
    }
    Iterator out = copy(first1, _last1, result);
    out = copy(first2, _last2, out);
    delete(first1);
    delete(first2);
    delete(result);
    return out;
}

Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    Form_t f3 = THIS(result).type();

    FormWO_t a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
    FormWO_t b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        int res = CompareOpt(a, b, op[Compare]);
        if (res < 0) {
            THIS(first1).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
        } else if (res > 0) {
            THIS(first2).inc();
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        } else {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), a, op[Assign]);
            THIS(first1).inc();
            THIS(first2).inc();
            THIS(result).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        }
    }
    delete(first1);
    delete(first2);
    return result;
}

Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator result = THIS(_result).ctor(NULL, VA(_result), VAEND);
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    Form_t f3 = THIS(result).type();

    FormWO_t a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
    FormWO_t b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        int res = CompareOpt(a, b, op[Compare]);
        if (res < 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), a, op[Assign]);
            THIS(result).inc();
            THIS(first1).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
        } else if (res > 0) {
            THIS(first2).inc();
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        } else {
            THIS(first1).inc();
            THIS(first2).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        }
    }
    Iterator out = copy(first1, _last1, result);
    delete(first1);
    delete(first2);
    delete(result);
    return out;
}

Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator result = THIS(_result).ctor(NULL, VA(_result), VAEND);
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();
    Form_t f3 = THIS(result).type();

    FormWO_t a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
    FormWO_t b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        int res = CompareOpt(a, b, op[Compare]);
        if (res < 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), a, op[Assign]);
            THIS(result).inc();
            THIS(first1).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
        } else if (res > 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()), b, op[Assign]);
            THIS(result).inc();
            THIS(first2).inc();
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        } else {
            THIS(first1).inc();
            THIS(first2).inc();
            a = FORM_WITH_OBJ(f1, THIS(first1).derefer());
            b = FORM_WITH_OBJ(f2, THIS(first2).derefer());
        }
    }
    Iterator out = copy(first1, _last1, result);
    out = copy(first2, _last2, out);
    delete(first1);
    delete(first2);
    delete(result);
    return out;
}

//Data Processing 
//adjacent_find
Iterator adjacent_find(Iterator _first, Iterator _last, .../*Equal*/)
{
    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    if (THIS(first).equal(VA(_last)))
        return first;

    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator next = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(next).type();
    for (THIS(next).inc(); !THIS(next).equal(VA(_last)); THIS(first).inc(), THIS(next).inc()) {
        if (EqualOpt(FORM_WITH_OBJ(f, THIS(next).derefer()),
                     FORM_WITH_OBJ(f, THIS(first).derefer()), op))
            break;
    }
    delete(next);
    return first;
}

//count
size_t count(Iterator _first, Iterator _last, FormWO_t val, .../*Equal*/)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    size_t n = 0;
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        if (EqualOpt(val, FORM_WITH_OBJ(f, THIS(first).derefer()), op))
            n++;
    }
    delete(first);
    return n;
}

size_t count_if(Iterator _first, Iterator _last, Predicate pred)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    size_t n = 0;
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        if (pred(FORM_WITH_OBJ(f, THIS(first).derefer())))
            n++;
    }
    delete(first);
    return n;
}

//find
Iterator find(Iterator _first, Iterator _last, FormWO_t val, ...)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    while (!THIS(first).equal(VA(_last)) &&
           !EqualOpt(val, FORM_WITH_OBJ(f, THIS(first).derefer()), op))
        THIS(first).inc();
    return first;
}

Iterator find_if(Iterator _first, Iterator _last, Predicate pred)
{
    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    while (!THIS(first).equal(VA(_last)) &&
           !pred(FORM_WITH_OBJ(f, THIS(first).derefer())))
        THIS(first).inc();
    return first;
}

//find_end
static Iterator _find_end(Iterator first1, Iterator last1,
                          Iterator first2, Iterator last2,
                          FormWO_t op)
{
    if (THIS(first1).equal(VA(last1))) {
        void *mem = ARP_MallocARelDtor(sizeOf(last1), destroy);
        return THIS(last1).ctor(mem, VA(last1), VAEND);
    }
    Iterator result = last1; //result是引用
    if (last1->rank < BidirectionalIter && last2->rank < BidirectionalIter) {
        while (true)
        {
            Iterator new_result = search(first1, last1, first2, last2, op);
            if (THIS(new_result).equal(VA(last1)))
                return result;
            result = new_result;
            THIS(first1).assign(VA(new_result)); //first1是赋值
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
        if (THIS(r_result).equal(VA(r_last1))) {
            void *mem = ARP_MallocARelDtor(sizeOf(last1), destroy);
            result = THIS(last1).ctor(mem, VA(last1), VAEND);
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
    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Iterator result = _find_end(first1, _last1, first2, _last2, op);
    delete(first1);
    delete(first2);
    return result;
}

Iterator find_first_of(Iterator _first1, Iterator _last1,
                       Iterator _first2, Iterator _last2, ...)
{
    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_first1), destroy);
    Iterator first1 = THIS(_first1).ctor(mem, VA(_first1), VAEND);
    Form_t f1 = THIS(_first1).type();
    Form_t f2 = THIS(_first2).type();
    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc()) {
        Iterator iter = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
        for (; !THIS(iter).equal(VA(_last2)); THIS(iter).inc())
            if (EqualOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                         FORM_WITH_OBJ(f2, THIS(iter).derefer()),
                         op)) {
                delete(iter);
                return first1;
            }
        delete(iter);
    }
    return first1;
}

UnaryFunc for_each(Iterator _first, Iterator _last, UnaryFunc f)
{
    Iterator iter = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t t = THIS(_first).type();
    for (; !THIS(iter).equal(VA(_last)); THIS(iter).inc())
        f(FORM_WITH_OBJ(t, THIS(iter).derefer()));
    delete(iter);
    return f;
}

void generate(Iterator _first, Iterator _last, Generator gen, ...)
{
    va_list ap;
    va_start(ap, gen);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), gen(), op);
    delete(first);
}

Iterator generate_n(Iterator _first, size_t n, Generator gen, ...)
{
    va_list ap;
    va_start(ap, gen);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    for (; n--; THIS(first).inc())
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), gen(), op);
    return first;
}

bool includes(Iterator _first1, Iterator _last1,
              Iterator _first2, Iterator _last2, ...)
{
    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(first1).type();
    Form_t f2 = THIS(first2).type();

    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        int res = CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                             FORM_WITH_OBJ(f2, THIS(first2).derefer()), op);
        if (res > 0) {
            delete(first1);
            delete(first2);
            return false;
        } else if (res < 0)
            THIS(first1).inc();
        else {
            THIS(first1).inc();
            THIS(first2).inc();
        }
    }
    delete(first1);
    delete(first2);
    return true;
}

Iterator max_element(Iterator _first, Iterator _last, ...)
{
    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator result = THIS(_first).ctor(mem, VA(_first), VAEND);
    if (THIS(_first).equal(VA(_last))) return result;
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(_first).type();
    for (THIS(first).inc(); !THIS(first).equal(VA(_last)); THIS(first).inc())
        if (CompareOpt(FORM_WITH_OBJ(f, THIS(result).derefer()),
                       FORM_WITH_OBJ(f, THIS(first).derefer()), op) < 0)
            THIS(result).assign(VA(first));
    delete(first);
    return result;
}

Iterator merge(Iterator _first1, Iterator _last1,
               Iterator _first2, Iterator _last2,
               Iterator _result, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Iterator result = THIS(_result).ctor(NULL, VA(_result), VAEND);
    Form_t f1 = THIS(_first1).type();
    Form_t f2 = THIS(_first2).type();
    Form_t f3 = THIS(_result).type();
    while (!THIS(first1).equal(VA(_last1)) && !THIS(first2).equal(VA(_last2)))
    {
        if (CompareOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                       FORM_WITH_OBJ(f2, THIS(first2).derefer()), op[Compare]) < 0) {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()),
                      FORM_WITH_OBJ(f1, THIS(first1).derefer()), op[Assign]);
            THIS(first1).inc();
        } else {
            AssignOpt(FORM_WITH_OBJ(f3, THIS(result).derefer()),
                      FORM_WITH_OBJ(f2, THIS(first2).derefer()), op[Assign]);
            THIS(first2).inc();
        }
        THIS(result).inc();
    }
    Iterator tmp = result;
    result = copy(first2, _last2, copy(first1, _last1, result));
    delete(tmp);
    delete(first1);
    delete(first2);
    return result;
}

Iterator min_element(Iterator _first, Iterator _last, ...)
{
    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator result = THIS(_first).ctor(mem, VA(_first), VAEND);
    if (THIS(_first).equal(VA(_last))) return result;
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(_first).type();
    for (THIS(first).inc(); !THIS(first).equal(VA(_last)); THIS(first).inc())
        if (CompareOpt(FORM_WITH_OBJ(f, THIS(result).derefer()),
                       FORM_WITH_OBJ(f, THIS(first).derefer()), op) > 0)
            THIS(result).assign(VA(first));
    delete(first);
    return result;
}

Iterator partition(Iterator _first, Iterator _last, Predicate pred, ...)
{
    va_list ap;
    va_start(ap, pred);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    Form_t f = THIS(_first).type();
    
    while (true)
    {
        while (true)
            if (THIS(first).equal(VA(last))) {
                delete(last);
                return first;
            } else if (pred(FORM_WITH_OBJ(f, THIS(first).derefer())))
                THIS(first).inc();
            else
                break;
        THIS(last).dec();
        while (true)
            if (THIS(first).equal(VA(last))) {
                delete(last);
                return first;
            } else if (!pred(FORM_WITH_OBJ(f, THIS(last).derefer())))
                THIS(last).dec();
            else
                break;
        iter_swap(first, last, op);
        THIS(first).inc();
    }
}

Iterator remove_element(Iterator _first, Iterator _last, FormWO_t val, ...)
{
    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, val);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = find(_first, _last, val, op[Equal]);
    if (THIS(first).equal(VA(_last)))
        return first;
    Iterator next = THIS(first).ctor(NULL, VA(first), VAEND);
    THIS(next).inc();
    first = remove_copy(next, _last, first, val, op[Assign], op[Equal]);
    delete(next);
    return first;
}

Iterator remove_copy(Iterator _first, Iterator _last,
                     Iterator _result, FormWO_t val, ...)
{
    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, val);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f1 = THIS(first).type();
    Form_t f2 = THIS(result).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f1, THIS(first).derefer());
        if (!EqualOpt(v, val, op[Equal])) {
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      v, op[Assign]);
            THIS(result).inc();
        }
    }
    delete(first);
    return result;
}

Iterator remove_if(Iterator _first, Iterator _last, Predicate pred, ...)
{
    va_list ap;
    va_start(ap, pred);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = find_if(_first, _last, pred);
    if (THIS(first).equal(VA(_last)))
        return first;
    Iterator next = THIS(first).ctor(NULL, VA(first), VAEND);
    THIS(next).inc();
    first = remove_copy_if(next, _last, first, pred, op);
    delete(next);
    return first;
}

Iterator remove_copy_if(Iterator _first, Iterator _last,
                        Iterator _result, Predicate pred, ...)
{
    va_list ap;
    va_start(ap, pred);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f1 = THIS(first).type();
    Form_t f2 = THIS(result).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f1, THIS(first).derefer());
        if (!pred(v)) {
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      v, op);
            THIS(result).inc();
        }
    }
    delete(first);
    return result;
}

void replace(Iterator _first, Iterator _last, FormWO_t old_val, FormWO_t new_val, ...)
{
    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, new_val);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(_first).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f, THIS(first).derefer());
        if (EqualOpt(v, old_val, op[Equal]))
            AssignOpt(v, new_val, op[Assign]);
    }
    delete(first);
}

Iterator replace_copy(Iterator _first, Iterator _last,
                      Iterator _result, FormWO_t old_val,
                      FormWO_t new_val, ...)
{
    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, new_val);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f1 = THIS(_first).type();
    Form_t f2 = THIS(_result).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc(), THIS(result).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f1, THIS(first).derefer());
        if (EqualOpt(v, old_val, op[Equal]))
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      new_val, op[Assign]);
        else
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      v, op[Assign]);
    }
    delete(first);
    return result;
}

void replace_if(Iterator _first, Iterator _last, Predicate pred, FormWO_t new_val, ...)
{
    va_list ap;
    va_start(ap, new_val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(_first).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f, THIS(first).derefer());
        if (pred(v))
            AssignOpt(v, new_val, op);
    }
    delete(first);
}

Iterator replace_copy_if(Iterator _first, Iterator _last, Iterator _result, Predicate pred, FormWO_t new_val, ...)
{
    va_list ap;
    va_start(ap, new_val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f1 = THIS(_first).type();
    Form_t f2 = THIS(_result).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc(), THIS(result).inc()) {
        FormWO_t v = FORM_WITH_OBJ(f1, THIS(first).derefer());
        if (pred(v))
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      new_val, op);
        else
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                      v, op);
    }
    delete(first);
    return result;
}

static void __reverse(Iterator _first, Iterator _last, FormWO_t op)
{
    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    if (first->rank >= RandomAccessIter && last->rank >= RandomAccessIter) {
        while (THIS(first).cmp(VA(last)) < 0)
        {
            THIS(last).dec();
            iter_swap(first, last, op);
            THIS(first).inc();
        }
    } else {
        while (true)
        {
            if (THIS(first).equal(VA(last)))
                return;
            THIS(last).dec();
            if (THIS(first).equal(VA(last)))
                return;
            iter_swap(first, last, op);
            THIS(first).inc();
        }
    }
    delete(first);
    delete(last);
}

void reverse(Iterator _first, Iterator _last, ...)
{
    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    __reverse(_first, _last, op);
}

Iterator reverse_copy(Iterator _first, Iterator _last, Iterator _result, ...)
{
    va_list ap;
    va_start(ap, _result);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    Form_t f1 = THIS(last).type();
    Form_t f2 = THIS(result).type();
    while (!THIS(_first).equal(VA(last)))
    {
        THIS(last).dec();
        AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                  FORM_WITH_OBJ(f1, THIS(last).derefer()), op);
        THIS(result).inc();
    }
    delete(last);
    return result;
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

static void __rorate_cycle(Iterator _first, Iterator _last, Iterator initial, size_t shift, FormWO_t op)
{
    Form_t f = THIS(_first).type();
    char mem[f.f == OBJ ? classSz(f.class) : f.size];
    FormWO_t val;
    if (f.f == OBJ)
        val = FORM_WITH_OBJ(f, construct(f, mem, VAEND));
    else
        val = FORM_WITH_OBJ(f, mem);
    AssignOpt(val, FORM_WITH_OBJ(f, THIS(initial).derefer()), op);
    Iterator ptr1 = THIS(initial).ctor(NULL, VA(initial), VAEND);
    Iterator ptr2 = THIS(ptr1).add(VA(shift));
    while (!THIS(ptr2).equal(VA(initial)))
    {
        AssignOpt(FORM_WITH_OBJ(f, THIS(ptr1).derefer()),
                  FORM_WITH_OBJ(f, THIS(ptr2).derefer()), op);
        THIS(ptr1).assign(VA(ptr2));
        if (distance(ptr2, _last) > shift)
            THIS(ptr2).self_add(VA(shift));
        else
            ptr2 = THIS(_first).add(VA(shift - distance(ptr2, _last)));
    }
    AssignOpt(FORM_WITH_OBJ(f, THIS(ptr1).derefer()), val, op);
    if (f.f == OBJ)
        destroy(val.mem);
}

static void __rorate(Iterator _first, Iterator _middle, Iterator _last, FormWO_t op)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator middle = THIS(_middle).ctor(NULL, VA(_middle), VAEND);
    Iterator i = THIS(_middle).ctor(NULL, VA(_middle), VAEND);
    if (first->rank == ForwardIter) {
        while (true)
        {
            iter_swap(first, i, op);
            THIS(first).inc();
            THIS(i).inc();
            if (THIS(first).equal(VA(middle))) {
                if (THIS(i).equal(VA(_last))) return;
                THIS(middle).assign(VA(i));
            } else if (THIS(i).equal(VA(_last)))
                THIS(i).assign(VA(middle));
        }
    } else if (first->rank == BidirectionalIter) {
        reverse(first, middle, op);
        reverse(middle, _last, op);
        reverse(first, _last, op);
    } else {
        size_t n = __gcd(distance(first, _last), distance(first, middle));
        while (n--)
            __rorate_cycle(first, _last, THIS(first).add(VA(n)), distance(first, middle), op);
    }
    delete(first);
    delete(i);
    delete(middle);
}

void rotate(Iterator _first, Iterator _middle, Iterator _last, ...)
{
    if (THIS(_first).equal(VA(_middle)) || THIS(_middle).equal(VA(_last))) return;

    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    __rorate(_first, _middle, _last, op);
}

Iterator rotate_copy(Iterator _first, Iterator _middle, Iterator _last, Iterator _result)
{
    return copy(_first, _middle, copy(_middle, _last, _result));
}

static Iterator __search(Iterator _first1, Iterator _last1,
                  Iterator _first2, Iterator _last2, FormWO_t op)
{
    size_t d1 = distance(_first1, _last1);
    size_t d2 = distance(_first2, _last2);
    void *mem1 = ARP_MallocARelDtor(sizeOf(_last1), destroy);
    void *mem2 = ARP_MallocARelDtor(sizeOf(_first1), destroy);
    Iterator last1 = THIS(_last1).ctor(mem1, VA(_last1), VAEND);
    Iterator first1 = THIS(_first1).ctor(mem2, VA(_first1), VAEND);
    if (d1 < d2) return last1;

    Iterator current1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator current2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f = THIS(_first1).type();

    while (!THIS(current2).equal(VA(_last2)))
        if (EqualOpt(FORM_WITH_OBJ(f, THIS(current1).derefer()),
                      FORM_WITH_OBJ(f, THIS(current2).derefer()), op)) {
            THIS(current1).inc();
            THIS(current2).inc();
        } else {
            if (d1 == d2)
                return last1;
            else {
                THIS(first1).inc();
                THIS(current1).assign(VA(first1));
                THIS(current2).assign(VA(_first2));
                --d1;
            }
        }
    return first1;
}

Iterator search(Iterator _first1, Iterator _last1,
                Iterator _first2, Iterator _last2, ...)
{
    va_list ap;
    va_start(ap, _last2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    return __search(_first1, _last1, _first2, _last2, op);
}

Iterator search_n(Iterator _first, Iterator _last, long long count, FormWO_t val, ...)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    void *mem =ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);
    Iterator i = THIS(first).ctor(NULL, VA(first), VAEND);
    Form_t f = THIS(i).type();
    if (count <= 0)
        return first;
    else {
        first = find(first, _last, val, op);
        while (!THIS(first).equal(VA(_last)))
        {
            long long n = count - 1;
            THIS(i).assign(VA(first));
            THIS(i).inc();
            while (!THIS(i).equal(VA(_last)) && n != 0 && EqualOpt(FORM_WITH_OBJ(f, THIS(i).derefer()),
                                                                   val, op))
            {
                THIS(i).inc();
                --n;
            }
            if (!n)
                break;
            else
                first = find(i, _last, val, op);
        }
        delete(i);
        return first;
    }
}

Iterator swap_ranges(Iterator _first1, Iterator _last1, Iterator _first2, ...)
{
    va_list ap;
    va_start(ap, _first2);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    void *mem =ARP_MallocARelDtor(sizeOf(_first2), destroy);
    Iterator first2 = THIS(_first2).ctor(mem, VA(_first2), VAEND);
    for (; !THIS(first1).equal(VA(_last1)); THIS(first1).inc(), THIS(first2).inc())
        iter_swap(first1, first2, op);
    delete(first1);
    return first2;
}

Iterator transform(Iterator _first, Iterator _last, Iterator _result, UnaryOperation unary_op, ...)
{
    va_list ap;
    va_start(ap, unary_op);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    void *mem =ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);
    Form_t f1 = THIS(_first).type();
    Form_t f2 = THIS(result).type();
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc(), THIS(result).inc())
        AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()),
                  unary_op(FORM_WITH_OBJ(f1, THIS(first).derefer())), op);
    delete(first);
    return result;
}

Iterator unique(Iterator _first, Iterator _last, ...)
{
    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _last);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = adjacent_find(_first, _last, op[Equal]);
    return unique_copy(first, _last, first, op[Assign], op[Equal]);
}

static Iterator __unique_copy(Iterator _first, Iterator _last, Iterator _result, FormWO_t assign, FormWO_t equal)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
    Iterator result = THIS(_result).ctor(mem, VA(_result), VAEND);

    Form_t f1 = THIS(_first).type();
    Form_t f2 = THIS(_result).type();
    FormWO_t val = FORM_WITH_OBJ(f1, THIS(first).derefer());
    AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()), val, assign);
    for (THIS(first).inc(); !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        val = FORM_WITH_OBJ(f1, THIS(first).derefer());
        if (!EqualOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()), val, equal)) {
            THIS(result).inc();
            AssignOpt(FORM_WITH_OBJ(f2, THIS(result).derefer()), val, assign);
        }
    }
    THIS(result).inc();
    delete(first);
    return result;
}

Iterator unique_copy(Iterator _first, Iterator _last, Iterator _result, ...)
{
    if (THIS(_first).equal(VA(_last))) {
        void *mem = ARP_MallocARelDtor(sizeOf(_result), destroy);
        return THIS(_result).ctor(mem, VA(_result), VAEND);
    }

    enum {Assign, Equal};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    return __unique_copy(_first, _last, _result, op[Assign], op[Equal]);
}

static Iterator __lower_bound(Iterator _first, Iterator _last, FormWO_t val, FormWO_t op)
{
    long long len = distance(_first, _last);
    long long half;
    Iterator middle = THIS(_first).ctor(NULL, VA(_first), VAEND);
    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);

    Form_t f = THIS(first).type();
    while (len > 0)
    {
        half = len >> 1;
        THIS(middle).assign(VA(first));
        advance(middle, half);
        FormWO_t v = FORM_WITH_OBJ(f, THIS(middle).derefer());
        if (CompareOpt(v, val, op) < 0) {
            THIS(first).assign(VA(middle));
            THIS(first).inc();
            len = len - half - 1;
        } else
            len = half;
    }
    delete(middle);
    return first;
}

Iterator lower_bound(Iterator _first, Iterator _last, FormWO_t val, ...)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    return __lower_bound(_first, _last, val, op);
}

static Iterator __upper_bound(Iterator _first, Iterator _last, FormWO_t val, FormWO_t op)
{
    size_t len = distance(_first, _last);
    size_t half;
    Iterator middle = THIS(_first).ctor(NULL, VA(_first), VAEND);
    void *mem = ARP_MallocARelDtor(sizeOf(_first), destroy);
    Iterator first = THIS(_first).ctor(mem, VA(_first), VAEND);

    Form_t f = THIS(first).type();
    while (len > 0)
    {
        half = len >> 1;
        THIS(middle).assign(VA(first));
        advance(middle, half);
        FormWO_t v = FORM_WITH_OBJ(f, THIS(middle).derefer());
        if (CompareOpt(v, val, op) < 0)
            len = half;
        else {
            THIS(first).assign(VA(middle));
            THIS(first).inc();
            len = len - half - 1;
        }
    }
    delete(middle);
    return first;
}

Iterator upper_bound(Iterator _first, Iterator _last, FormWO_t val, ...)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    return __upper_bound(_first, _last, val, op);
}

bool binary_search(Iterator _first, Iterator _last, FormWO_t val, ...)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);
    Iterator i = lower_bound(_first, _last, val, op);
    return THIS(i).equal(VA(_last)) && !CompareOpt(val,
                                                   FORM_WITH_OBJ(THIS(i).type(),THIS(i).derefer()),
                                                   op);
}

bool next_permutation(Iterator _first, Iterator _last, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _last);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    if (!THIS(_first).equal(VA(_last))) return false;
    Iterator i = THIS(_first).ctor(NULL, VA(_first), VAEND);
    THIS(i).inc();
    if (THIS(i).equal(VA(_last))) {
        delete(i);
        return false;
    }
    THIS(i).assign(VA(_last));
    THIS(i).dec();
    Iterator ii = THIS(i).ctor(NULL, VA(i), VAEND);
    Iterator j = THIS(i).ctor(NULL, VA(i), VAEND);

    Form_t f = THIS(_first).type();
    for (;;) {
        THIS(ii).assign(VA(i));
        THIS(i).dec();
        FormWO_t i_v = FORM_WITH_OBJ(f, THIS(i).derefer());
        FormWO_t ii_v = FORM_WITH_OBJ(f, THIS(ii).derefer());
        if (CompareOpt(i_v, ii_v, op[Compare]) < 0) {
            THIS(j).assign(VA(_last));
            THIS(j).dec();
            FormWO_t j_v = FORM_WITH_OBJ(f, THIS(j).derefer());
            while (CompareOpt(i_v, j_v, op[Compare]) < 0)
            {
                THIS(j).dec();
                j_v = FORM_WITH_OBJ(f, THIS(j).derefer());
            }
            iter_swap(i, j, op[Assign]);
            reverse(ii, _last, op[Assign]);
            delete(i);
            delete(ii);
            delete(j);
            return true;
        }
        if (THIS(i).equal(VA(_first))) {
            reverse(_first, _last, op[Assign]);
            delete(i);
            delete(ii);
            delete(j);
            return false;
        }
    }
}

bool prev_permutation(Iterator _first, Iterator _last, ...)
{
    enum {Assign, Compare};
    FormWO_t op[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _last);
    for (int i = 0; i < ARR_LEN(op); i++)
        op[i] = va_arg(ap, FormWO_t);
    va_end(ap);

    assert(_first->rank >= BidirectionalIter && _last->rank >= BidirectionalIter);
    if (!THIS(_first).equal(VA(_last))) return false;
    Iterator i = THIS(_first).ctor(NULL, VA(_first), VAEND);
    THIS(i).inc();
    if (THIS(i).equal(VA(_last))) {
        delete(i);
        return false;
    }
    THIS(i).assign(VA(_last));
    THIS(i).dec();
    Iterator ii = THIS(i).ctor(NULL, VA(i), VAEND);
    Iterator j = THIS(i).ctor(NULL, VA(i), VAEND);

    Form_t f = THIS(_first).type();
    for (;;) {
        THIS(ii).assign(VA(i));
        THIS(i).dec();
        FormWO_t i_v = FORM_WITH_OBJ(f, THIS(i).derefer());
        FormWO_t ii_v = FORM_WITH_OBJ(f, THIS(ii).derefer());
        if (CompareOpt(i_v, ii_v, op[Compare]) > 0) {
            THIS(j).assign(VA(_last));
            THIS(j).dec();
            FormWO_t j_v = FORM_WITH_OBJ(f, THIS(j).derefer());
            while (CompareOpt(i_v, j_v, op[Compare]) > 0)
            {
                THIS(j).dec();
                j_v = FORM_WITH_OBJ(f, THIS(j).derefer());
            }
            iter_swap(i, j, op[Assign]);
            reverse(ii, _last, op[Assign]);
            delete(i);
            delete(ii);
            delete(j);
            return true;
        }
        if (THIS(i).equal(VA(_first))) {
            reverse(_first, _last, op[Assign]);
            delete(i);
            delete(ii);
            delete(j);
            return false;
        }
    }
}

void random_shuffle(Iterator _first, Iterator _last, unsigned int *seed, ...)
{
    va_list ap;
    va_start(ap, seed);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    assert(_first->rank >= RandomAccessIter && _last->rank >= RandomAccessIter);
    if (THIS(_first).equal(VA(_last))) return;
    if (!seed) {
        for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(VA(_last)); THIS(i).inc()) {
            size_t n = distance(_first, i) + 1;
            iter_swap(i, THIS(_first).add(VA(rand() % n)), op);
        }
    } else {
        for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(VA(_last)); THIS(i).inc()) {
            size_t n = distance(_first, i) + 1;
            iter_swap(i, THIS(_first).add(VA(__rand_s(seed) % n)), op);
        }
    }
}

void partial_sort(Iterator _first, Iterator _middle, Iterator _last, Compare cmp)
{
    assert(_first->rank >= RandomAccessIter && _middle->rank >= RandomAccessIter && _last->rank >= RandomAccessIter);
    make_heap(_first, _middle, cmp);
    Iterator i = THIS(_middle).ctor(NULL, VA(_middle), VAEND);
    Form_t f = THIS(i).type();
    for (; !THIS(i).equal(VA(_last)); THIS(i).inc()) {
        FormWO_t i_v = FORM_WITH_OBJ(f, THIS(i).derefer());
        FormWO_t first_v = FORM_WITH_OBJ(f, THIS(_first).derefer());
        if (cmp(i_v, first_v) < 0) {
            iter_swap(_first, i);
            pop_heap(_first, _middle, cmp);
            push_heap(_first, _middle, cmp);
        }
    }
    delete(i);
    sort_heap(_first, _middle, cmp);
}

void partial_sort_copy(Iterator _first, Iterator _last,
                       Iterator _res_first, Iterator _res_last, Compare cmp, ...)
{
    assert(_res_first->rank >= RandomAccessIter && _res_last->rank >= RandomAccessIter);
    va_list ap;
    va_start(ap, cmp);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator res_real_last = THIS(_res_first).ctor(NULL, VA(_res_first), VAEND);
    Form_t f1 = THIS(first).type();
    Form_t f2 = THIS(res_real_last).type();
    while (!THIS(first).equal(VA(_last)) && !THIS(res_real_last).equal(VA(_res_first)))
    {
        AssignOpt(FORM_WITH_OBJ(f2, THIS(res_real_last).derefer()),
                  FORM_WITH_OBJ(f1, THIS(first).derefer()), op);
        THIS(res_real_last).inc();
        THIS(first).inc();
    }

    make_heap(_res_first, res_real_last, cmp);

    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        FormWO_t first_v = FORM_WITH_OBJ(f1, THIS(first).derefer());
        FormWO_t res_v = FORM_WITH_OBJ(f2, THIS(_res_first).derefer());
        if (cmp(first_v, res_v) < 0) {
            AssignOpt(res_v, first_v, op);
            pop_heap(_res_first, res_real_last, cmp);
            push_heap(_res_first, res_real_last, cmp);
        }
    }
    sort_heap(_res_first, res_real_last, cmp);
    delete(first);
    delete(res_real_last);
}

//sort
const int __sort_threshold = 16;

static void __unguarded_linear_insert(Iterator _last, FormWO_t val, FormWO_t op)
{
    Iterator last = THIS(_last).sub(VA(0)); //copy出一个迭代器
    Iterator next = THIS(_last).sub(VA(1));

    Form_t f = val._;
    FormWO_t v_n = FORM_WITH_OBJ(f, THIS(next).derefer());
    while (CompareOpt(val, v_n, op) < 0)
    {
        AssignOpt(FORM_WITH_OBJ(f, THIS(last).derefer()), v_n, VAEND);
        THIS(last).dec();
        THIS(next).dec();
        v_n.mem = THIS(next).derefer();
    }
    AssignOpt(FORM_WITH_OBJ(f, THIS(last).derefer()), val, VAEND);
}

static void __linear_insert(Iterator _first, Iterator _last, FormWO_t op)
{
    Form_t f = THIS(_first).type();
    char mem[f.f == OBJ ? classSz(f.class) : f.size];
    FormWO_t val = FORM_WITH_OBJ(f, mem);
    if (f.f == OBJ)
        val.mem = construct(f, mem, VAEND);
    AssignOpt(val, FORM_WITH_OBJ(f, THIS(_last).derefer()), VAEND);
    if (CompareOpt(FORM_WITH_OBJ(f, THIS(_first).derefer()), val, op) > 0) {
        copy_backward(_first, _last, THIS(_last).add(VA(1)));
        AssignOpt(FORM_WITH_OBJ(f, THIS(_first).derefer()), val, VAEND);
    } else
        __unguarded_linear_insert(_last, val, op);
}

static void __insertion_sort(Iterator _first, Iterator _last, FormWO_t op)
{
    if (THIS(_first).equal(VA(_last))) return;
    for (Iterator i = THIS(_first).add(VA(1)); !THIS(i).equal(VA(_last)); THIS(i).inc())
        __linear_insert(_first, i, op);
}

static inline FormWO_t __median(FormWO_t a, FormWO_t b, FormWO_t c, FormWO_t op)
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

static Iterator __unguarded_partition(Iterator _first, Iterator _last, FormWO_t pivot, FormWO_t op)
{
    Iterator first = THIS(_first).add(VA(0));
    Iterator last = THIS(_last).add(VA(0));
    Form_t f = pivot._;
    while (true)
    {
        while (CompareOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), pivot, op) < 0) THIS(first).inc();
        THIS(last).dec();
        while (CompareOpt(FORM_WITH_OBJ(f, THIS(last).derefer()), pivot, op) < 0) THIS(last).dec();
        if (THIS(first).cmp(VA(last)) >= 0) return first;
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

static void __introsort_loop(Iterator _first, Iterator _last, size_t depth_limit, FormWO_t op)
{
    long long dis = distance(_first, _last);
    Form_t f = THIS(_first).type();
    while (dis > __sort_threshold)
    {
        if (depth_limit == 0) {
            partial_sort(_first, _last, _last, op.mem); //这个cmp有点混乱
            return;
        }
        --depth_limit;
        Iterator m = THIS(_first).add(VA(dis / 2));
        FormWO_t pivot = __median(FORM_WITH_OBJ(f, THIS(_first).derefer()),
                                  FORM_WITH_OBJ(f, THIS(m).derefer()),
                                  FORM_WITH_OBJ(f, THIS(_last).derefer()), op);
        Iterator cut = __unguarded_partition(_first, _last, pivot, op);
        __introsort_loop(cut, _last, depth_limit, op);
        _last = cut; //直接得到引用就行
    }
}

static void __unguarded_insertion_sort(Iterator _first, Iterator _last, FormWO_t op)
{
    Form_t f = THIS(_first).type();
    for (Iterator i = THIS(_first).add(VA(0)); !THIS(i).equal(VA(_last)); THIS(i).inc())
        __unguarded_linear_insert(i, FORM_WITH_OBJ(f, THIS(i).derefer()), op);
}

static void __final_insertion_sort(Iterator _first, Iterator _last, FormWO_t op)
{
    long long dis = distance(_first, _last);
    if (dis > __sort_threshold) {
        Iterator m = THIS(_first).add(VA(__sort_threshold));
        __insertion_sort(_first, m, op);
        __unguarded_insertion_sort(m, _last, op);
    } else
        __insertion_sort(_first, _last, op);
}

inline void sort(Iterator _first, Iterator _last, ...)
{
    va_list ap;
    va_start(ap, _last);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    if (!THIS(_first).equal(VA(_last))) {
        size_t n = distance(_first, _last);
        __introsort_loop(_first, _last, __lg(n) * 2, op);
        __final_insertion_sort(_first, _last, op);
    }
}
