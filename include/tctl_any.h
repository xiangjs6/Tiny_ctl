#ifndef TCTL_ANY_H
#define TCTL_ANY_H

#include "tctl_metaclass.h"
enum TypeFlag {POD, OBJ};

typedef struct {
    METAOBJECT_HEAD(
        struct {
            METACLASS_FUNC;
            void *(*value)(void);
            size_t (*size)(void);
            enum TypeFlag (*type)(void);
        }
    );
 } *Any;

Any ctor_any(void *mem, void *ptr, size_t size);
//Any不可以被继承
const void *_Any(void);
#define ANY Any : _Any()
#endif //TCTL_ANY_H
