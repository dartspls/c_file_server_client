#include "hashing.h"

/* calculate a checksum for data in inp */
unsigned int checksum(char *inp, int length)
{
    unsigned int a, b, r, r1, r2;
    a = b = r = r1 = r2 = 0;
    for (int i = 0; i < length; i++)
    {
        a += inp[i];
        b += (length - i) * inp[i];
    }

    r1 = a % MOD16;
    r2 = b % MOD16;
    return r1 + (r2 << 16);
}

/* given a checksum r, an outgoing character and an incoming character, calculate new checksum.*/
/* e.g with checksum 'hello' we can calculate csum for 'elloz' by supplying the csum for 'hello', the outgoing char 'h' and incoming char 'z'.*/
unsigned int rolling_csum(int length, unsigned int r, char outgoing, char incoming)
{
    unsigned int r1 = r & 0xffff;
    unsigned int r2 = r >> 16;
    r1 = (r1 - outgoing + incoming) % MOD16;
    r2 = (r2 - (length * outgoing) + r1) % MOD16;
    return r1 + (r2 << 16);
}

/* create a 16 bit hash of a checksum */
unsigned short hash(unsigned int csum)
{
    return csum >> 16 + csum & 0xffff;
}

void md5sum(char *data, int length, unsigned char *digest)
{
    int digest_len = DIGEST_LENGTH;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
    EVP_DigestUpdate(ctx, data, length);
    EVP_DigestFinal_ex(ctx, digest, &digest_len);
    EVP_MD_CTX_free(ctx);
}
