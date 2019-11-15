#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t semaphore;
char letters_array[26] = "abcdefghijklmnopqrstuvwxyz";

void* change_case() {
    unsigned short i = 0;
    char ch;
    sem_wait(&semaphore);
    while (letters_array[i] != '\0') {
        ch = letters_array[i];
        if (ch >= 'A' && ch <= 'Z')
            letters_array[i] = (char)(letters_array[i] + 32);
        else if (ch >= 'a' && ch <= 'z')
            letters_array[i] = (char)(letters_array[i] - 32);
        i++;
    }
    printf("Current array: %s\n",letters_array);
    sleep(1);
    sem_post(&semaphore);
    return NULL;
}

void* reverse() {
    unsigned short i = 0;
    char temp_ch;
    sem_wait(&semaphore);
    for (i = 0; i < 13; i++){
        temp_ch = letters_array[i];
        letters_array[i] = letters_array[25 - i];
        letters_array[25-i] = temp_ch;
    }
    printf("Current array: %s\n",letters_array);
    sleep(1);
    sem_post(&semaphore);
    return NULL;
}

int main () {

    pthread_t thread1;
    pthread_t thread2;

    sem_init(&semaphore, 0, 1);

    while (1) {
        pthread_create(&thread1, NULL, change_case, NULL);
        pthread_create(&thread2, NULL, reverse, NULL);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }
    
    sem_destroy(&semaphore);

    return 0;
}