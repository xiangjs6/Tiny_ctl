#include <stdio.h>
#include "allocator.h"
#include "tctl_object.h"

void f(int *a)
{
    printf("%d\n", *a);
}

int main(void) {
    char *s = allocate(128);
    char *s1 = allocate(45);
    char *s2 = allocate(1027);
    char *s3 = reallocate(NULL, 0, 64);
    sprintf(s3, "hello world");
    s3 = reallocate(s3, 64, 128);
    s3 = reallocate(s3, 128, 256);
    s3 = reallocate(s3, 256, 67);
    deallocate(s, 128);
    deallocate(s1, 45);
    deallocate(s2, 1027);
    THIS(NULL);
    f(TEMP((int)1));
    return 0;
}
