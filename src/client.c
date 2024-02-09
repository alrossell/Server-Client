#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <arpa/inet.h>

#include "../include/globals.h"
#include "../include/client.h"

int create_client() {
    char buffer[1024] =  { 0 };
    char* client_message = "This is a message from the client";

    printf("Creating client\n");

    int status, value_read, client;
    struct sockaddr_in server_address;

    if ((client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket creation error \n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported \n");
        return 1;
    }

    if ((status = connect(client, (struct sockaddr*)&server_address, sizeof(server_address))) < 0) {
        fprintf(stderr, "Connection Failed \n");
        return 1;
    }

    printf("Client sending message\n");
    send(client, client_message, strlen(client_message), 0);


    printf("Client reading message\n");
    read(client, buffer, 1024 - 1);

    printf("Message client read: %s\n", buffer);
 

    close(status);
    close(client);
    return 0;
}

