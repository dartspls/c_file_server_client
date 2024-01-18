#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SA struct sockaddr /* macro to avoid typing this lots */
#define BUFSIZE 1024       /* I realise there is a BUFSIZ defined in stdio that is 8192. I'm going to keep this one anyway */

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
