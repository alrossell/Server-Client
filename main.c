#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "include/server.h"
#include "include/client.h"

void* server_thread_function(void* arg) {
    printf("Server thread function\n");
    create_server();
    pthread_exit(NULL);
}

void* client_thread_function(void* arg) {
    printf("Client thread function\n");
    create_client();
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    int arg_one = 1;
    int arg_two = 2;

    pthread_t server_thread;
    pthread_t client_thread;

    if (pthread_create(&server_thread, NULL, server_thread_function, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }     

    if (pthread_create(&client_thread, NULL, client_thread_function, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }


    if (pthread_join(server_thread, NULL) != 0) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }

    if (pthread_join(client_thread, NULL) != 0) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }

    printf("Created thread has finished, main thread exiting\n");

    return 0;
}

