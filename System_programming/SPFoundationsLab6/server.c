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
    int* fd;
    pthread_cond_t* cond;
    } server_thread;

typedef struct  {
    volatile server_thread* data;
    volatile size_t size;
    pthread_mutex_t mutex;
    } dynamic_pool;

dynamic_pool* thread_pool;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

size_t free_thread (dynamic_pool* pool, const pthread_t* thread) {
    size_t i;
    for (i = 0; i < pool->size; i++) {
        if (&(pool->data[i].worker) == thread) {
            pool->data[i].fd = NULL;
            pool->data[i].is_used = 0;
            break;
        }
    }
    return i;
}

void* test(void* thr_info) {
    pthread_mutex_lock(&thread_pool->mutex);
    server_thread* info = (server_thread *) thr_info;
    while (!(info->is_used)) {
        pthread_cond_wait(info->cond,&thread_pool->mutex);
    }
    dprintf(*info->fd, "testtest");
    close(*info->fd);
    free_thread(thread_pool,&(info->worker));
    pthread_mutex_unlock(&thread_pool->mutex);
    return NULL;
}

size_t create_dynamic_thread_pool (dynamic_pool* pool) {
    pool->size = DEFAULT_SIZE;
    pool->data = (server_thread*) malloc (pool->size * sizeof(server_thread));
    pthread_mutex_init(&pool->mutex,NULL);
    unsigned int i;
    for (i = 0; i < pool->size; i++) {
        pool->data[i].fd = NULL;
        pool->data[i].is_used = 0;
        pool->data[i].cond = &(pthread_cond_t) PTHREAD_COND_INITIALIZER;
        pthread_create((void*) &(pool->data[i].worker), NULL, test, (void*) &pool->data[i]);
    }
    return pool->size;
}

size_t increment_pool (dynamic_pool* pool) {
    size_t old_size = pool->size;
    pool->size += SIZE_CHANGE;
    pool->data = (server_thread*) realloc ((void*)pool->data, pool->size * sizeof(server_thread));
    size_t i;
    for (i = old_size; i < pool->size; i++) {
        //pool->data[i].fd = (int) malloc(sizeof(int));
       // pool->data[i].fd = (int) NULL;
        pool->data[i].is_used = 0;
        pthread_create((void*) &(pool->data[i].worker), NULL, test, (void*) &(pool->data[i].fd));
    }
    return pool->size;
}

server_thread* get_free_thread (dynamic_pool* pool) {
    unsigned int i;
    for (i = 0; i < pool->size; i++) {
        if (!pool->data[i].is_used) {
            pool->data[i].is_used = 1;
            return (void*) &(pool->data[i]);
        }
    }
    //increment_pool(pool);
    pool->data[i].is_used = 1;
    return (void*) &(pool->data[i]);
}

size_t free_pool (dynamic_pool* pool) {
    pthread_mutex_lock(&pool->mutex);
    free ((void*)pool->data);
    pool->size = 0;
    pool->data = NULL;
    pthread_mutex_unlock(&pool->mutex);
    return pool->size;
}

int main() {

    printf("Server is running...\n");

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
    setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR, &saddr, saddr_size);
    if (bind(sock_fd, (const struct sockaddr *) &saddr, saddr_size) < 0) perror("bind");
    if (listen(sock_fd, 0) < 0) perror("listen");

    thread_pool = malloc(sizeof(dynamic_pool));
    create_dynamic_thread_pool(thread_pool);

    while(1) {
        int client_fd = accept(sock_fd, (struct sockaddr*) &saddr, &saddr_size);
        pthread_mutex_lock(&thread_pool->mutex);
        server_thread* response = get_free_thread(thread_pool);
        response->fd = &client_fd;
        pthread_cond_broadcast(response->cond);
        pthread_mutex_unlock(&thread_pool->mutex);
    }

    return 0;
}