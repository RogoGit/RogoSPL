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
	munmap(server_info, sizeof(info_t));
	close(fd);
	remove(MMAP_FILE);
	exit(1);
}

int main() {

	signal(SIGINT,server_shutdown);
	
	errno = 0;
	if ((fd = open(MMAP_FILE, O_RDWR | O_CREAT, PERMISSION)) < 0) {
        fprintf(stderr,"Cannot open mmap file\n");
        return 1;
   }

    if (truncate(MMAP_FILE, sizeof(info_t)) < 0) {
        fprintf(stderr,"Unable to truncate\n");
        return 1;
    }
	
    if ((server_info = (info_t*)mmap(NULL, sizeof(info_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0)) == MAP_FAILED) { 
        fprintf(stderr,"Error in mmap\n");
        return 1;
	}

	printf("Using memory mapped file. Server is running...\n");
	
    server_init(server_info);

    while(1) {
        update_info(server_info);
        sleep(1);
    }
	
    return 0;
}