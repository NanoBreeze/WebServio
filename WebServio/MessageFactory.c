#include "MessageFactory.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



char* create200Message(char* file) {

    char* statusLine = "HTTP/1.1 200 OK\r\n";

    char* response = createMessage(file, statusLine);
    if (!response) { return NULL; }

    return response;

}

char* create301Message(char* file) {

    char* statusLine = "HTTP/1.1 301 Moved Permanently\r\n";

    char* response = createMessage(file, statusLine);
    if (!response) { return NULL; }

    return response;
}


char* create404Message(char* file) {

    char* statusLine = "HTTP/1.1 404 Not Found\r\n";

    char* response = createMessage(file, statusLine);
    if (!response) { return NULL; }

    return response;
}

char* create400Message() {

    char* dateTime = getCurrentDateTime();
    char* prefix = "HTTP/1.1 400 Bad Request\r\n"
        "Server: WebServio\r\n"
         "Date: ";
    char* suffix = "\r\n\r\n";

    char* message = (char*) malloc(strlen(prefix) + strlen(dateTime) + strlen(suffix) + 1);
    if (!message) { return NULL; }

    strcpy(message, prefix);
    strcat(message, dateTime);
    strcat(message, suffix);

    return message;
}

char* create411Message() { //maybe pass in the char* instead of returning?

    char* dateTime = getCurrentDateTime();
    char* prefix = "HTTP/1.1 411 Length Required\r\n"
        "Server: WebServio\r\n"
         "Date: ";
    char* suffix = "\r\n\r\n";

    char* message = (char*) malloc(strlen(prefix) + strlen(dateTime) + strlen(suffix) + 1);
    if (!message) { return NULL; }

    strcpy(message, prefix);
    strcat(message, dateTime);
    strcat(message, suffix);

    return message;

}

char* getCurrentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    char* dateTime = asctime(timeinfo);
    dateTime[strlen(dateTime) - 1] = 0; //trim the \n

    //return "sodfij";
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

char* createMessage(char* file, char* statusLine) {


    char* fileText = getFileText(file);
    if (!fileText) { return NULL; }

    char* dateTime = getCurrentDateTime();
    char* hardCodedHeaders =
        "Server: WebServio\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n";

    int lengthOfFileTextLength = 0;
    if (strlen(fileText) > 0) {
        lengthOfFileTextLength = floor(log10(strlen(fileText))) + 1;
    }

    char* message = (char*) malloc(strlen(statusLine) + strlen(hardCodedHeaders) + strlen("Content-length: ") + lengthOfFileTextLength + strlen("\r\nDate: ") +
        strlen(dateTime) + strlen("\r\n\r\n") + strlen(fileText) + 1);

    if (!message) { return NULL; }

    char sLengthOfFileText[lengthOfFileTextLength];
    sprintf(sLengthOfFileText, "%d", strlen(fileText));

    strcpy(message, statusLine);
    strcat(message, hardCodedHeaders);
    strcat(message, "Content-length: ");
    strcat(message, sLengthOfFileText);
    strcat(message, "\r\nDate: ");
    strcat(message, dateTime);
    strcat(message, "\r\n\r\n");
    strcat(message, fileText);

    free(fileText);
    return message;
}
