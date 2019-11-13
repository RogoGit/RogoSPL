#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "server_info_struct.h"

info_t* server_info;

int main() {
	
   key_t key = ftok("shared_memory_key", PROJ);
		 
    errno = 0;
    int memory = shmget(key, sizeof(info_t), SHM_RDONLY);
    if (memory < 0) {
        fprintf(stderr,"Error in shmget \n");
        return 1;
    }
    if ((server_info = shmat(memory, NULL,0)) == NULL) {
        fprintf(stderr,"Error in shmat \n");
        return 1;
    }

    printf("PID: %li, GID: %li, UID: %li\n", server_info->pid, server_info->gid, server_info->uid);
    printf("Server working %li seconds\n ", server_info->run_time);
    printf("Average load system time for 1 minute: %.3lf,\n\t 5 minutes: %.3lf,\n\t 15 minutes: %.3lf\n", 
		   server_info->load_avg[0], server_info->load_avg[1], server_info->load_avg[2]);
    return 0;
}
