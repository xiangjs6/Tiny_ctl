//
// Created by xjs on 2020/12/19.
//

#ifndef TINY_CTL_TCTL_UNORDERED_SET_H
#define TINY_CTL_TCTL_UNORDERED_SET_H
#include "tctl_class.h"
#include "tctl_iterator.h"
#include "tctl_def.h"

#define UNORDERED_SET_FUNC            \
struct {                              \
    CLASS_FUNC;                       \
    Iterator (*begin)(void);          \
    Iterator (*end)(void);            \
    size_t (*size)(void);             \
    bool (*empty)(void);              \
    void (*erase)(Iterator iter);     \
    Iterator (*insert)(FormWO_t x);   \
    size_t (*count)(FormWO_t x);      \
    Iterator (*find)(FormWO_t x);     \
    size_t (*bucket_count)(void);     \
    size_t (*max_bucket_count)(void); \
    void (*reserve)(size_t);          \
    void (*clear)(void);              \
    void (*swap)(Unordered_Set);      \
}

typedef struct _Unordered_Set *Unordered_Set;
struct _Unordered_Set {
    METAOBJECT_HEAD(UNORDERED_SET_FUNC);
};

Form_t _Unordered_Set(void);
#define UNORDERED_SET Unordered_Set : _Unordered_Set()
#endif //TINY_CTL_TCTL_UNORDERED_SET_H
