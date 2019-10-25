#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int lines_count(const char *str){
    int lines = 0;
    int i;
    for (i=0; str[i] != '\0'; i++) {
        if (str[i] == '\n' ) {
            lines+=1;
        }
    }
    return lines;
}

int words_count(const char *str) {
    int words = 0;
    int i = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\n' || str[i]== '\r' || str[i] == '\t' ) {
            words += 1;
            while (str[i]== ' ' || str[i] == '\n' || str[i]== '\r' || str[i] == '\t') {
                i++;
            }
        }
    }
    return words;
}

long int bytes_count(const char *filename) {
    struct stat st;
    stat(filename,&st);
    return st.st_size;
}

void handle_file(const char *filename) {

    int lines;
    long int bytes;
    int chars;
    int words;

    ssize_t read_bytes;
    char buffer[BUFFER_SIZE+1];
    errno = 0;

    int fd = open(filename,O_RDONLY);

    while ((read_bytes = read (fd, buffer, BUFFER_SIZE)) > 0) {
        buffer[read_bytes] = 0; // null terminator
    }

    lines = lines_count(buffer);
    words = words_count(buffer);
    bytes = bytes_count(filename);
    chars = (int)strlen(buffer);

    printf("%d\t%d\t%ld\t%d\n",lines,words,bytes,chars);
    //printf("%s",buffer);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("You need to choose at least one file!\n");
    }

    handle_file(argv[1]);

    return 0;
}