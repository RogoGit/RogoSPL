CC=gcc

all: shared_mem_server shared_mem_client message_queue_server message_queue_client mmap_server mmap_client

shared_mem_server:  shared_memory_server.c server_info_struct.h
		      $(CC) $(CFLAGS) -o sh_mem_server shared_memory_server.c server_info_struct.h

shared_mem_client: shared_memory_client.c server_info_struct.h
		      $(CC) $(CFLAGS) -o sh_mem_client shared_memory_client.c server_info_struct.h

message_queue_server: message_queue_server.c server_info_struct.h
		      $(CC) $(CFLAGS) -o mes_queue_server message_queue_server.c server_info_struct.h

message_queue_client: message_queue_client.c server_info_struct.h
		      $(CC) $(CFLAGS) -o mes_queue_client message_queue_client.c server_info_struct.h

mmap_server: mmap_server.c server_info_struct.h
		      $(CC) $(CFLAGS) -o mmap_server mmap_server.c server_info_struct.h

mmap_client: mmap_client.c server_info_struct.h
		      $(CC) $(CFLAGS) -o mmap_client mmap_client.c server_info_struct.h

clean:
	 @rm sh_mem_server sh_mem_client mes_queue_server mes_queue_client mmap_server mmap_client

