#include "common.h"

int main(int argc, char **argv)
{
    int sock = 0, bytes_read, client_fd, port;
    struct sockaddr_in server_address;
    char buffer[BUFSIZE];
    char const *filename;

    if (argc != 4)
    {
        printf("usage: %s <ip> <port> <filename>\n", *argv);
        exit(1);
    }

    check(port = parse_port(argv[2]), "Failed to parse port, Must be int 0 < n < 65536");
    filename = argv[3];

    check(sock = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    check(inet_pton(AF_INET, argv[1], &server_address.sin_addr), "Invalid address or address not found");

    /* connect and send request for filename */
    check(client_fd = connect(sock, (SA *)&server_address, sizeof(server_address)), "Connection with server failed");
    send(sock, filename, strlen(filename), 0);

    /* open file for writing */
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error creating file %s\n", filename);
        close(sock);
        exit(EXIT_FAILURE);
    }

    /* read from socket and write to file */
    while ((bytes_read = read(sock, buffer, BUFSIZE)) > 0)
    {
        fwrite(buffer, 1, bytes_read, fp);
    }
    /* clean up */
    close(sock);
    fclose(fp);
    printf("Transmission finished. Data written to %s\n", filename);
}
