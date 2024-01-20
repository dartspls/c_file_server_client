#include "common.h"

int setup_socket(char *ip, int port)
{
    int client_fd = 0;
    struct sockaddr_in server_address;
    check(client_fd = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    check(inet_pton(AF_INET, ip, &server_address.sin_addr), "Invalid address or address not found");

    /* connect and send request for filename */
    check(connect(client_fd, (SA *)&server_address, sizeof(server_address)), "Connection with server failed");
    return client_fd;
}

/* transfer the differences between two files */
void sync_file(char *ip, char *port_str, char *filename)
{
    int port, client_fd, bytes_read, total_bytes_read;
    char actualpath[PATH_MAX];
    char block_buffer[BLOCK_SIZE];
    unsigned int csum[1];
    char type[1];
    type[0] = SYNC;

    total_bytes_read = 0;
    port = parse_port(port_str);
    client_fd = setup_socket(ip, port);
    /* try find file */
    if (realpath(filename, actualpath) == NULL)
    {
        printf("Error bad path: %s\n", filename);
        exit(1);
    }

    /* we have file. open it */
    FILE *fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    /* communicate to server that we are syncing */
    write(client_fd, type, 1);
    /* send file name */
    write(client_fd, filename, strlen(filename));

    while ((bytes_read = fread(block_buffer, 1, BLOCK_SIZE, fp)) > 0)
    {
        /* calculate checksum */
        csum[0] = checksum(block_buffer, bytes_read);
        /* calculate fingerprint */
        unsigned char digest[DIGEST_LENGTH];
        md5sum(block_buffer, bytes_read, digest);

        /* transmit checksum */
        write(client_fd, csum, sizeof(csum));
        /* transmit fingerprint */
        write(client_fd, digest, DIGEST_LENGTH);
    }

    /* wait for data from server */
}

/* transfer a file in entirety */
void transfer_file(char *ip, char *port_str, char *filename)
{
    int client_fd, bytes_read, port, total_bytes_read;
    char buffer[BUFSIZE];
    total_bytes_read = 0;

    port = parse_port(port_str);

    /* setup client socket */
    client_fd = setup_socket(ip, port);
    send(client_fd, filename, strlen(filename), 0);

    /* open file for writing */
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error creating file %s\n", filename);
        close(client_fd);
        exit(1);
    }

    /* read from socket and write to file */
    while ((bytes_read = read(client_fd, buffer, BUFSIZE)) > 0)
    {
        total_bytes_read += bytes_read;
        fwrite(buffer, 1, bytes_read, fp);
    }
    /* clean up */
    close(client_fd);
    fclose(fp);
    printf("Transmission finished. %d bytes written to %s\n", total_bytes_read, filename);
}

int main(int argc, char **argv)
{
    int sync = 0;

    if (argc < 4)
    {
        printf("usage: %s <ip> <port> <filename> <(optional) -s>\n", *argv);
        exit(1);
    }
    if (argc == 5 && strcmp(argv[4], "-s") == 0)
    {
        sync = 1;
    }
    else
    {
        printf("invalid flag: %s\n", argv[4]);
        exit(1);
    }

    if (sync == 1)
    {
        sync_file(argv[1], argv[2], argv[3]);
    }
    else
    {
        transfer_file(argv[1], argv[2], argv[3]);
    }
}
