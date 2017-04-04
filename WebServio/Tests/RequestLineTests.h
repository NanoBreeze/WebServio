#ifndef REQUESTLINETESTS_H_INCLUDED
#define REQUESTLINETESTS_H_INCLUDED

/*
char * test_leading_whitespace();
char * test_GET_method();
char * test_POST_method();

char * test_HEAD_method() ;
char * test_unsupported_method() ;
char * test_excessive_whitespace_between_method_and_path() ;
char * test_correct_path() ;
char * test_path_encoding_accepts_ASCII_only();
char * test_path_without_leading_slash() ;
char * test_excessive_whitespace_between_path_and_version() ;
char * test_version_without_HTTP() ;
char * test_version_with_letter_major() ;
char * test_early_CRLF() ;
char * test_missing_CRLF_at_end() ;
char * test_correct_CRLF() ;

*/

char * run_all_request_line_parser_tests();


#endif // REQUESTLINETESTS_H_INCLUDED
