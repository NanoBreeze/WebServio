#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <stdbool.h>


char* getSettings(); //separate linked list for 301 because contains two entries: src and destination

bool setIndex(char* fileName);
bool set404(char* fileName);
bool set301(char* src, char* dest);
bool setCaseSensitivePaths(bool);
bool setDirListings(bool);
bool setBacklog(int count);

bool setSingleEntrySetting(char* fileName, char* settingType);


#endif // SETTINGS_H_INCLUDED
