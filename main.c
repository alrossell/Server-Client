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

#include "globals.h"

int create_client() {
    printf("Creating client\n");

    int status, value_read, client;
    struct sockaddr_in server_address;

    char* client_message = "This is a message from the server\n";
    char buffer[1024] = { 0 };

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

    printf("Sending message to server\n");
    send(client, client_message, strlen(client_message), 0);
    printf("Client message sent\n");
    value_read = read(client, buffer, 1024 - 1); 
    printf("Client buffer: %s\n", buffer);

    close(client);
    return 0;

}

void* server_thread_function(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d is running\n", thread_id);
    create_server();
    pthread_exit(NULL);
}

void* client_thread_function(void* arg) {
    int thread_id = *((int*)arg);
    printf("Thread %d is running\n", thread_id);
    create_client();
    pthread_exit(NULL);
}

int main() {
    int arg_one = 1;
    int arg_two = 2;

    pthread_t thread_one_id;
    pthread_t thread_two_id;

    // Create a new thread
    if (pthread_create(&thread_one_id, NULL, server_thread_function, (void*)&arg_one) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    printf("Enter (Y/N) to create client thread: ");
    char input;

    scanf("%c", &input);

    if (input == 'Y' || input == 'y') {
        if (pthread_create(&thread_two_id, NULL, client_thread_function, (void*)&arg_two) != 0) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    printf("Main thread is waiting for the created thread to finish\n");

    // Wait for the created thread to finish
    if (pthread_join(thread_one_id, NULL) != 0) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }

    if (pthread_join(thread_two_id, NULL) != 0) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }

    printf("Created thread has finished, main thread exiting\n");

    return 0;
}

