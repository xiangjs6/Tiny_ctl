#ifndef TCTL_ANY_H
#define TCTL_ANY_H

#include "tctl_metaclass.h"
enum TypeFlag {POD, OBJ};

typedef struct {
    METACLASS_FUNC;
    void *(*value)(void);
    size_t (*size)(void);
 } *Any;

//Any不可以被继承
const void *_Any(void);
#define ANY Any : _Any()
#endif //TCTL_ANY_H
