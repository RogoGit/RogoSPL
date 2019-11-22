#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "server_info_struct.h"

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

int set_signal_action(int sig, void (*sig_handler)(int)) {
    struct sigaction action;
	sigset_t mask;
    action.sa_handler = sig_handler;
    action.sa_flags = 0;
	action.sa_mask  = mask;
    sigemptyset(&mask);
    if (sigaction(sig, &action, NULL) == 0) return 0; else return 1;
}

void show_pid() { printf("PID: %i\n", server_info->pid); }
void show_gid() { printf("GID: %i\n", server_info->gid); }
void show_uid() { printf("UID: %i\n", server_info->uid); }
void show_time() { printf("Server works: %li seconds\n", server_info->run_time); }
void show_load() { printf("Average load system time:\n \t1 minute - %.3lf,\n \t5 minutes - %.3lf,\n \t15 minutes - %.3lf\n",
                          server_info->load_avg[0], server_info->load_avg[1], server_info->load_avg[2]); }

int handle_signals() {
	if (set_signal_action(SIGHUP,  show_pid) != 0) return 1;
    if (set_signal_action(SIGINT,  show_uid) != 0) return 1;
    if (set_signal_action(SIGTERM, show_gid) != 0) return 1;
    if (set_signal_action(SIGUSR1, show_time) != 0) return 1;
    if (set_signal_action(SIGUSR2, show_load) != 0) return 1;
	return 0;
}

int main() {

    server_info = malloc(sizeof(info_t));
    server_init(server_info);
	
	if (handle_signals() != 0) { puts("Problems with setting signal handler"); return 1; }
	
    printf("Server is running...\n");

    while(1) {
        update_info(server_info);
        sleep(1);
    }

    return 0;
}
