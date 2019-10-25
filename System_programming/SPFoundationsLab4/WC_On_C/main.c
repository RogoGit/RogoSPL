#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <zconf.h>

#define BUFFER_SIZE 4096

int lines_count(const char *str){
    int lines = 0;
    int i;
    for( i=0; str[i] != '\0'; i++) {
        if( str[i] == '\n' ) {
            lines+=1;
        }
    }
    return lines;
}

void handle_file(char *filename) {

    int lines;
    //int bytes;
    //int chars;
    //int words;

    ssize_t read_bytes;
    char buffer[BUFFER_SIZE+1];
    errno = 0;

    int fd = open(filename,O_RDONLY);

    while ((read_bytes = read (fd, buffer, BUFFER_SIZE)) > 0) {
        buffer[read_bytes] = 0; // null terminator
    }

    lines = lines_count(buffer);

    printf("%d\n",lines);
    //printf("%s",buffer);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("You need to choose at least one file!\n");
    }

    handle_file(argv[1]);

    return 0;
}