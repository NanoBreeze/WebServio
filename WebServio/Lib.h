#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdbool.h>

char* getCurrentDateTime();
char* getFileText(char* file);
bool writeFile(char* fileName, char* text);


#endif // LIB_H_INCLUDED
