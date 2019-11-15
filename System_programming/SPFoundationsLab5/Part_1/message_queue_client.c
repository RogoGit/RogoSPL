#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "server_info_struct.h"

info_t* server_info;
msg_t msg;

int main () {

    key_t key = ftok("message_queue_key", PROJ);

    errno = 0;
    int message = msgget(key, 0);
    if (message < 0) {
        fprintf(stderr,"Cannot get server data\n");
        return 1;
    }

    server_info = malloc(sizeof(info_t));
    msg.mtype = MSG_SERVER;

    if (msgsnd(message, &msg, sizeof(msg_t), 0) == -1) {
        fprintf(stderr,"Error in msgsnd\n");
        return 1;
    }

    if (msgrcv(message, &msg, sizeof(msg_t), MSG_CLIENT, 0) < 0) {
        fprintf(stderr,"Error in msgrcv\n");
        return 1;
    }

    memcpy(server_info, msg.mtext, sizeof(info_t));

    printf("\nUsing message_queue. \n");

    printf("\nPID: %i,\nGID: %i,\nUID: %i\n", server_info->pid, server_info->gid, server_info->uid);
    printf("Server works: %li seconds\n", server_info->run_time);
    printf("Average load system time:\n \t1 minute - %.3lf,\n \t5 minutes - %.3lf,\n \t15 minutes - %.3lf\n",
           server_info->load_avg[0], server_info->load_avg[1], server_info->load_avg[2]);

    return 0;
}