#ifndef TCTL_PLAINDATA_H
#define TCTL_PLAINDATA_H

#include "tctl_metaclass.h"

#define PLAINDATA_FUNC    \
struct {                  \
    METACLASS_FUNC;       \
    void *value(void);    \
    char *typename(void); \
    size_t size(void);    \
}

typedef struct {
    METAOBJECT_HEAD(PLAINDATA_FUNC);
 } *PlainData;
#endif //TCTL_PLAINDATA_H
