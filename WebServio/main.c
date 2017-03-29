#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <pthread.h>

#define PORT "3002"
#define BACKLOG 20


char buffer[100000];


int main()
{


    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, PORT, &hints, &result)) != 0) {
        printf("Error with getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    int fileDescriptor;
    if ((fileDescriptor = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        printf("Error with fileDescriptor: %s\n", strerror(errno));
        return;
    }

    int yes = 1;
    int setSockErrorCode;
    if ((setSockErrorCode = setsockopt(fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes)) == -1) {
        printf("Error with setting setsockopt to allow reusing port: %s\n", strerror(errno));
        return;
    }

    int bindErrorCode;
    if ((bindErrorCode = bind(fileDescriptor, result->ai_addr, result->ai_addrlen)) == -1) {
        printf("%d", fileDescriptor);
        printf("Error with binding: %s\n", strerror(errno));
        return;
    }

    int listenErrorCode;
    if ((listenErrorCode = listen(fileDescriptor, BACKLOG)) == -1) {
        printf("Error with listening: %s\n", strerror(errno));
        return;
    }

    printf("about to wait\n");

    int acceptedFileDescriptor;
    socklen_t acceptedFileDescriptorLength = sizeof acceptedFileDescriptor;
    struct sockaddr_storage acceptedAddr;
    if ((acceptedFileDescriptor = accept(fileDescriptor, (struct sockaddr*) &acceptedAddr, &acceptedFileDescriptorLength)) == -1) {
        printf("Error with accepting: %s\n", strerror(errno));
        return;
    }

    printf("%d", acceptedFileDescriptor);

    int bytesReceived = 0;
    if ( (bytesReceived = recv(acceptedFileDescriptor, buffer, sizeof buffer, 0)) <= 0) {
        //connection closed
        if (bytesReceived == 0) {
            printf("Client closed connection");
        }
        else {
            printf("Error with receiving information: %s\n", strerror(errno));
        }
        close(acceptedFileDescriptor);
    }
    else {
        printf("Received data: \n%s", buffer);
    }


    return 0;
}
