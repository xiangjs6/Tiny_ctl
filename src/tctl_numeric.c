//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_numeric.h"
#include "../include/auto_release_pool.h"
#include <assert.h>
#include <stdarg.h>
#define Import ITERATOR

static inline FormWO_t AddOpt(FormWO_t first, FormWO_t second, FormWO_t op)
{
    if (op._.f != FUNC) {
        assert(first._.f == OBJ);
        Object obj = first.mem;
        return (FormWO_t){first._, THIS(obj).add(second)};
    } else {
        BinaryOperation func = op.mem;
        return func(first, second);
    }
}

static inline FormWO_t SubOpt(FormWO_t first, FormWO_t second, FormWO_t op)
{
    if (op._.f != FUNC) {
        assert(first._.f == OBJ);
        Object obj = first.mem;
        return (FormWO_t){first._, THIS(obj).sub(second)};
    } else {
        BinaryOperation func = op.mem;
        return func(first, second);
    }
}

static inline FormWO_t MulOpt(FormWO_t first, FormWO_t second, FormWO_t op)
{
    if (op._.f != FUNC) {
        assert(first._.f == OBJ);
        Object obj = first.mem;
        return (FormWO_t){first._, THIS(obj).mul(second)};
    } else {
        BinaryOperation func = op.mem;
        return func(first, second);
    }
}

static inline void AssignOpt(FormWO_t left, FormWO_t right, FormWO_t op)
{
    if (op._.f != FUNC) {
        assert(left._.f == OBJ);
        Object obj = left.mem;
        THIS(obj).assign(right);
    } else {
        AssignOperation func = op.mem;
        func(left, right);
    }
}

FormWO_t accumulate(const Iterator _first, const Iterator _last, FormWO_t init, ...)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);
    Form_t f = THIS(first).type();
    assert(f.f == THIS(last).type().f);
    va_list ap;
    va_start(ap, init);
    FormWO_t t = va_arg(ap, FormWO_t);
    va_end(ap);
    for (; !THIS(first).equal(VA(last)); THIS(first).inc()) {
        FormWO_t val = FORM_WITH_OBJ(f, THIS(first).derefer());
        init = AddOpt(init, val, t);
    }
    delete(first);
    delete(last);
    return init;
}

Iterator adjacent_difference(const Iterator _first, const Iterator _last, const Iterator _result, ...)
{
    void *mem = ARP_MallocDtor(sizeOf(_result), destroy);
    Iterator res = THIS(_result).ctor(mem , VA(_result), VAEND);
    if (THIS(_first).equal(VA(_last)))
        return res;
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);

    FormWO_t assign, op;
    va_list ap;
    va_start(ap, _result);
    assign = va_arg(ap, FormWO_t);
    if (assign._.f == END)
        op = VAEND;
    else
        op = va_arg(ap, FormWO_t);
    va_end(ap);

    Form_t f1 = THIS(_result).type();
    Form_t f2 = THIS(_first).type();
    AssignOpt(FORM_WITH_OBJ(f1, THIS(res).derefer()),
              FORM_WITH_OBJ(f2, THIS(first).derefer()), assign);
    FormWO_t pre_val = FORM_WITH_OBJ(f2, THIS(first).derefer()); //相当于指向first的内容
    for(THIS(first).inc(), THIS(res).inc(); !THIS(first).equal(VA(last)); THIS(first).inc(), THIS(res).inc()) {
        FormWO_t tmp = SubOpt(FORM_WITH_OBJ(f2, THIS(first).derefer()),
               pre_val, op);
        AssignOpt(FORM_WITH_OBJ(f1, THIS(res).derefer()),
                  tmp, assign);
        pre_val = FORM_WITH_OBJ(f2, THIS(first).derefer());
    }
    delete(first);
    delete(last);
    return res;
}

FormWO_t inner_product(const Iterator _first1, const Iterator _last1, const Iterator _first2, FormWO_t init, ...)
{
    Iterator first1 = THIS(_first1).ctor(NULL, VA(_first1), VAEND);
    Iterator last1 = THIS(_last1).ctor(NULL, VA(_last1), VAEND);
    Iterator first2 = THIS(_first2).ctor(NULL, VA(_first2), VAEND);
    Form_t f1 = THIS(_first1).type();
    Form_t f2 = THIS(_first2).type();

    enum {Op1, Op2};
    FormWO_t vargs[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, init);
    for (int i = 0; i < 2; i++)
        if ((vargs[i] = va_arg(ap, FormWO_t))._.f == END)
            break;
    va_end(ap);

    for (; !THIS(first1).equal(VA(last1)); THIS(first1).inc(), THIS(first2).inc()) {
        FormWO_t tmp = MulOpt(FORM_WITH_OBJ(f1, THIS(first1).derefer()),
                              FORM_WITH_OBJ(f2, THIS(first2).derefer()), vargs[Op2]);
        init = AddOpt(init, tmp, vargs[Op1]);
    }

    delete(first1);
    delete(first2);
    delete(last1);
    return init;
}

Iterator partial_sum(const Iterator _first, const Iterator _last, const Iterator _result, ...)
{
    void *mem = ARP_MallocDtor(sizeOf(_result), destroy);
    Iterator res = THIS(_result).ctor(mem , VA(_result), VAEND);
    if (THIS(_first).equal(VA(_last)))
        return res;

    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);

    enum {Assign, Op};
    FormWO_t vargs[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, _result);
    for (int i = 0; i < 2; i++)
        if ((vargs[i] = va_arg(ap, FormWO_t))._.f == END)
            break;
    va_end(ap);

    Form_t f1 = THIS(_first).type();
    Form_t f2 = THIS(_result).type();

    AssignOpt(FORM_WITH_OBJ(f2, THIS(res).derefer()),
              FORM_WITH_OBJ(f1, THIS(first).derefer()), vargs[Assign]);
    FormWO_t pre_val = FORM_WITH_OBJ(f1, THIS(first).derefer());
    for(THIS(first).inc(), THIS(res).inc(); !THIS(first).equal(VA(last)); THIS(first).inc(), THIS(res).inc()) {
        FormWO_t tmp = AddOpt(FORM_WITH_OBJ(f2, THIS(first).derefer()),
                              pre_val, vargs[Op]);
        AssignOpt(FORM_WITH_OBJ(f1, THIS(res).derefer()),
                  tmp, vargs[Assign]);
        pre_val = FORM_WITH_OBJ(f2, THIS(first).derefer());
    }
    delete(first);
    delete(last);
    return res;
}

void iota(const Iterator _first, const Iterator _last, FormWO_t x, ...)
{
    Iterator first = THIS(_first).ctor(NULL, VA(_first), VAEND);
    Iterator last = THIS(_last).ctor(NULL, VA(_last), VAEND);

    enum {Assign, Op};
    FormWO_t vargs[2] = {VAEND, VAEND};
    va_list ap;
    va_start(ap, x);
    for (int i = 0; i < 2; i++)
        if ((vargs[i] = va_arg(ap, FormWO_t))._.f == END)
            break;
    va_end(ap);

    Form_t f = THIS(first).type();
    while (!THIS(first).equal(VA(last)))
    {
        AssignOpt(FORM_WITH_OBJ(f, THIS(first).derefer()), x, vargs[Assign]);
        AddOpt(x, VA(1), vargs[Op]);
    }
    delete(first);
    delete(last);
}
