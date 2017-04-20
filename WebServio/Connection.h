#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <pthread.h>

extern pthread_mutex_t lock;
extern pthread_cond_t cond;

extern int threadsFree;

/*
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int threadsFree = 2;
*/
/*
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    threadsFree = 2;
*/

void* startConnection(void* p);

#endif // CONNECTION_H_INCLUDED
