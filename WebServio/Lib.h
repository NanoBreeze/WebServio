#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdbool.h>
#include "LinkedList.h"

//parameter passed for multithreading
typedef struct p {
    int acceptedFileDescriptor;
    LinkedList* settings;
    LinkedList* settings301;
} Params;



char* getCurrentDateTime();
char* getFileText(char* file);
bool writeFile(char* fileName, char* text);
bool getSettings(char* start, LinkedList* settings, LinkedList* settings301);

bool isEmptyPath(char* path);
bool isDir(char* path);
bool isExistentFile(char* path);
char* getIndexPath(LinkedList* settings);
char* get404Path(LinkedList* settings);
char* get301Path(LinkedList* settings301, char* source);
bool isDirListingsTrue(LinkedList* settings);

#endif // LIB_H_INCLUDED
