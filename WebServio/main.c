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


bool isEmptyPath(char* path) {
    return strcmp(path, "") == 0;
}

bool isDir(char* path) {
    struct stat s;
    return stat(path, &s) == 0 && s.st_mode & S_IFDIR;
}

bool isExistentFile(char* path) {
    struct stat s;
    return stat(path, &s) == 0 && s.st_mode & S_IFREG;
}

//if specified, return specified. Otherwise, return default
char* getIndexPath(LinkedList* settings) {

    char* index = find(settings, "Index");
    if (!index) {
        index = "hello.html";
    }

    return index;
}


char* get404Path(LinkedList* settings) {

    char* fourOhFour = find(settings, "404");
    if (!fourOhFour) {
        fourOhFour = "404.html";
    }

    return fourOhFour;
}


char* get301Path(LinkedList* settings301, char* source) {
    return find(settings301, source); //NULL means no file associated
}

bool isDirListingsTrue(LinkedList* settings) {

    char* trueFalse = find(settings, "DirListings");

    return strcmp(trueFalse, "true") == 0;
}

int main()
{

    char* settingsText = getFileText("../WebServioSettingsCLI/settings.conf");

    LinkedList* settings = createLinkedList();
    LinkedList* settings301 = createLinkedList();

    if (!getSettings(settingsText, settings, settings301)) {
        printf("Cannot get settings.\n");
        return;
    }

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
    int backlog;

    char* sBacklog = find(settings, "Backlog");


    if (!sBacklog) {
        backlog = 20;
    }
    else {
        char* end;
        long int result = strtol(sBacklog, &end, 10);

        if (*end != '\0' || atoi(sBacklog) < 1) {
            printf("Make sure to specify a positive integer.\n>>> ");
            return;
        }

        backlog = atoi(sBacklog);
    }


    if ((listenErrorCode = listen(fileDescriptor, backlog)) == -1) {
        printf("Error with listening: %s\n", strerror(errno));
        return;
    }
    printf("about to wait to accept\n");

    while(1) {
    int acceptedFileDescriptor;
    socklen_t acceptedFileDescriptorLength = sizeof acceptedFileDescriptor;
    struct sockaddr_storage acceptedAddr;

    if ((acceptedFileDescriptor = accept(fileDescriptor, (struct sockaddr*) &acceptedAddr, &acceptedFileDescriptorLength)) == -1) {
        printf("Error with accepting: %s\n", strerror(errno));
        return;
    }

    printf("Got connection. The file descriptor is: %d\n", acceptedFileDescriptor);
    fflush(stdout);



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

        /*char* testR = create200Message("home.html");

        if (sendall(acceptedFileDescriptor, testR, strlen(testR))) {
            printf("All sent!\n");
            free(testR);
            close(acceptedFileDescriptor); //not sure why without this, doesn't send. Wierd????? Wierd!
        }
        else {
            printf("Error with sending data: %s\n", strerror(errno));
        }
*/







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
      /*  if (buffer[requestLineLength + headerLength] != '\0' ) { //message exists. I don't know why but sometimes Firefox seems to be repeating strange things in its message????
            char* c_contentLength = find(headerFields, "content-length");

            if (!c_contentLength) {
                printf("Error, if message exists, must contain content length header. Error 411");
                return;
            }


            int i_contentLength = atoi(c_contentLength);

            message = (char*) malloc(i_contentLength + 1);
            extractMessage(buffer + requestLineLength + headerLength, i_contentLength, message);
        }
*/


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


        //we check if path has 301 because if so, redirect regardless of whether the path actually exists or not
        if (get301Path(settings301, requestLine.path)) {
            char* host = find(headerFields, "host");

            char* fullHost = malloc(strlen("http://") + strlen(host) + 1);
            strcpy(fullHost, "http://");
            strcat(fullHost, host);

            char* newPath = get301Path(settings301, requestLine.path);
            response = create301Message(fullHost, newPath);
            free(fullHost);
        }
        //send index file if empty path
        else if (isEmptyPath(requestLine.path)) {
            char* index = getIndexPath(settings);
            response = create200Message(index);
        }
        else if (isDir(requestLine.path)) {

            if (isDirListingsTrue(settings)) {
                response = create200MessageDir(requestLine.path);;
            }
            else {
                char* fourOhFour = get404Path(settings);
                response = create404Message(fourOhFour);
            }
        }
        else if (isExistentFile(requestLine.path)) {
            response = create200Message(requestLine.path);
        }
        else {
            char* fourOhFour = get404Path(settings);
            response = create404Message(fourOhFour);
        }


        if (!response) { return NULL; }
        if (sendall(acceptedFileDescriptor, response, strlen(response))) {
            printf("All sent!\n");
            free(response);
            close(acceptedFileDescriptor); //not sure why without this, doesn't send. Wierd????? Wierd!
        }
        else {
            printf("Error with sending data: %s\n", strerror(errno));

        }
    }

}
    getchar();

    return 0;
}




