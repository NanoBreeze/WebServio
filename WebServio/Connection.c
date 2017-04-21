
#include "Connection.h"
#include "LinkedList.h"
#include "Parser.h"
#include "Lib.h"
#include "MessageFactory.h"

#include <stdbool.h>
#include <errno.h>
#include <pthread.h>



pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

threadsFree = 2;


bool validateHeaders(LinkedList* headerFields) {
    if (containsDuplicate(headerFields)) {
        printf("Error: header fields must not contain duplicates");
        return false;
    }

    char* host = find(headerFields, "host");
    if (!host) {
        printf("Error: There must be a host header field present.\n");
        return false;
    }

    return true;
}


bool extractMessage(char* request, LinkedList* headerFields, char** message, bool* send411) {


    char* c_contentLength = find(headerFields, "content-length");

// I don't know why but sometimes Firefox seems to be repeating headers in the message section but doesn't contain a content-length
    if (!c_contentLength) {
        printf("Error, if message exists, must contain content length header. Error 411\n");
        *send411 = true;

        return false;
    }

    int i_contentLength = atoi(c_contentLength);

    *message = (char*) malloc(i_contentLength + 1);
    if (!*message) {
        printf("Out of memory for extracting message!\n");
        return false;
    }

    copyMessage(request, i_contentLength, *message);

    return true;
}


void printRequest(RequestLine requestLine, LinkedList* headerFields, char* message) {

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
}

char* createResponse(char* path, LinkedList* headerFields, LinkedList* settings, LinkedList* settings301) {

   //we check if path has 301 because if so, redirect regardless of whether the path actually exists or not
    if (get301Path(settings301, path)) {
        char* host = find(headerFields, "host");

        char* fullHost = malloc(strlen("http://") + strlen(host) + 1);
        strcpy(fullHost, "http://");
        strcat(fullHost, host);

        char* newPath = get301Path(settings301, path);
        char* response = create301Message(fullHost, newPath);
        free(fullHost);

        return response;
    }

    //send index file if empty path
    else if (isEmptyPath(path)) {
        char* index = getIndexPath(settings);
        return create200Message(index);
    }
    else if (isDir(path)) {

        if (isDirListingsTrue(settings)) {
            return create200MessageDir(path);;
        }
        else {
            char* fourOhFour = get404Path(settings);
            return create404Message(fourOhFour);
        }
    }
    else if (isExistentFile(path)) {
        return create200Message(path);
    }
    else {
        char* fourOhFour = get404Path(settings);
        return create404Message(fourOhFour);
    }
}


bool sendResponse(int fileDescriptor, char* buffer, int length) {

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


void send400Message(int acceptedFileDescriptor) {
    char* response = create400Message();
    sendResponse(acceptedFileDescriptor, response, strlen(response));
    free(response);
}

void terminateConnection(int acceptedFileDescriptor) {

    close(acceptedFileDescriptor);
    /*pthread_mutex_lock(&lock);

    threadsFree++;
    printf("About to signal condition. Incremented threadsFree already to: %d\n", threadsFree);
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&lock);
    */
}






void* startConnection(void* p) {

    Params* params = (Params*) p;

    int acceptedFileDescriptor = params->acceptedFileDescriptor;
    LinkedList* settings = params->settings;
    LinkedList* settings301 = params->settings301;



    while (1) {
        char buffer[10000];


        int bytesReceived = 0;
        if ( (bytesReceived = recv(acceptedFileDescriptor, buffer, sizeof buffer, 0)) <= 0) {
            //connection closed
            if (bytesReceived == 0) {
                printf("Client closed connection. Current thread id: %d.\n", pthread_self());
                fflush(stdout);
            }
            else {
                printf("Error with receiving information: %s.\n", strerror(errno));
                fflush(stdout);
            }


            terminateConnection(acceptedFileDescriptor);

            printf("Connection closed. Exiting. Current thread id: %d.\n", pthread_self());
            fflush(stdout);
            return;
        }
        else {

            printf("\nRECEIVED DATA. Current thread id: %d\n%s", pthread_self(), buffer);

            printf("==================================");

            RequestLine requestLine;
            int requestLineLength = 0;
            if (!parseRequestLine(buffer, &requestLine, &requestLineLength)) {
                printf("Error in the request line.\n");

                send400Message(acceptedFileDescriptor);
                terminateConnection(acceptedFileDescriptor);
                return;
            }


            LinkedList* headerFields = createLinkedList();
            int headerLength = 0;
            if (!parseHeader(buffer + requestLineLength, headerFields, &headerLength)) {
                printf("Error in the headers\n");

                send400Message(acceptedFileDescriptor);
                terminateConnection(acceptedFileDescriptor);
                return;
            }

            if (!validateHeaders(headerFields)) {
                printf("Error in the headers\n");

                send400Message(acceptedFileDescriptor);
                terminateConnection(acceptedFileDescriptor);
                return;
            }


            char* message = NULL;

            /*
            if (buffer[requestLineLength + headerLength] != '\0' ) { //message exists. I don't know why but sometimes Firefox seems to be repeating headers in the message section but doesn't contain a content-length????

                bool send411 = false;
                if (!extractMessage(buffer + requestLineLength + headerLength, headerFields, &message, &send411)) {
                    printf("Error in extracting the message\n");

                    if (send411) {
                        printf("About to send 411 error");
                        char* response = create411Message();
                        sendResponse(acceptedFileDescriptor, response, strlen(response));
                        free(response);
                    }
                    continue;
                }
            }
            */

            //printRequest(requestLine, headerFields, message);
            if (message) {
                free(message);
            }

            char* response = createResponse(requestLine.path, headerFields, settings, settings301);

            if (!response) {
                terminateConnection(acceptedFileDescriptor);
                return;
            }

            if (sendResponse(acceptedFileDescriptor, response, strlen(response))) {
                printf("All sent!\n");
                free(response);
            }
            else {
                printf("Error with sending data: %s\n", strerror(errno));
            }
        }
    }
}


