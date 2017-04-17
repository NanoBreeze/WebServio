#ifndef FILEOPERATIONS_H_INCLUDED
#define FILEOPERATIONS_H_INCLUDED

#include <stdbool.h>

char* getFileText(char* file);
bool writeFile(char* fileName, char* text);


#endif // FILEOPERATIONS_H_INCLUDED
