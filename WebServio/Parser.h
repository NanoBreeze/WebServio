#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdbool.h>

typedef enum { GET, POST, HEAD } METHOD;

typedef struct requestLine {

	METHOD method;
	char* path;
	int major;
	int minor;
	int length;

} RequestLine;

bool parseRequestLine(char* start, RequestLine* requestLine);
bool getMethod(char** start, METHOD* method);
bool consumeSP(char** start);
bool getPath(char** start, char** path);
bool getProtocolVersion(char** start, int* major, int* minor);
bool consumeCRLF(char** start);

int parseHeaders(const char[]);


#endif // PARSER_H_INCLUDED
