#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define ESTIMATED_CONNECTIONS 10
#define MAX_EVENTS 128



//feeling a bit lazy to put this as a setting so hardcoding this for now.
//This specifies whether our web server is multithreaded or (if not defined) multiplexed
//#define MULTITHREADED

//used to specify the maximum number of threads to be created if web server uses multithreading
#define MAX_THREAD_COUNT 10


#endif // COMMON_H_INCLUDED
