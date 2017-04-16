#include "MessageFactory.h"
#include "Lib.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "dirent.h"


/**
This file requires refactoring, not DRY, many duplications
**/

char* create200Message(char* file) {

    char* statusLine = "HTTP/1.1 200 OK\r\n";

    char* response = createMessageWithBodyFromFile(file, statusLine);
    if (!response) { return NULL; }

    return response;

}

char* create301Message(char* file) {

    char* statusLine = "HTTP/1.1 301 Moved Permanently\r\n";

    char* response = createMessageWithBodyFromFile(file, statusLine);
    if (!response) { return NULL; }

    return response;
}


char* create404Message(char* file) {

    char* statusLine = "HTTP/1.1 404 Not Found\r\n";

    char* response = createMessageWithBodyFromFile(file, statusLine);
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


char* create200MessageDir(char* dirPath) {
    //why not just allocate 100000 bytes. That's surely enough and we're not aiming for optimization? Because I want to be more optimal for the challenge.

    size_t totalMemoryAllocated = sizeof(char) * (strlen("<h1>Directory: ") + strlen(dirPath) + strlen("</h1><br>") + 1);

    char* message = (char*) malloc(totalMemoryAllocated);
    if (!message) { return NULL; }

    size_t memoryUsed = totalMemoryAllocated;
    sprintf(message, "<h1>Directory: %s</h1><br>", dirPath);

    DIR *d = NULL;
    struct dirent *dir = NULL;

    d = opendir(dirPath);
    if (d) {

        while ((dir = readdir(d)) != NULL) {
          //  <p><a href="dirName">dirName</a></p>
            char* dirName = dir->d_name;
            size_t dirNameLength = strlen(dir->d_name);

            if (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0) {
                continue;
            }

            char line[2 * dirNameLength + strlen("<p><a href=\"\"></a></p>")];

            while (memoryUsed + sizeof(line) + 1 > totalMemoryAllocated) { //use while instead of if in case dirName is super large and doubling memory isn't enough
                totalMemoryAllocated *= 2;
                message = (char*) realloc(message, totalMemoryAllocated);

                if (!message) { return NULL; }
            }

            sprintf(line, "<p><a href=\"%s\">%s</a></p>", dirName, dirName);

            strcat(message, line);
            memoryUsed += sizeof(line);
        }
    }
    closedir(d);

    char* statusLine = "HTTP/1.1 200 OK\r\n";

    char* response = createMessageWithSuppliedBody(message, statusLine);
    if (!response) { return NULL; }

    free(message);

    return response;

}


char* createMessageWithBodyFromFile(char* file, char* statusLine) {

    char* fileText = getFileText(file);
    if (!fileText) { return NULL; }

    char* message = createMessageWithSuppliedBody(fileText, statusLine);
    if (!message) { return NULL; }

    free(fileText);

    return message;
}

char* createMessageWithSuppliedBody(char* bodyText, char* statusLine) {

    if (!bodyText) { return NULL; }

    char* dateTime = getCurrentDateTime();
    char* hardCodedHeaders =
        "Server: WebServio\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n";

    int lengthOfBodyTextLength = 0;
    if (strlen(bodyText) > 0) {
        lengthOfBodyTextLength = floor(log10(strlen(bodyText))) + 1;
    }

    char* message = (char*) malloc(strlen(statusLine) + strlen(hardCodedHeaders) + strlen("Content-length: ") + lengthOfBodyTextLength + strlen("\r\nDate: ") +
        strlen(dateTime) + strlen("\r\n\r\n") + strlen(bodyText) + 1);

    if (!message) { return NULL; }

    char sLengthOfBodyText[lengthOfBodyTextLength];
    sprintf(sLengthOfBodyText, "%d", strlen(bodyText));

    strcpy(message, statusLine);
    strcat(message, hardCodedHeaders);
    strcat(message, "Content-length: ");
    strcat(message, sLengthOfBodyText);
    strcat(message, "\r\nDate: ");
    strcat(message, dateTime);
    strcat(message, "\r\n\r\n");
    strcat(message, bodyText);

    return message;
}
