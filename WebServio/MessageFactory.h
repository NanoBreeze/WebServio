#ifndef MESSAGEFACTORY_H_INCLUDED
#define MESSAGEFACTORY_H_INCLUDED

#include <stdbool.h>

#include "LinkedList.h"

char* create200Message(char* file);
char* create301Message(char* file);
char* create404Message(char* file);
char* create400Message();
char* create411Message();

char* getCurrentDateTime();
char* getFileText(char* file);
char* createMessage(char* file, char* statusLine);

#endif // MESSAGEFACTORY_H_INCLUDED
