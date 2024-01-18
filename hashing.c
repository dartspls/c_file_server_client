#include "hashing.h"

/* https://github.com/WayneD/rsync/blob/master/checksum.c */
unsigned int get_checksum1(char *buf1, int length)
{
    int i;
    unsigned int s1, s2;
    signed char *buf = (signed char *)buf1;

    s1 = s2 = 0;
    for (i = 0; i < length - 4; i += 4)
    {
        s2 += 4 * (s1 + buf[i]) + 3 * buf[i + 1] + 2 * buf[i + 2] + buf[i + 3];
        s1 += (buf[i] + buf[i + 1] + buf[i + 2] + buf[i + 3]);
    }
    for (; i < length; i++)
    {
        s1 += buf[i];
        s2 += s1;
    }

    return (s1 & 0xffff) + (s2 << 16);
}

unsigned int hash(char *data, int length)
{
    unsigned int hash_val = 0;
    for (int i = 0; i < length; i++)
    {
    }
    return 0;
}