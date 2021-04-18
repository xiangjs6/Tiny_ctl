
void *_valueAux(int t, ...)
{
    void *ret;
    void *class;
    va_list ap;
    float f;
    double d;
    char c;
    short s;
    int i;
    long l;
    long long ll;
    va_start(ap, t);
    switch (t)
    {
        case 'f':
            class = T(Int);
            ret = ARP_MallocARel(classSz(class));
            f = (float)va_arg(ap, double);
            construct(class, ret, f);
            break;
        case 'F':
            ret = ARP_MallocARel(sizeof(double));
            *(double*)ret = (double)va_arg(ap, double);
            break;
        case 'c':
            ret = ARP_MallocARel(sizeof(unsigned char));
            *(char*)ret = (unsigned char)va_arg(ap, int);
            break;
        case 's':
            ret = ARP_MallocARel(sizeof(unsigned short));
            *(unsigned short*)ret = (unsigned short)va_arg(ap, int);
            break;
        case 'i':
            ret = ARP_MallocARel(sizeof(unsigned int));
            *(unsigned int*)ret = (unsigned int)va_arg(ap, unsigned int);
            break;
        case 'l':
            ret = ARP_MallocARel(sizeof(unsigned long));
            *(unsigned long*)ret = (unsigned long)va_arg(ap, unsigned long);
            break;
        case 'L':
            ret = ARP_MallocARel(sizeof(unsigned long long));
            *(unsigned long long*)ret = (unsigned long long)va_arg(ap, unsigned long long);
            break;
        case 'p':
            ret = ARP_MallocARel(sizeof(void*));
            *(void**)ret = (void*)va_arg(ap, void*);
            break;
        case 't':
            ret = va_arg(ap, FormWO_t).mem;
            break;
        case 'S':
            ret = ARP_MallocARel(va_arg(ap, size_t));
            break;
        default:
            assert(0);
    }
    return ret;
}

