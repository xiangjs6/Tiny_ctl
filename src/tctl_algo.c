//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_algo.h"
#include "../include/tctl_algobase.h"
#include "../include/auto_release_pool.h"
#include <stddef.h>
#include <memory.h>
#include <stdarg.h>

#define Import ITERATOR
#define ARR_LEN(a) (sizeof(a) / sizeof(*a))

inline static int CompareOpt(FormWO_t a, FormWO_t b, FormWO_t op)
{
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

//set
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

//adjacent_find
Iterator adjacent_find(Iterator _first, Iterator _last, .../*Compare*/)
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
        int res = CompareOpt(FORM_WITH_OBJ(f, THIS(next).derefer()),
                             FORM_WITH_OBJ(f, THIS(first).derefer()), op);
        if (!res)
            break;
    }
    delete(next);
    return first;
}

//count
size_t count(Iterator _first, Iterator _last, FormWO_t val, .../*Compare*/)
{
    va_list ap;
    va_start(ap, val);
    FormWO_t op = va_arg(ap, FormWO_t);
    va_end(ap);

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Form_t f = THIS(first).type();
    size_t n = 0;
    for (; !THIS(first).equal(VA(_last)); THIS(first).inc()) {
        int res = CompareOpt(val, FORM_WITH_OBJ(f, THIS(first).derefer()), op);
        if (!res)
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
           !CompareOpt(val, FORM_WITH_OBJ(f, THIS(first).derefer()), op))
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
        Iterator r_last1 = THIS(last1).reserve_iterator();
        Iterator r_last2 = THIS(last2).reserve_iterator();
        Iterator r_result = search(r_last1, first1, r_last2, first2, op);
        result = THIS(r_result).reserve_iterator();
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
                break;
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
    result = copy(first2, _last2, (delete(result), copy(first1, _last1, result)));
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

Iterator partition(Iterator _first, Iterator _last, Predicate pred)
{
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
        iter_swap(first, last);
        THIS(first).inc();
    }
}
