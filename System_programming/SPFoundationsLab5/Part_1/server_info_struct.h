#ifndef SERVER_INFO_STRUCT_H
#define SERVER_INFO_STRUCT_H

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PERMISSION 0644
#define PROJ 13

typedef struct info {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t start_time;
    time_t run_time;
    double load_avg[3];
} info_t;

#endif