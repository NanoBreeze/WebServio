#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdbool.h>

#include "LinkedList.h"

typedef enum { GET, POST, HEAD } METHOD;

typedef struct requestLine {

	METHOD method;
	char* path;
	int major;
	int minor;
} RequestLine;

typedef struct headerField {
    char name[200]; //because when a pointer, we use the temp buffer as an array, and when assigning the temp buffer for name and value end up at the same address so overwrite, and to avoid dynamic memory management
    char value[200];
} HeaderField;

bool parseRequestLine(char* start, RequestLine* requestLine, int* requestLineLength);
bool getMethod(char** start, METHOD* method);
bool consumeSP(char** start);
bool getPath(char** start, char** path);
bool getProtocolVersion(char** start, int* major, int* minor);
bool consumeCRLF(char** start);

bool parseHeader(char* start, LinkedList* headerFields, int* headerLength);
bool parseHeaderField(char** start, HeaderField* headerField);
bool parseName(char** start, char* name);
bool consumeColon(char** start);
bool parseValue(char** start, char* value);

void copyMessage(char* start, int length, char* message);


bool parseSettings(char* start, LinkedList* settings, LinkedList* settings301); //separate linked list for 301 because contains two entries: src and destination
bool parseIndex(char** start, LinkedList* settings);
bool parse404(char** start, LinkedList* settings);
bool parse301(char** start, LinkedList* settings);
bool parseCaseSensitivePaths(char** start, LinkedList* settings);
bool parseBacklog(char** start, LinkedList* settings);
bool parseDirListings(char** start, LinkedList* settings);


bool skipToLF(char** start);
char* getTextToLF(char* start);
bool consumeLF(char** start);


#endif // PARSER_H_INCLUDED
