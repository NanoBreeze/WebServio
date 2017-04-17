#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdbool.h>
#include "LinkedList.h"

char* getCurrentDateTime();
char* getFileText(char* file);
bool writeFile(char* fileName, char* text);
bool getSettings(char* start, LinkedList* settings, LinkedList* settings301);

#endif // LIB_H_INCLUDED
