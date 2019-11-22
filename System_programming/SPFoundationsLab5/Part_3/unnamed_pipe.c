#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER 4096

void read_and_pass (int file, int pipe_fd) {
    char in_buf[BUFFER];
    char out_buf[BUFFER / 2];
    ssize_t bytes_read;
    unsigned int i, j;
    while ((bytes_read = read(file, in_buf, BUFFER)) > 0) {
        for (i = 0, j = 0; i < bytes_read; i += 2, j++) {
            out_buf[j] = in_buf[i];
        }
        write(pipe_fd, out_buf, j);
    }
    if (close(pipe_fd) < 0) perror("close");
}

int main(int argc, char** argv) {

    int file;
    int pipe_fd[2];

    if (argc != 2) {
        fprintf(stderr, "Choose file\n");
        return 1;
    }

    if ((file = open(argv[1],O_RDONLY)) < 0) {
        fprintf(stderr, "Cannot open this file\n");
        return 1;
    }

    if (pipe(pipe_fd) < 0) {
        fprintf(stderr, "Cannot create pipe\n");
        return 1;
    }

    int pipe_read = pipe_fd[0];
    int pipe_write = pipe_fd[1];

    int wc_process = fork();
    switch (wc_process) {
        case -1:
            // error
            fprintf(stderr, "Cannot create child\n");
            return 1;
        case 0:
            // child process
            if (close(pipe_write) < 0) perror("close");
            if (dup2(pipe_read, STDIN_FILENO) == -1) perror("dup2");
			printf("Character count: \n");
            if (execlp("/usr/bin/wc", "wc", "-c", NULL) == -1) perror("execl");
			printf("\n");
            break;
        default:
            // parent process
            if (close(pipe_read) < 0) perror("close");
            read_and_pass(file,pipe_write);
            break;
    }

    return 0;
}