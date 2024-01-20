#pragma once
#include "hashing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <linux/limits.h>

#define SA struct sockaddr /* macro to avoid typing this lots */
#define BUFSIZE 1024       /* I realise there is a BUFSIZ defined in stdio that is 8192. I'm going to keep this one anyway */
#define BLOCK_SIZE 512
#define TRANSFER 0x1
#define SYNC 0x2

/* Error checking. I find readability improves compared to having lots of ifs - idea from Jacob Sorber's C sockets series */
int check(int exp, const char *msg)
{
    if (exp == -1)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

/* convert string to integer in range 0 < n < 65536 */
int parse_port(char *port_str)
{
    int port;
    errno = 0;
    char *p_end;
    port = strtol(port_str, &p_end, 10);

    /* little bit of error and bounds checking */
    if (port_str == p_end || (errno != 0 && port == 0) || port <= 0 || port >= 65536)
    {
        return -1;
    }
    return port;
}
