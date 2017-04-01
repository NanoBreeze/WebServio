#ifndef REQUESTLINETESTS_H_INCLUDED
#define REQUESTLINETESTS_H_INCLUDED


static char * test_leading_whitespace();
static char * test_GET_method();
static char * test_POST_method();
static char * test_HEAD_method() ;
static char * test_unsupported_method() ;
static char * test_excessive_whitespace_between_method_and_path() ;
static char * test_correct_path() ;
static char * test_path_encoding_accepts_ASCII_only();
static char * test_path_without_leading_slash() ;
static char * test_excessive_whitespace_between_path_and_version() ;
static char * test_version_without_HTTP() ;
static char * test_version_with_letter_major() ;
static char * test_early_CRLF() ;
static char * test_missing_CRLF_at_end() ;
static char * test_correct_CRLF() ;


char* run_all_request_line_parser_tests();


#endif // REQUESTLINETESTS_H_INCLUDED
