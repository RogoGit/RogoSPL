#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

volatile char letters_array[26] = "abcdefghijklmnopqrstuvwxyz";
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned long wait_main;
unsigned long wait_ch_case;
unsigned long wait_reverse;

void* change_case() {
    while (1) {
        usleep(wait_ch_case);
        unsigned short i = 0;
        char ch;
        if (pthread_mutex_lock(&mutex) != 0) perror("lock in change_case");
        while (letters_array[i] != '\0') {
            ch = letters_array[i];
            if (ch >= 'A' && ch <= 'Z')
                letters_array[i] = (char) (letters_array[i] + 32);
            else if (ch >= 'a' && ch <= 'z')
                letters_array[i] = (char) (letters_array[i] - 32);
            i++;
        }
        if (pthread_mutex_unlock(&mutex) != 0) perror("unlock in change_case");
    }
}

void* reverse() {
    while (1) {
        usleep(wait_reverse);
        unsigned short i = 0;
        char temp_ch;
        if (pthread_mutex_lock(&mutex) != 0) perror("lock in reverse");
        for (i = 0; i < 13; i++) {
            temp_ch = letters_array[i];
            letters_array[i] = letters_array[25 - i];
            letters_array[25 - i] = temp_ch;
        }
        if (pthread_mutex_unlock(&mutex) != 0) perror("unlock in reverse");
    }
}

unsigned long parse_ulong(const char* arg) {
	char *arg_end;
	unsigned long num; 
	if (arg[0] == '-') {
		puts("Arguments must be positive numbers");
		exit(1);
	}
	num = strtoul(arg,&arg_end,10);
	if (*arg_end != '\0') {
		puts("Arguments must be numbers");
		exit(1);
	}
	return num;
}

int main (int argc, const char* argv[]) {

    if (argc !=4) {
        puts("First argument is time for main thread, second - change_case, third - reverse");
        return 0;
    }

    wait_main = parse_ulong(argv[1]);
    wait_ch_case = parse_ulong(argv[2]);
    wait_reverse = parse_ulong(argv[3]);

    pthread_t thread1;
    pthread_t thread2;

    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&thread1, NULL, change_case, NULL) != 0 ||
    pthread_create(&thread2, NULL, reverse, NULL) != 0) {
        fprintf(stderr, "Cannot create threads");
        return 1;
    }

    while (1) {
        usleep(wait_main);
        if (pthread_mutex_lock(&mutex) != 0) perror("lock in main");
        printf("Current array: %s\n", letters_array);
        if (pthread_mutex_unlock(&mutex) != 0) perror("unlock in main");
    }

        return 0;
}