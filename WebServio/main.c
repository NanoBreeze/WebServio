#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>



#define BACKLOG 20

int main()
{

   printf("Hello world!\n");

    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, "3002", &hints, &result)) != 0) {
        printf("Error with getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    int fileDescriptor;
    if ((fileDescriptor = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        printf("Error with fileDescriptor: %s\n", strerror(errno));
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

    int acceptedFileDesciptor;
    socklen_t acceptedFileDesciptorLength = sizeof acceptedFileDesciptor;
    struct sockaddr_storage acceptedAddr;
    if ((acceptedFileDesciptor = accept(fileDescriptor, (struct sockaddr*) &acceptedAddr, &acceptedFileDesciptorLength)) == -1) {
        printf("Error with accepting: %s\n", strerror(errno));
        return;
    }

    printf("%d", acceptedFileDesciptor);









    return 0;
}
