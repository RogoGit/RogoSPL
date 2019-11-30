#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <zconf.h>

#define DEFAULT_SIZE 2
#define SIZE_CHANGE 1

typedef struct {
    pthread_t worker;
    int is_used;
    } server_thread;

typedef struct  {
    volatile server_thread* data;
    volatile size_t size;
    pthread_mutex_t mutex;
    } dynamic_pool;

dynamic_pool* thread_pool;

size_t create_dynamic_thread_pool (dynamic_pool* pool) {
    pool->size = DEFAULT_SIZE;
    pool->data = (server_thread*) malloc (pool->size * sizeof(server_thread));
    unsigned int i;
    for (i = 0; i < pool->size; i++) {
        pool->data[i].is_used = 0;
    }
    pthread_mutex_init(&pool->mutex,NULL);
    return pool->size;
}

size_t increment_pool (dynamic_pool* pool) {
    size_t old_size = pool->size;
    pool->size += SIZE_CHANGE;
    pool->data = (server_thread*) realloc ((void*)pool->data, pool->size * sizeof(server_thread));
    size_t i;
    for (i = old_size; i < pool->size; i++) {
        pool->data[i].is_used = 0;
    }
    return pool->size;
}

size_t decrement_pool (dynamic_pool* pool) {
    pool->size -= SIZE_CHANGE;
    pool->data = (server_thread*) realloc ((void*)pool->data, pool->size * sizeof(server_thread));
    return pool->size;
}

pthread_t get_free_thread (dynamic_pool* pool) {
    pthread_mutex_lock(&pool->mutex);
    unsigned int i;
    for (i = 0; i < pool->size; i++) {
        if (!pool->data[i].is_used) {
            pool->data[i].is_used = 1;
            return pool->data[i].worker;
        }
    }

    increment_pool(pool);
    pool->data[i].is_used = 1;
    pthread_mutex_unlock(&pool->mutex);
    return pool->data[i].worker;
}

size_t free_thread (dynamic_pool* pool, const pthread_t* thread) {
    pthread_mutex_lock(&pool->mutex);
    size_t i;
    for (i = 0; i < pool->size; i++) {
        if (&(pool->data[i].worker) == thread) {
            pool->data[i].is_used = 0;
            decrement_pool(pool);
            break;
        }
    }
    pthread_mutex_unlock(&pool->mutex);
    return i;
}

size_t free_pool (dynamic_pool* pool) {
    pthread_mutex_lock(&pool->mutex);
    free ((void*)pool->data);
    pool->size = 0;
    pool->data = NULL;
    pthread_mutex_unlock(&pool->mutex);
    return pool->size;
}

void* test(void* fdp) {
    int fd = *(int*)fdp;
    dprintf(fd, "testtest");
    close(fd);
    return NULL;
}

int main() {

    printf("Using sockets. Server is running...\n");

    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Cannot create socket");
        return 1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(8010);
    unsigned int saddr_size = sizeof(struct sockaddr_in);

    errno = 0;
    if (bind(sock_fd, (const struct sockaddr *) &saddr, saddr_size) < 0) perror("bind");
    if (listen(sock_fd, 0) < 0) perror("listen");

    thread_pool = malloc(sizeof(dynamic_pool));
    create_dynamic_thread_pool(thread_pool);

    while(1) {
        int client_fd = accept(sock_fd, (struct sockaddr*) &saddr, &saddr_size);
        pthread_t response = get_free_thread(thread_pool);
        pthread_create(&response, NULL, test, &client_fd);
        free_thread(thread_pool,&response);
    }


    return 0;
}