#pragma once
#include <openssl/evp.h>

#define TABLE_SIZE 65536
#define DIGEST_LENGTH 16
#define MOD16 (1 << 16)

/* I'm not sure the best way to enforce that digest is 16 bytes long. I decided to have it as a parameter rather than create it within the function and return it. */

/* create digest for data. length is size of data. place digest in digest which must be 16 bytes in size */
void md5sum(char *data, int length, unsigned char *digest);
unsigned int checksum(char *inp, int length);
unsigned int rolling_csum(int length, unsigned int r, char outgoing, char incoming);