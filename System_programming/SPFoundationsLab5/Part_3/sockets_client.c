#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "server_info_struct.h"

#define SOCKET_PATH "/tmp/lab5-socket"

info_t* server_info;

int main () {

    server_info = malloc(sizeof(info_t));

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

    if (connect(sock_fd, (const struct sockaddr *) &saddr, saddr_size) < 0) {
         fprintf(stderr,"Cannot connect to server");
         return 1;
    }
    if (read(sock_fd, server_info, sizeof(info_t)) < 0) perror("read");

    printf("\nUsing sockets. \n");

    printf("\nPID: %i,\nGID: %i,\nUID: %i\n", server_info->pid, server_info->gid, server_info->uid);
    printf("Server works: %li seconds\n", server_info->run_time);
    printf("Average load system time:\n \t1 minute - %.3lf,\n \t5 minutes - %.3lf,\n \t15 minutes - %.3lf\n",
           server_info->load_avg[0], server_info->load_avg[1], server_info->load_avg[2]);

    close(sock_fd);

    return 0;
}