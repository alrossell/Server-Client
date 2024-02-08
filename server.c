#include <asm-generic/socket.h>
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

int create_server() {
    int server, new_socket;
    int opt = 1;
    
    int valread;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[1024] =  { 0 };
    char* server_message = "This is a message from the server\n";

    if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket failed\n");
        return 1;
    }

    if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        fprintf(stderr, "Set Socket Option Error\n");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        fprintf(stderr, "Bind failed\n");
        return 1;
    }

    if(listen(server, 32) < 0) {
        fprintf(stderr, "Listen failed\n");
        return 1;
    }

    if((new_socket = accept(server, (struct sockaddr*)&address, &addrlen)) < 0) {
        fprintf(stderr,"Accept failed\n");
        return 1;
    }

    printf("Server reading message\n");
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    send(new_socket, server_message, strlen(server_message), 0);
    printf("Server message sent\n");
 
    close(new_socket);
    close(server);
    return 0;
}

