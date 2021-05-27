#ifndef TCTL_ANY_H
#define TCTL_ANY_H

#include "tctl_metaclass.h"
//POD必须是0，服务于VA宏的ANYONE参数
enum TypeFlag {POD, OBJ, FUNC};

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

//Any不可以被继承
const void *_Any(void);
#define ANY Any : _Any()
#endif //TCTL_ANY_H
