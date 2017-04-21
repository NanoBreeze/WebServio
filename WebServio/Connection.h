#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <pthread.h>
#include "Lib.h"


extern pthread_mutex_t lock;
extern pthread_cond_t cond;

extern int threadsFree;


void* startConnection(void* p);
void processRequest(char* buffer, Params* params);
void analyzeReadError(int acceptedFileDescriptor, int bytesReceived);


#endif // CONNECTION_H_INCLUDED
