CC=gcc
CFLAGS=-pthread -lrt

all: sem_posix sem_sysv lock_mutex lock_rwlock

sem_posix: sem_library_func.c
	   $(CC) $(CFLAGS) -o sem_posix sem_library_func.c

sem_sysv: sem_sysv.c
	   $(CC) $(CFLAGS) -o sem_sysv sem_sysv.c

lock_mutex: lock_mutex.c
	   $(CC) $(CFLAGS) -o lock_mutex lock_mutex.c

lock_rwlock: lock_rwlock.c
	   $(CC) $(CFLAGS) -o lock_rwlock lock_rwlock.c

clean:
	@rm sem_posix sem_sysv lock_mutex lock_rwlock

