
#include <stdio.h>
#include "minunit.h"
#include "../Parser.h"
#include "RequestLineTests.h"


static char * test_leading_whitespace() {
    char a[] = "    GET /index/name HTTP/1.1\r\n";

    RequestLine requestLine;
    int length = 0;
     mu_assert("Error: test_leading_whitespace", true == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_GET_method() {
    char a[] = "GET /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_GET_Method", true == parseRequestLine(a, &requestLine, &length));
     return 0;
 }

static char * test_POST_method() {
    char a[] = "POST /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_POST_Method", true == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_HEAD_method() {
    char a[] = "HEAD /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_HEAD_Method", true == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_unsupported_method() {
    char a[] = "TRACE /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_unsupported_method", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_excessive_whitespace_between_method_and_path() {

    char a[] = "GET  /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_excessive_whitespace_between_method_and_path", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_correct_path() {

    char a[] = "GET /index/name?wow HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_correct_path", true == parseRequestLine(a, &requestLine, &length));
     return 0;
 }

static char * test_path_encoding_accepts_ASCII_only() {

    char a[] = "GET /index/name?wow))() HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_path_encoding_accepts_ASCII_only", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_path_without_leading_slash() {

    char a[] = "GET index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_path_without_leading_slash", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_excessive_whitespace_between_path_and_version() {

    char a[] = "GET /index/name  HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_excessive_whitespace_between_path_and_version", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_version_without_HTTP() {

    char a[] = "GET /index/name HYTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_version_without_HTTP", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}


static char * test_version_with_letter_major() {

    char a[] = "GET /index/name HTTP/j.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_version_with_letter_major", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_early_CRLF() {

    char a[] = "GET /index/name\r\nHTTP/3.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_early_CRLF", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_missing_CRLF_at_end() {

    char a[] = "GET /index/name HTTP/3.1";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_missing_CRLF_at_end", false == parseRequestLine(a, &requestLine, &length));
     return 0;
}

static char * test_correct_CRLF() {

    char a[] = "GET /index/name HTTP/1.1\r\n";

    RequestLine requestLine;

    int length = 0;
     mu_assert("Error: test_missing_CRLF_at_end", true == parseRequestLine(a, &requestLine, &length));
     return 0;
}


char * run_all_request_line_parser_tests() {

     mu_run_test(test_leading_whitespace);
     mu_run_test(test_GET_method);
     mu_run_test(test_POST_method);
     mu_run_test(test_HEAD_method);
     mu_run_test(test_unsupported_method);
     mu_run_test(test_excessive_whitespace_between_method_and_path);
     mu_run_test(test_correct_path);
     mu_run_test(test_path_encoding_accepts_ASCII_only);
     mu_run_test(test_path_without_leading_slash);
     mu_run_test(test_excessive_whitespace_between_path_and_version);

     mu_run_test(test_version_without_HTTP);
     mu_run_test(test_version_with_letter_major);
     mu_run_test(test_early_CRLF);
     mu_run_test(test_missing_CRLF_at_end);
     mu_run_test(test_correct_CRLF);

     return 0;

}

