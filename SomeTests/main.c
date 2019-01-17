#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main() {
    struct stat src_stat;
    int src_fd = open("../hw", O_RDONLY);

    fstat(src_fd, &src_stat);
    size_t src_size = src_stat.st_size;

    void* executable = mmap(NULL, src_size, PROT_READ | PROT_EXEC, MAP_SHARED, src_fd, 0);

    int (*func)(const char*, int);
    func = (int (*)())executable;
    int ret = func("hello", 5);

    return 0;
}