#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "server_info_struct.h"

info_t* server_info;
int memory;

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
	shmdt(server_info);
	shmctl(memory,IPC_RMID,NULL);
	exit(1);
}

int main() {

	signal(SIGINT,server_shutdown);
	
	key_t key = ftok("shared_memory_key", PROJ);
		 
    errno = 0;
    memory = shmget(key, sizeof(info_t), IPC_CREAT | PERMISSION);
	
    if (memory < 0) {
        fprintf(stderr,"Error in shmget \n");
        return 1;
    }
    if ((server_info = shmat(memory, NULL, 0)) == NULL) {
        fprintf(stderr,"Error in shmat \n");
        return 1;
    }

	printf("Using shared memory. Server is running...\n");
	
    server_init(server_info);

    while(1) {
        update_info(server_info);
        sleep(1);
    }
	
    return 0;
}