all: server_process client_process

CC = cc
INCLUDE = include/
CFLAGS = -Wall -Wextra -ansi -pedantic -Werror

server_process: src/server_process.c src/server_funcs.c
	$(CC) -I$(INCLUDE) $(CFLAGS) src/server_process.c src/server_funcs.c -o server_process

client_process: src/client_process.c src/client_funcs.c
	$(CC) -I$(INCLUDE) $(CFLAGS) src/client_process.c src/client_funcs.c -o client_process

clean:
	-rm server_process client_process cli_serv.zip

zip:
	zip -r cli_serv.zip .
