#ifndef MEMSET_H
#define MEMSET_H

static void *local_memset(void *s, int c, uint32_t n)
{
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}


#endif
