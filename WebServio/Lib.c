#include "Lib.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>



char* getCurrentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    char* dateTime = asctime(timeinfo);
    dateTime[strlen(dateTime) - 1] = '\0'; //trim the \n

    return dateTime; //date format isn't correct, right now showing local time only
}


char* getFileText(char* file) {

        FILE* fp;
        fp = fopen(file, "rb");
        char* fileText;
        long fsize = 0;

        //store all text in one memory block. Consider chunking later
        if (fp) {
            fseek(fp, 0, SEEK_END);
            fsize = ftell(fp);
            rewind(fp);

            fileText = (char*) malloc(fsize+1);
            if (!fileText) {
                printf("Out of memory!\n");
                return NULL;
            }
            fread(fileText, fsize, 1, fp);

            fileText[fsize] = '\0';
            printf("%s", fileText);
            fclose(fp);
        }

        return fileText;
}


bool writeFile(char* fileName, char* text) {

    FILE* file = fopen(fileName, "wb");
    if (!file) { return false; }

    fprintf(file, "%s", text);
    fclose(file);

    return true;
}


bool getSettings(char* start, LinkedList* settings, LinkedList* settings301) { //separate linked list for 301 because contains two entries: src and destination

    return parseSettings(start, settings, settings301);
}

bool isEmptyPath(char* path) {
    return strcmp(path, "") == 0;
}

bool isDir(char* path) {
    struct stat s;
    return stat(path, &s) == 0 && s.st_mode & S_IFDIR;
}

bool isExistentFile(char* path) {
    struct stat s;
    return stat(path, &s) == 0 && s.st_mode & S_IFREG;
}



//if specified, return specified. Otherwise, return default
char* getIndexPath(LinkedList* settings) {

    char* index = find(settings, "Index");
    if (!index) {
        index = "hello.html";
    }

    return index;
}

char* get404Path(LinkedList* settings) {

    char* fourOhFour = find(settings, "404");
    if (!fourOhFour) {
        fourOhFour = "404.html";
    }

    return fourOhFour;
}

char* get301Path(LinkedList* settings301, char* source) {
    return find(settings301, source); //NULL means no file associated
}

bool isDirListingsTrue(LinkedList* settings) {

    char* trueFalse = find(settings, "DirListings");

    return strcmp(trueFalse, "true") == 0;
}

