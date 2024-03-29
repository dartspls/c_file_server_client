#include "common.h"
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#define FILE_PATH_LEN 256

/* globals */
int server_fd;

/* capture SIGINT and close server socket */
void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nShutting down...\n");
        close(server_fd);
        exit(0);
    }
}

/* thread function to handle a transmission to a client */
void *handle_connection(void *p_client)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int client_socket = *((int *)p_client);
    char actualpath[FILE_PATH_LEN];

    free(p_client);

    /* check that data was received. If read returns -1 then we got nothing from the client */
    check(bytes_read = read(client_socket, buffer, FILE_PATH_LEN), "recv error");
    buffer[bytes_read] = 0; // explicit null termination

    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    if (realpath(buffer, actualpath) == NULL)
    {
        printf("Error bad path: %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    /* open file */
    FILE *fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("Error opening file %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    /* read data and write to socket */
    while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0)
    {
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    fclose(fp);
    printf("closing connection with client\n");
    return NULL;
}

/* initialize server socket */
void init_server(int port)
{
    struct sockaddr_in server_addr;
    /* create a socket */
    check((server_fd = socket(AF_INET, SOCK_STREAM, 0)), "Failed to obtain socket file descriptor");
    /* set up some socket address stuff */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    /* bind and listen*/
    check(bind(server_fd, (SA *)&server_addr, sizeof(server_addr)), "Binding failed");
    check(listen(server_fd, 1), "Listening failed");
    printf("Server initialized and listening on port %d\n", port);
}

int main(int argc, char **argv)
{
    int client_socket, port, addr_len;
    struct sockaddr_in client_addr;
    addr_len = sizeof(struct sockaddr_in);

    /* for cleaning up bound port on exit */
    signal(SIGINT, handle_signal);

    /* check cmd line args */
    if (argc != 2)
    {
        printf("usage: %s <port>\n", *argv);
        exit(1);
    }

    check(port = parse_port(argv[1]), "Failed to parse port, Must be int 0 < n < 65536");

    init_server(port);

    /* wait and accept connections */
    for (;;)
    {
        pthread_t thrd;
        check(client_socket = accept(server_fd, (SA *)&client_addr, (socklen_t *)&addr_len), "Accept failed");
        int *p_client = malloc(sizeof(int));
        *p_client = client_socket;
        pthread_create(&thrd, NULL, handle_connection, p_client);
    }
}
