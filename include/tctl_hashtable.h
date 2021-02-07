//
// Created by xjs on 2020/12/14.
//

#ifndef TINY_CTL_TCTL_HASHTABLE_H
#define TINY_CTL_TCTL_HASHTABLE_H
#include "tctl_def.h"
#include "tctl_iterator.h"

#define HASHTABLE_FUNC                   \
struct {                                 \
    CLASS_FUNC;                          \
    Iterator (*begin)(void);             \
    Iterator (*end)(void);               \
    size_t (*size)(void);                \
    bool (*empty)(void);                 \
    Iterator (*insert_unique)(FormWO_t); \
    Iterator (*insert_equal)(FormWO_t);  \
    void (*erase)(Iterator);             \
    Iterator (*find)(FormWO_t);          \
    size_t (*count)(FormWO_t);           \
    size_t (*bucket_count)(void);        \
    size_t (*max_bucket_count)(void);    \
    void (*resize)(size_t);              \
    void (*copy_from)(const Hashtable);  \
    void (*clear)(void);                 \
    void (*swap)(Hashtable h);           \
}

typedef struct _Hashtable *Hashtable;
struct _Hashtable {
    METAOBJECT_HEAD(HASHTABLE_FUNC);
};

void initHashtable(void) __attribute__((constructor));
Form_t _Hashtable(void);
#define HASHTABLE Hashtable : _Hashtable()
#endif //TINY_CTL_TCTL_HASHTABLE_H
