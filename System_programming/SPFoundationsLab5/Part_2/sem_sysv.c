#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

volatile char letters_array[26] = "abcdefghijklmnopqrstuvwxyz";
int sem_id;

void sem_wait(int sem_id) {
    struct sembuf operation =
            { .sem_num = 0,
              .sem_op = -1,
              .sem_flg = 0 };
    if (semop(sem_id, &operation, 1) < 0) perror("sem_wait");
}

void sem_post(int sem_id) {
    struct sembuf operation =
            { .sem_num = 0,
              .sem_op = 1,
              .sem_flg = 0 };
    if (semop(sem_id, &operation, 1) < 0) perror("sem_post");
}

void* change_case() {
    while (1) {
        sem_wait(sem_id);
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
        sem_post(sem_id);
        sleep(1);
    }
}

void* reverse() {
    while (1) {
        sem_wait(sem_id);
        unsigned short i = 0;
        char temp_ch;
        for (i = 0; i < 13; i++) {
            temp_ch = letters_array[i];
            letters_array[i] = letters_array[25 - i];
            letters_array[25 - i] = temp_ch;
        }
        sleep(1);
        printf("Current array: %s\n", letters_array);
        sem_post(sem_id);
        sleep(1);
    }
}

int main () {

    pthread_t thread1;
    pthread_t thread2;

    if ((sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600)) < 0) {
        perror("semget");
        return 1;
    }

    semctl(sem_id, 0, SETVAL, 1);

    if (pthread_create(&thread1, NULL, change_case, NULL) != 0 ||
    pthread_create(&thread2, NULL, reverse, NULL) != 0) {
        fprintf(stderr, "Cannot create threads");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    semctl(sem_id, 0, IPC_RMID);

    return 0;
}