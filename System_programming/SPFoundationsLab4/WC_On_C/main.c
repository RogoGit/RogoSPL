#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000000

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
    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\n' || str[i]== '\r' || str[i] == '\t') {
            i++;
        } else {
            break;
        }
    }
    for(; str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\n' || str[i]== '\r' || str[i] == '\t' ) {
            words+=1;
            while (str[i]==' ' || str[i] == '\n' || str[i]== '\r' || str[i] == '\t') {
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

int* handle_file(const char *filename) {

    int* file_data = calloc(4, sizeof(int));
    ssize_t read_bytes;
    char buffer[BUFFER_SIZE+1];

    int fd = open(filename,O_RDONLY);

    while ((read_bytes = read (fd, buffer, BUFFER_SIZE)) > 0) {
        buffer[read_bytes] = 0; // null terminator
    }

    file_data[0] = lines_count(buffer); //lines
    file_data[1] = words_count(buffer); //words
    file_data[2] = (int)bytes_count(filename); //bytes
    file_data[3] = (int)strlen(buffer); //chars

    return file_data;
}

int* handle_stdin() {

    int* file_data = calloc(4, sizeof(int));

    char inp[BUFFER_SIZE+1];
    char buffer[BUFFER_SIZE+1];

    while(scanf("%s", inp) != EOF) {
        strcat(buffer,inp);
        strcat(buffer, "\n");
    };

    file_data[0] = lines_count(buffer); //lines
    file_data[1] = words_count(buffer); //words
    file_data[2] = (int) strlen(buffer); //bytes
    file_data[3] = (int) strlen(buffer); //chars

    return file_data;

}

int check_file(const char *filename, int show) {

    errno = 0;
    int fd = open(filename,O_RDONLY);

    if (errno == EACCES) {
        if (show) fprintf(stderr, "%s - permission denied\n", filename);
        return 0;
    } else if (errno == ENOENT) {
        if (show) fprintf(stderr, "%s - no such file found\n", filename);
        return 0;
    } else if (errno) {
        if (show) fprintf(stderr, "%s - file opening error\n", filename);
        return 0;
    }

    char buffer[1];
    if (read(fd, buffer, 1) < 0) {
        if (show) fprintf(stderr, "%s - cannot read file\n", filename);
        return 0;
    }

    return 1;
}

int starts_with(const char *one, const char *another) {
    if (strncmp(one,another,strlen(another)) == 0) return 1;
    return 0;
}

int main(int argc, char *argv[]) {

    int lines_flag = 0; int words_flag = 0;
    int bytes_flag = 0; int chars_flag = 0;

    int files_count = 0;
    int files_iter = -1;

    if (argc < 2) {
        files_count = 1;
    } else {

        for (int i = 1; i < (argc); i++) {
            if ((!starts_with(argv[i], "-") && check_file(argv[i], 1)) ||
                (starts_with(argv[i], "-") && strlen(argv[i]) == 1)) {
                files_count++;
            }
        }
    }

    int* files_info[files_count]; //array with given files info
    char* file_names[files_count];

    if (argc < 2) {
        files_info[0] = handle_stdin();
        file_names[0] = "-";
        
    } else {

        for (int i = 1; i < (argc); i++) {
            if (starts_with(argv[i], "-") && (strlen(argv[i]) > 1)) {
                for (unsigned int ch = 1; ch < strlen(argv[i]); ch++) {
                    switch (argv[i][ch]) {
                        case 'l':
                            lines_flag++;
                            break;
                        case 'w':
                            words_flag++;
                            break;
                        case 'c':
                            bytes_flag++;
                            break;
                        case 'm':
                            chars_flag++;
                            break;
                        default:
                            fprintf(stderr, "%c - illegal option!\n", argv[i][ch]);
                            exit(EXIT_FAILURE);
                    }
                }
            } else {
                if (starts_with(argv[i], "-")) {
                    files_iter++;
                    files_info[files_iter] = handle_stdin();
                    file_names[files_iter] = "-";
                } else {
                    if (check_file(argv[i], 0)) {
                        files_iter++;
                        files_info[files_iter] = handle_file(argv[i]);
                        file_names[files_iter] = argv[i];
                    }
                }
            }
        }
    }

    // constructing output

    int lines_sum = 0; int words_sum = 0;
    int bytes_sum = 0; int chars_sum = 0;

    for (int i=0; i < files_count; i++) {
        // summing
        lines_sum += files_info[i][0]; words_sum += files_info[i][1];
        bytes_sum += files_info[i][2]; chars_sum += files_info[i][3];

        // default
        if (!lines_flag && !words_flag && !bytes_flag && !chars_flag) {
            printf("\t%d\t%d\t%d", files_info[i][0], files_info[i][1], files_info[i][2]);
        }

        if (lines_flag) printf("\t%d", files_info[i][0]);
        if (words_flag) printf("\t%d", files_info[i][1]);
        if (bytes_flag) printf("\t%d", files_info[i][2]);
        if (chars_flag) printf("\t%d", files_info[i][3]);

        printf("\t%s", file_names[i]);
        printf("\n");
    }

    // show sum
    if (files_count > 1) {
        // default
        if (!lines_flag && !words_flag && !bytes_flag && !chars_flag) {
            printf("\t%d\t%d\t%d", lines_sum, words_sum, bytes_sum);
        }

        if (lines_flag) printf("\t%d", lines_sum);
        if (words_flag) printf("\t%d", words_sum);
        if (bytes_flag) printf("\t%d", bytes_sum);
        if (chars_flag) printf("\t%d", chars_sum);
        printf("\tитого");
        printf("\n");
    }


    return 0;
}