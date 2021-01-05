//
// Created by xjs on 2020/9/1.
//

#include "include/_tctl_iterator.h"
//#include "tctl_allocator.h"
//#include "../include/auto_release_pool.h"
#include <memory.h>
#include <assert.h>

static void *_derefer(void);
//init
volatile static struct IteratorSelector IteratorS = {
        {},
        _derefer
};
const struct IteratorSelector *_IteratorS = NULL;

static void *_derefer(void)
{
    void *_this = pop_this();
    const struct IteratorClass *this = classOf(_this);
    assert(this->derefer);
    return this->derefer(_this);
}

static void *_ctor(void *_this, va_list *app)
{
    struct IteratorClass *this = super_ctor(_Class(), _this, app);
    va_list ap;
    va_copy(ap, *app);
    voidf selector;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf)IteratorS.derefer) {
            *(voidf *) this->derefer = method;
            break;
        }
    }
    va_end(ap);
    return this;
}