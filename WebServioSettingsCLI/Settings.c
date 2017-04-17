#include <string.h>

#include "Settings.h"


char* getSettings() {
    return getFileText("settings.conf");
}


bool setIndex(char* fileName) {
    return setSingleEntrySetting(fileName, "Index");
}


bool set404(char* fileName) {
    return setSingleEntrySetting(fileName, "404");
}


//Requires a bit of refactoring, a little difficult to read
bool set301(char* src, char* dest) {
    /**find "301" (make sure this is indeed a setting type and not some chars in a file name by checking there's a space after it
    //Check whether the source in that line matches the supplied source
    //if match, delete that line
    //if not, find next "301" and repeat until no more
    //add supplied src and destination at end
**/

    char* settingsText = getFileText("settings.conf");

    char* ptr = strstr(settingsText, "301");

    int origSettingsTextLength = strlen(settingsText); //when we shift, the size of settingsText changes. Technically, we don't really need this but clearer to use it
    int origLineLength = 0;
    int newLineLength = strlen("301 ") + strlen(src) + strlen(" ") + strlen(dest) + strlen("\n");


    while (ptr) {

        if (*(ptr + 3) == ' ') { //possible SEG fault if ptr+3 is out of bounds. Hardcoded, consider changing
            char* curSrcStart = ptr + 4;
            char* curSrcDestDelimiter = strchr(curSrcStart, ' ');

            size_t curSrcLength = (curSrcDestDelimiter - curSrcStart) / sizeof(char);

            char* curSrc = (char*) malloc((curSrcLength + 1)* sizeof(char));

            strncpy(curSrc, curSrcStart, curSrcLength);
            curSrc[curSrcLength] = '\0';

            if (strcmp(src, curSrc) == 0) { //the source already exists

                char* newLinePtr = strchr(ptr, '\n');

                origLineLength = (newLinePtr - ptr) / sizeof(char) + 1;

                shiftCharsToCoverLine(&settingsText, &ptr, &newLinePtr);
                break;
            }
        }

        ptr = strstr(ptr + 1, "301");
    }


    //the 301 doesn't exist
    int diff = newLineLength - origLineLength;

    //currently, this realloc is allocating a bit more memory than needed. Allocating the difference between old and new is a nice optimization
    settingsText = realloc(settingsText, sizeof(char) * (origSettingsTextLength + diff + 1));
    if (!settingsText) { return false; }

    sprintf(settingsText, "%s301 %s %s\n", settingsText, src, dest);

    return writeFile("settings.conf", settingsText);
}


bool setCaseSensitivePaths(bool yes) {

    if (yes) {
        return setSingleEntrySetting("true", "CaseSensitivePaths");
    }

    return setSingleEntrySetting("false", "CaseSensitivePaths");
}


bool setDirListings(bool yes) {

    if (yes) {
        return setSingleEntrySetting("true", "DirListings");
    }

    return setSingleEntrySetting("false", "DirListings");
}


bool setBacklog(int count) {

    char sInt[sizeof(int) * 4 + 1];
    sprintf(sInt, "%d", count);

    return setSingleEntrySetting(sInt, "Backlog");
}

//I feel the logic needs to be worked out here
bool setSingleEntrySetting(char* value, char* settingType) { //eg of settingtype, 404, Index, DirListings

    char* settingsText = getFileText("settings.conf");
    if (!settingsText) { return false; }

    char* ptr = strstr(settingsText, settingType);

    int origSettingsTextLength = strlen(settingsText); //when we shift, the size of settingsText changes. Technically, we don't really need this but clearer to use it
    int origLineLength = 0;
    int newLineLength = strlen(settingType) + strlen(" ") + strlen(value) + strlen("\n");


    if (ptr) {
        //basiclaly, delete the line and create a new one at end. This is easier than inserting characters in the middle of the index line
        char* newLinePtr = strchr(ptr, '\n');

        origLineLength = (newLinePtr - ptr) / sizeof(char) + 1;

        shiftCharsToCoverLine(&settingsText, &ptr, &newLinePtr);
    }

    int diff = newLineLength - origLineLength;

    settingsText = realloc(settingsText, sizeof(char) * (origSettingsTextLength + diff + 1));
    if (!settingsText) { return false; }

    sprintf(settingsText, "%s%s %s\n", settingsText, settingType, value); //yes, mind the space

    return writeFile("settings.conf", settingsText);
}


void shiftCharsToCoverLine(char** settingsText, char** dest, char** newLinePtr) { //use single * is enough already

    int remaining = strlen(*settingsText) - (*newLinePtr - *settingsText) / sizeof(char);

    memmove(*dest, (*newLinePtr + 1), remaining * sizeof(char)); //shift rest of characters to "cover over" the current index line
}
