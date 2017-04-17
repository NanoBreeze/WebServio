#include "FileOperations.h"

#include <string.h>
#include <stdio.h>
#include <dirent.h>

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
