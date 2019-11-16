#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t semaphore;
volatile char letters_array[26] = "abcdefghijklmnopqrstuvwxyz";

void* change_case() {
    while (1) {
        if (sem_wait(&semaphore) < 0) perror("sem_wait in change_case");
        unsigned short i = 0;
        char ch;
        while (letters_array[i] != '\0') {
            ch = letters_array[i];
            if (ch >= 'A' && ch <= 'Z')
                letters_array[i] = (char) (letters_array[i] + 32);
            else if (ch >= 'a' && ch <= 'z')
                letters_array[i] = (char) (letters_array[i] - 32);
            i++;
        }
        sleep(1);
        printf("Current array: %s\n", letters_array);
        if (sem_post(&semaphore) < 0) perror("sem_post in change_case");
        sleep(1);
    }
}

void* reverse() {
    while (1) {
        if (sem_wait(&semaphore) < 0) perror("sem_wait in reverse");
        unsigned short i = 0;
        char temp_ch;
        for (i = 0; i < 13; i++) {
            temp_ch = letters_array[i];
            letters_array[i] = letters_array[25 - i];
            letters_array[25 - i] = temp_ch;
        }
        sleep(1);
        printf("Current array: %s\n", letters_array);
        if (sem_post(&semaphore) < 0) perror("sem_post in reverse");
        sleep(1);
    }
}

int main () {

    pthread_t thread1;
    pthread_t thread2;

    if (sem_init(&semaphore, 0, 1) != 0) perror("sem_init");

    if (pthread_create(&thread1, NULL, change_case, NULL) != 0 ||
    pthread_create(&thread2, NULL, reverse, NULL) != 0) {
        fprintf(stderr, "Cannot create threads");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&semaphore);

    return 0;
}