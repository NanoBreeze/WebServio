#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <time.h>


#include "Tests/minunit.h"
#include "Tests/RequestLineTests.h"
#include "Tests/LinkedListTests.h"
#include "Tests/HeaderFieldTests.h"

#include "Parser.h"
#include "LinkedList.h"
#include "Lib.h"

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


bool sendall(int fileDescriptor, char* buffer, int length) {

    int totalBytesSent = 0;
    int bytesRemaining = length;

    while (totalBytesSent < length) {
        int bytesSent = 0;

        if (( bytesSent = send(fileDescriptor, buffer + totalBytesSent, bytesRemaining, 0)) == -1) {
            break;
        }
        else {
            totalBytesSent += bytesSent;
            bytesRemaining -= bytesSent;

        }
    }

    return totalBytesSent == length;
}


int main()
{

    char* settingsText = getFileText("../WebServioSettingsCLI/settings.conf");

    LinkedList* settings = createLinkedList();
    LinkedList* settings301 = createLinkedList();

    bool isSuccess = getSettings(settingsText, settings, settings301);

    free(settingsText);

    if (containsDuplicate(settings))  {
        printf("settings contains duplicates\n");
        return;
    }

    if (containsDuplicate(settings301)) {
        printf("there are duplicate 301 settings\n");
        return;
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
    printf("about to wait to accept\n");
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

        printf("RECEIVED DATA: \n%s", buffer);
        printf("==================================");

        RequestLine requestLine;
        int requestLineLength = 0;
        if ((requestLineLength = parseRequestLine(buffer, &requestLine)) == -1) {
            printf("Error in the request line\n");
            return;
        }

        LinkedList* headerFields = createLinkedList();
        int headerLength = 0;

        if ((headerLength = parseHeader(buffer + requestLineLength, headerFields)) == -1) {
            printf("Error in the headers\n");
            return;
        }

        //error checking on the header fields
        if (containsDuplicate(headerFields)) {
            printf("Error: header fields must not contain duplicates");
            return;
        }

        //host must exist
        char* host = find(headerFields, "host");;
        if (!host) {
            printf("Error: There must be a host header field present.\n");
            return;
        }

        char* message = NULL;
        if (buffer[requestLineLength + headerLength] != '\0' ) { //message exists
            char* c_contentLength = find(headerFields, "content-length");

            if (!c_contentLength) {
                printf("Error, if message exists, must contain content length header. Error 411");
                return;
            }

            int i_contentLength = atoi(c_contentLength);

            message = (char*) malloc(i_contentLength + 1);
            extractMessage(buffer + requestLineLength + headerLength, i_contentLength, message);
        }



        printf("================== PARSED DATA =====================\n\n");

        printf("Request line\n");
        printf("\tMethod: %d. 0: GET, 1: POST, 2: HEAD\n", requestLine.method);
        printf("\tPath: %s\n", requestLine.path);
        printf("\tMajor: %d\n", requestLine.major);
        printf("\tMinor: %d\n", requestLine.minor);


        printf("Headers\n");
        Node* current = headerFields->head;

        while (current) {
            printf("\t%s: %s\n", current->key, current->value);
            current = current->next;
        }

        if (message) {
            printf("Message\n");
            printf("%s", message);

        }

    char* response;

    struct stat s;
    if (stat(requestLine.path, &s) == 0) {

        if (s.st_mode & S_IFDIR) {

            printf("It's a directory\n");
            response = create200MessageDir(requestLine.path); //should be able to set configuration to return 404 or list of files in the directory
        }
        else if (s.st_mode & S_IFREG) {
            printf("It's a file\n");
            response = create200Message(requestLine.path); //should be able to return 200 or 301 response
        }
        else {
            printf("Neither!\n");
            response = create404Message("404.html");
        }
    }
    else {
        //does not exist
        response = create404Message("404.html");
    }

    if (!response) { return NULL; }
        if (sendall(acceptedFileDescriptor, response, strlen(response))) {
            printf("All sent!\n");
            close(acceptedFileDescriptor); //not sure why without this, doesn't send. Wierd????? Wierd!
        }
        else {
            printf("Error with sending data: %s\n", strerror(errno));

        }
    }

    getchar();

    return 0;
}




