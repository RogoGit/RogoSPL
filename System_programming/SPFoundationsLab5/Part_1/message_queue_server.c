#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "server_info_struct.h"

info_t* server_info;
int message;

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
	puts("\nServer shutdown...");
	msgctl(message, IPC_RMID, NULL); 
	exit(1);
}

int main() {

    signal(SIGINT,server_shutdown);

    key_t key = ftok("message_queue_key", PROJ);

    errno = 0;
    message = msgget(key, IPC_CREAT | PERMISSION);
    if (message < 0) {
        fprintf(stderr,"Error. Cannot create queue. \n");
        return 1;
    }

    server_info = malloc(sizeof(info_t));
    server_init(server_info);

    printf("Using message queue. Server is running...\n");

    while(1) {
		
        msg_t msg;

        if (msgrcv(message, &msg, sizeof(msg_t), MSG_SERVER, 0) < 0) {
            fprintf(stderr,"Not recieved anything...\n");
        } else {
			update_info(server_info);
            memcpy(msg.mtext, server_info, sizeof(info_t));
            msg.mtype = MSG_CLIENT;
            if (msgsnd(message, &msg, sizeof(msg_t), 0) == -1) {
                fprintf(stderr, "Error sending the message\n");
            }
        }
        sleep(1);
    }
	
    return 0;
}