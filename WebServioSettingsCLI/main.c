#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "Settings.h"


/* Review writing parsers from BNF. This current one is confusing to follow. :( */
int main()
{
    setIndex("newIndex.html");
    size_t bufLength = 100;
    char* buffer = (char*) malloc(bufLength * sizeof(char));

    if (!buffer) {
        printf("Allocation error in creating buffer\n");
        return;
    }

    printf("Welcome to WebServio settings. Available commands are 'set', 'show' and 'help'.\n>>> ");

    for (;;) {

        int inputLength = getline(&buffer, &bufLength, stdin);

        if(inputLength == 4 && strncmp(buffer, "set", 3) == 0) {
            printf("Valid settings to set are: Index, 404, 301, DirListings, CaseSensitivePaths, and Backlog. Type 'help' for more information.\n>>> ");
            continue;
        }
        else if (inputLength > 4) {

            if (strncmp(buffer, "set", 3) == 0 && *(buffer + 3) == ' ') {

                if (inputLength < 8) {
                    printf("Valid settings to set are: Index, 404, 301, DirListings, CaseSensitivePaths, and Backlog. Type 'help' for more information.\n>>> ");
                    continue;
                }

                char* spaceDelimiter1 = strchr(buffer + 4, ' ');

                if (!spaceDelimiter1) {
                    printf("Valid settings to set are: Index, 404, 301, DirListings, CaseSensitivePaths, and Backlog. Type 'help' for more information.\n>>> ");
                    continue;
                }

                char* spaceDelimiter2 = strchr(spaceDelimiter1 + 1, ' '); //should be NULL if not 301


                //get setting type, eg, Index, 404
                char settingType[20]; //char[] larger than is necessary
                strncpy(settingType, buffer + 4, (spaceDelimiter1 - (buffer+4)) /sizeof(char));
                settingType[spaceDelimiter1 - (buffer+4)] = '\0';

                if (strcmp(settingType, "301") == 0) { //one space after 301 and one space after the source
                    if (!spaceDelimiter2) {
                        printf("Please include source and destination files.\n>>> ");
                        continue;
                    }

                    if (strchr(spaceDelimiter2 + 1, ' ')) { //should be NULL
                        printf("There are too many spaces for this command.\n>>> ");
                        continue;
                    }

                    char source[80]; //true/false/number/filename shouldn't exceed 79 chars
                    strncpy(source, spaceDelimiter1 + 1, spaceDelimiter2 - (spaceDelimiter1 + 1));
                    source[spaceDelimiter2 - (spaceDelimiter1+1)] = '\0';

                    char dest[80]; //true/false/number/filename shouldn't exceed 79 chars
                    strncpy(dest, spaceDelimiter2 + 1, inputLength - (spaceDelimiter2 - buffer + 1) - 1); //minus one more to remove then \n
                    dest[inputLength - (spaceDelimiter2 - buffer + 1) - 1] = '\0';

                    if (set301(source, dest)) {
                        printf("A '301' setting has been set.\n>>> ");
                        continue;
                    }
                    else {
                        printf("Could not set '301' setting.\n>>> ");
                        continue;
                    }
                }
                else { //only one space after setting type

                    if (strcmp(settingType, "Index") == 0 ||
                        strcmp(settingType, "Backlog") == 0 ||
                        strcmp(settingType, "DirListings") == 0 ||
                        strcmp(settingType, "CaseSensitivePaths") == 0 ||
                        strcmp(settingType, "404") == 0)  {

                        if (spaceDelimiter2) {
                            printf("There are too many spaces for this command.\n>>> ");
                            continue;
                        }

                        char value[80]; //true/false/number/filename shouldn't exceed 79 chars
                        strncpy(value, spaceDelimiter1 + 1,  inputLength - (spaceDelimiter1 - buffer + 1) - 1);
                        value[inputLength - (spaceDelimiter1 - buffer + 1) - 1] = '\0';

                        if (strcmp(settingType, "Index") == 0) {
                            if (setIndex(value)) {
                                printf("The 'Index' page has been set.\n>>> ");
                                continue;
                            }
                            else {
                                printf("Could not set 'Index' page.\n>>> ");
                                continue;
                            }
                        }
                        else if (strcmp(settingType, "404") == 0) {
                            if (set404(value)) {
                                printf("The '404' page has been set.\n>>> ");
                                continue;
                            }
                            else {
                                printf("Could not set '404' page.\n>>> ");
                                continue;
                            }
                        }
                        else if (strcmp(settingType, "Backlog") == 0) {

                            //validate that the value truly was an integer
                            char* end;
                            long int result = strtol(value, &end, 10);

                            if (result == LONG_MIN && errno != 0) {
                                printf("Underflow problem.\n>>> ");
                                continue;
                            }
                            if (result == LONG_MAX && errno != 0) {
                                printf("Overflow problem.\n>>> ");
                                continue;
                            }
                            if (*end != '\0' || atoi(value) < 1) {
                                printf("Make sure to specify a positive integer.\n>>> ");
                                continue;
                            }

                            if (setBacklog(atoi(value))) {
                                printf("The 'Backlog' has been set.\n>>> ");
                                continue;
                            }
                            else {
                                printf("Could not set 'Backlog' setting.\n>>> ");
                                continue;
                            }
                        }
                        else if(strcmp(settingType, "DirListings") == 0) {

                            bool yesNo;
                            if (strcmp(value, "true") == 0) {
                                yesNo = true;
                            }
                            else if (strcmp(value, "false") == 0) {
                                yesNo = false;
                            }
                            else {
                                printf("Valid parameters are: true/false.\n>>> ");
                                continue;
                            }

                            if (setDirListings(yesNo)) {
                                printf("The 'DirListings' page has been set.\n>>> ");
                                continue;
                            }
                            else {
                                printf("Could not set 'DirListings' page.\n>>> ");
                                continue;
                            }
                        }
                        else { // (strcmp(settingType, "CaseSensitivePaths")

                            bool yesNo;
                            if (strcmp(value, "true") == 0) {
                                yesNo = true;
                            }
                            else if (strcmp(value, "false") == 0) {
                                yesNo = false;
                            }
                            else {
                                printf("Valid parameters are: true/false.\n>>> ");
                                continue;
                            }

                            if (setCaseSensitivePaths(yesNo)) {
                                printf("The 'CaseSensitivePaths' setting has been set.\n>>> ");
                                continue;
                            }
                            else {
                                printf("Could not set 'CaseSensitivePaths' setting.\n>>> ");
                                continue;
                            }
                        }
                    }
                    else {
                        printf("Valid settings to set are: 'Index', '404', '301', 'DirListings', 'CaseSensitivePaths', and 'Backlog'.\n>>> ");
                        continue;
                    }
                }
            }
            else if (inputLength == 5) {
                if (strncmp(buffer, "show", 4) == 0) {
                    char* settings = getSettings();

                    if (!settings) {
                        printf("An error occured and the settings cannot be read.\n>>> ");
                        continue;
                    }

                    printf("%s\n>>> ", settings);
                    continue;

                }
                else if (strncmp(buffer, "help", 4) == 0) {
                    printf("Type 'show' to display current settings.\n"
                    "Type 'set' followed by one of the following setting type and their respective parameters to set a setting:\n"
                    "set Index [indexPage], sets the index page.\n"
                    "set 404 [404Page], sets the 404 page.\n"
                    "set Backlog [positive integer], sets max number of connections in backlog queue.\n"
                    "set DirListings ['true'|'false'], sets whether a GET request to a directory shows the files in the directory (true) or a 404 page (false).\n"
                    "set CaseSensitivePaths ['true'|'false'], sets whether paths are case-sensitive or not.\n"
                    "set 301 [source] [destination], sets redirect pages.\n>>> ");
                    continue;

                }
                else {
                    printf("Invalid command. Type 'help' for valid commands.\n>>> ");
                    continue;
                }
            }
            else {
                printf("Invalid command. Type 'help' for valid commands.\n>>> ");
                continue;
            }
        }
        else {
            printf("Invalid command. Type 'help' for valid commands.\n>>> ");
            continue;
        }
    }

    return 0;
}
