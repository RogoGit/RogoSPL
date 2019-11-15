#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include "server_info_struct.h"

info_t* server_info;
int fd;

int main () {
		 
    errno = 0;
    if ((fd = open(MMAP_FILE, O_RDWR, 0)) < 0) {
       fprintf(stderr,"Cannot get server data\n");
       return 1;
    }

    if ((server_info =(info_t*) mmap(NULL, sizeof(info_t), PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
       fprintf(stderr,"Error in mmap\n");
       return 1;
    }
	
	printf("\nUsing memory mapped file. \n");

    printf("\nPID: %i,\nGID: %i,\nUID: %i\n", server_info->pid, server_info->gid, server_info->uid);
    printf("Server works: %li seconds\n", server_info->run_time);
    printf("Average load system time:\n \t1 minute - %.3lf,\n \t5 minutes - %.3lf,\n \t15 minutes - %.3lf\n", 
		   server_info->load_avg[0], server_info->load_avg[1], server_info->load_avg[2]);
    return 0;
}


