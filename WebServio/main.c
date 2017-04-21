#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include <sys/epoll.h>
#include <fcntl.h>

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
#include "Connection.h"
#include "Common.h"

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




int main()
{

    char* settingsText = getFileText( "../WebServioSettingsCLI/settings.conf");

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


    int acceptedFileDescriptor;
    socklen_t acceptedFileDescriptorLength = sizeof acceptedFileDescriptor;
    struct sockaddr_storage acceptedAddr;


    #ifdef MULTITHREADED
    for(;;) {
        pthread_mutex_lock(&lock);

        while (threadsFree == 0) { //use while in case of spurious wakeups
            pthread_cond_wait(&cond, &lock);
        }

        threadsFree--;
        printf("Condition met. Current threads free: %d", threadsFree);
        pthread_mutex_unlock(&lock);

        //opens a connection
        if ((acceptedFileDescriptor = accept(fileDescriptor, (struct sockaddr*) &acceptedAddr, &acceptedFileDescriptorLength)) == -1) {
            printf("Error with accepting: %s\n", strerror(errno));
            return;
        }

        printf("\n\n\nGot connection. The file descriptor is: %d\n", acceptedFileDescriptor);

        Params params;
        params.acceptedFileDescriptor = acceptedFileDescriptor;
        params.settings = settings;
        params.settings301 = settings301;

        pthread_t thread;

        int ret = pthread_create(&thread, NULL, startConnection, (void*) &params);
        if (ret) {
            printf("Error in creating the thread: %d\n", ret);
        }

        pthread_detach(thread);
    }

    #else

    int epfd = epoll_create(ESTIMATED_CONNECTIONS);
    if (epfd == -1) {
        printf("Error in creating the epoll: %s\n", strerror(errno));
        return;
    }

    struct epoll_event event;
    int ret;

    event.data.fd = fileDescriptor;
    event.events = EPOLLIN | EPOLLOUT;

    if (ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fileDescriptor, &event)) {
        printf("Error in adding the socket file descriptor to epoll: %s\n", strerror(errno));
        return;
    }

    struct epoll_event *events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
    if (!events) {
        printf("Malloc error in allocating for events\n");
        return;
    }



    for (;;) {
        int numberEventsChanged = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (numberEventsChanged == -1) {
            printf("Error with epoll: %s\n", strerror(errno));
            continue;
        }

        for (int i = 0; i < numberEventsChanged; i++) {
            if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
                printf("There's an error with this fd: %d", events[i].data.fd);
                close(events[i].data.fd);
            }
            else if ( events[i].data.fd == fileDescriptor) { //new connection to accept

                //opens a connection
                if ((acceptedFileDescriptor = accept(fileDescriptor, (struct sockaddr*) &acceptedAddr, &acceptedFileDescriptorLength)) == -1) {
                    printf("Error with accepting: %s\n", strerror(errno));
                }

                printf("\n\n\nGot connection. The file descriptor is: %d\n", acceptedFileDescriptor);

                //make the socket to be non-blocking
                int flags = fcntl(acceptedFileDescriptor, F_GETFL, 0);
                flags |= O_NONBLOCK;
                fcntl(acceptedFileDescriptor, F_SETFL, flags);

                struct epoll_event event;
                int ret;

                event.data.fd = acceptedFileDescriptor;
                event.events = EPOLLIN | EPOLLET;

                if (ret = epoll_ctl(epfd, EPOLL_CTL_ADD, acceptedFileDescriptor, &event)) {
                    printf("Error in adding the socket file descriptor to epoll: %s\n", strerror(errno));
                }
            }
            else {  //received data on a previous connection

                Params params;
                params.acceptedFileDescriptor = events[i].data.fd;

                char buffer[10000];

                int bytesReceived = 0;
                if ( (bytesReceived = read(params.acceptedFileDescriptor, buffer, sizeof buffer)) <= 0) {

                    analyzeReadError(params.acceptedFileDescriptor, bytesReceived);
                }
                else {
                    params.settings301 = settings301;
                    params.settings = settings;

                    processRequest(buffer, &params);

                }
            }
        }
    }

    free(events);

    #endif // MULTITHREADED




    close(fileDescriptor);

    return 0;
}




