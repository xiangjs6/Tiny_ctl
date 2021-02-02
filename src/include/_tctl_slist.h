//
// Created by xjs on 2021/2/1.
//

#ifndef TINY_CTL__TCTL_SLIST_H
#define TINY_CTL__TCTL_SLIST_H
#include "../../include/tctl_slist.h"
#include "_tctl_class.h"
struct SlistSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    void* (*front)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*push_front)(FormWO_t x);
    void (*pop_front)(void);
    Iterator (*erase_after)(Iterator iter);
    Iterator (*insert_after)(Iterator iter, FormWO_t x);
    void (*reverse)(void);
    void (*clear)(void);
    void (*swap)(Slist l);
};
Form_t _SlistClass(void);
extern const struct SlistSelector *_SlistS;
#endif //TINY_CTL__TCTL_SLIST_H
