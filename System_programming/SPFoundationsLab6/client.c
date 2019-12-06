#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <zconf.h>

#define BUFSIZE 4096

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s host port [paths...]\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        fprintf(stderr, "Socket creation failed...\n");
        exit(0);
    }
    else printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8010);
    unsigned int saddr_size = sizeof(struct sockaddr_in);

    if (connect(sockfd, (struct sockaddr*) &servaddr, saddr_size) != 0) {
        fprintf(stderr, "Connection with the server failed...\n");
        exit(0);
    } else printf("Connected to the server..\n\n");

    // process request
    //errno = 0;
    //int dirs = argc-3;
    //write(sockfd, dirs, sizeof(int));

    for (int i = 1; i < argc; ++i) {
        if (write(sockfd, argv[i], strlen(argv[i])) < 0) {
            fprintf(stderr, "Cannot write to server");
        }
            write(sockfd,"\r\n",strlen("\r\n"));
    }
    //write(sockfd,"\0",strlen("\0"));

    char buff[BUFSIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(sockfd, buff, BUFSIZE)) > 0) {
        write(STDOUT_FILENO, buff, (size_t) bytes_read);
    }

    close(sockfd);

    return 0;
}