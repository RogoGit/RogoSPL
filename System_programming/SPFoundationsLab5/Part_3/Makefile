CC=gcc

all: server client signals pipe

server: sockets_server.c server_info_struct.h
	$(CC) -lsocket -o server sockets_server.c server_info_struct.h

client: sockets_client.c server_info_struct.h
	$(CC) -lsocket -o client sockets_client.c server_info_struct.h

signals: server_signals.c server_info_struct.h
	$(CC) -o signals server_signals.c server_info_struct.h

pipe: unnamed_pipe.c
	$(CC) -o pipe unnamed_pipe.c

clean:
	@rm server client signals pipe


