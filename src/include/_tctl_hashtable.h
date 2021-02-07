//
// Created by xjs on 2021/2/6.
//

#ifndef TINY_CTL__TCTL_HASHTABLE_H
#define TINY_CTL__TCTL_HASHTABLE_H
#include "../../include/tctl_hashtable.h"
#include "_tctl_class.h"
struct HashtableSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    Iterator (*insert_unique)(FormWO_t);
    Iterator (*insert_equal)(FormWO_t);
    void (*erase)(Iterator);
    Iterator (*find)(FormWO_t);
    size_t (*count)(FormWO_t);
    size_t (*bucket_count)(void);
    size_t (*max_bucket_count)(void);
    void (*resize)(size_t);
    void (*copy_from)(const Hashtable);
    void (*clear)(void);
    void (*swap)(Hashtable h);
};
Form_t _HashtableClass(void);
extern const struct HashtableSelector *_HashtableS;
#endif //TINY_CTL__TCTL_HASHTABLE_H
