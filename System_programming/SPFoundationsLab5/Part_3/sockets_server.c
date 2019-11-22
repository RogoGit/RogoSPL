#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include "server_info_struct.h"

#define SOCKET_PATH "/tmp/lab5-socket"

info_t* server_info;

void server_init (info_t* server_info) {
    server_info->pid = getpid();
    server_info->uid = getuid();
    server_info->gid = getgid();
    time(&server_info->start_time);
    getloadavg(server_info->load_avg,3);
}

void update_info (info_t* server_info) {
    getloadavg(server_info->load_avg,3);
    time_t time_now;
    time(&time_now);
    server_info->run_time = time_now - server_info->start_time;
}

void server_shutdown() {
    puts("Server shutdown...");
    unlink(SOCKET_PATH);
	exit(1);
}

int main() {

    signal(SIGINT, server_shutdown);

    server_info = malloc(sizeof(info_t));

    printf("Using sockets. Server is running...\n");

    int sock_fd;
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("Cannot create socket");
        return 1;
    }

    struct sockaddr_un saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sun_family = AF_UNIX;
    strncpy(saddr.sun_path, SOCKET_PATH, sizeof(saddr.sun_path)-1);
    unsigned int saddr_size = sizeof(struct sockaddr_un);

    errno = 0;
    if (bind(sock_fd, (const struct sockaddr *) &saddr, saddr_size) < 0) perror("bind");
    if (listen(sock_fd, 0) < 0) perror("listen");

	server_init(server_info);
    while(1) {
        update_info(server_info);
        errno = 0;
        int client_fd = accept(sock_fd, (struct sockaddr*) &saddr, &saddr_size);
        write(client_fd, server_info, sizeof(info_t));
        if (errno != 0) puts("Cannot send data to client");
        close(client_fd);
    }

    return 0;
}