
#include "Parser.h"
#include <string.h>

/*
typedef enum { GET, POST, HEAD } METHOD;

typedef struct {

	METHOD method;
	char* path;
	int major;
	int minor;

} RequestLine;

RequestLine parseRequestLine(const char[]);


For robustness, let all the CRLF go if they appear before the request line. Here, we simply remove all eading whitespace
Request line begins with a method (eg, GET), then a single space, then the path, then another single space, then the protocol version, and ends with CRLF

*/

bool parseRequestLine(char* start, RequestLine* requestLine) {

	while (isspace(*start)) { start++; }

//hmmmm, this is an interesting style, using returns, will check if there are better alternatives for readability though.

	if (!getMethod(&start, &(requestLine->method))) { return false; }

	if (!consumeSP(&start)) { return false; }

	if (!getPath(&start, &(requestLine->path))) { return false; }

	if (!consumeSP(&start)) { return false; }

	if (!getProtocolVersion(&start, &(requestLine->major), &(requestLine->minor))) { return false ; } //is this passing by value? Need to pass by ref

	if (!consumeCRLF(&start)) { return false; }

	return true;
}

//methods are case sensitive
bool getMethod(char** start, METHOD* method) {

	int MAX_METHOD_LENGTH = 7; //CONNECT
	char met[MAX_METHOD_LENGTH + 1];
	int posMet = 0;

	while (**start != ' ') {
		met[posMet] = **start;
		(*start)++;
		posMet++;
	}

	met[posMet] = '\0';

	//valid methods currently accepted are GET, HEAD, POST
	if (strcmp(met, "GET") == 0) {
		*method = GET; //I think this is illegal syntax
	}
	else if (strcmp(met, "HEAD") == 0) {
		*method = HEAD;
	}
	else if (strcmp(met, "POST") == 0) {
		*method = POST;
	}
	else {
		return false;
	}

	return true;

}


bool consumeSP(char** start) {
    if (**start == ' ') {
        (*start)++;
        return true;
    }
    return false;
}


//eg, /index, /hello/heythere
bool getPath(char** start, char** path) {
	if (**start != '/') { return false; }

	char pathBuffer[8000];
	int posPath = 0;

	while (**start != ' ') {
		pathBuffer[posPath] = **start;
		(*start)++;
		posPath++;
	}

	pathBuffer[posPath] = '\0';
	*path = pathBuffer; //TODO: are we giving all 8000 bytes?

	return true;
}


bool getProtocolVersion(char** start, int* major, int* minor) { //TODO: passing by value tho!

	if (strncmp(*start, "HTTP/", 5)) { //precondition
		return false;
	}

	*start += 5; //so if start we initially at the 'H' in "HTTP/1.1", we would now be on the 1

	//we don't use atoi because we're comparing only a single character
	*major = *(*start)++ - '0';
	if (*major < 0 || *major > 9) { return false; }

	if (*(*start)++ != '.') { return false;}

	*minor = *(*start)++ - '0';
	if (*minor < 0 || *minor > 9) { return false; }

	return true;
}

bool consumeCRLF(char** start) {
	if (**start == '\r' && *(*start+1) == '\n') {
		*start += 2;
		return true;
	}

	return false;
}


////////////////////////////// PARSING headerField
bool parseHeaderField(char* start, HeaderField* headerField) {

    if (!parseName(&start, headerField->name)) { return false; } //if the name already exists, return 400
    if (!consumeColon(&start)) { return false; }
    if (!parseValue(&start, headerField->value)) { return false; }
    if (!consumeCRLF(&start)) { return false; }

    return true;
}

//assume the only invalid characters in header name is whitespace. I know, this is against the ABNF rules in RFC 7230
//also assumed header fields have 200 characters max
bool parseName(char** start, char* name) {

    if (isspace(**start)) { return false; }

    const int MAX_HEADER_NAME_SIZE = 200;
    char tempName[MAX_HEADER_NAME_SIZE];
    //char b[200];
    int pos = 0;

    while (**start != ':') {
        if (isspace(**start) || pos > MAX_HEADER_NAME_SIZE) {
            break;
        }

        tempName[pos++] = **start;
        (*start)++;
    }

    if (**start == ':') {
        tempName[pos] = '\0';
        strcpy(name, tempName);
        //*name = tempName;   //the whole 200 bytes??
        return true;
    }

    return false;
}

bool consumeColon(char** start) {
    return *(*start)++ == ':';
}

//assume the only invalid characters in header name is whitespace. I know, this is against the ABNF rules in RFC 7230
//also assumed header fields have 200 characters max
bool parseValue(char** start, char* value) {

    while (isspace(**start)) { (*start)++; }

    const int MAX_HEADER_VALUE_SIZE = 205;
    char tempValue[MAX_HEADER_VALUE_SIZE];
    int pos = 0;

    while (**start != '\r') {
        if (isspace(**start) || pos > MAX_HEADER_VALUE_SIZE || **start == '\0') { //check for null terminator in case missing \r and to prevent keep going to end
            break;
        }

        tempValue[pos++] = **start;
        (*start)++;
    }

    if (**start == '\r') {
        tempValue[pos] = '\0';
        strcpy(value, tempValue);

//        *value = tempValue;
        return true;
    }

    return false;
}





