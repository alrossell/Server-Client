CC=gcc

make: main.c
	gcc main.c ./src/server.c ./src/client.c -o server-client

clean: main.c
	rm -f ./program
