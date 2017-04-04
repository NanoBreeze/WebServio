#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include "Tests/minunit.h"
#include "Tests/RequestLineTests.h"
#include "Tests/LinkedListTests.h"
#include "Tests/HeaderFieldTests.h"

#include "Parser.h"

#define PORT "3002"
#define BACKLOG 20


int run_all_tests() {
    tests_run = 0;

     printf("======= RUNNING REQUEST LINE PARSER TESTS ======\n");
     char *result = run_all_request_line_parser_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n\n", tests_run);



     printf("======= RUNNING LINKED LIST TESTS ======\n");
     tests_run = 0;
     result = run_all_linked_list_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n\n", tests_run);




     printf("======= RUNNING HEADER FIELD TESTS ======\n");
     tests_run = 0;
     result = run_all_header_field_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n\n", tests_run);

     getchar();

     return 0;
 }


char buffer[10000];

int main()
{

   run_all_tests();

    /*
    char a[] = "    GET /wersodijfoisdjf HTTP/1.1\r\n";
    RequestLine requestLine;
    parseRequestLine(a, &requestLine);

    char* b = strtok(a, " ");

    while (b != NULL) {
        printf("The value of b is: %s\n", b);
        b = strtok(NULL, " ");
    }


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



    //char* delimitedStr = strtok(buffer, "\r\n");


    for (int i = 0 ; i < strlen(buffer); i++) {
        printf("The characters is: %d\n", buffer[i]);
        int requestLineEndPos = parseRequestLine(buffer); //requestLineEndPos would be on the last character in the request line, aka, the LF

    }


   // while (delimitedStr != NULL) {
      //  printf("The value of delimitedStr is: %s\n", delimitedStr);
      // delimitedStr = strtok(NULL, "\r\n");
//    }

        printf("Received data: \n%s", buffer);
    }

    getchar();

      */
    return 0;

}

