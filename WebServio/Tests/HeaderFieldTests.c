
#include "HeaderFieldTests.h"
#include "minunit.h"
#include "../Parser.h"



static char * test_normal() {
    char strHeaderField[] = "Host:images-na.ssl-images-amazon.com\r\n";

    HeaderField headerField;

    mu_assert("Error: test_normal", true == parseHeaderField(strHeaderField, &headerField)
        && strcmp(headerField.name, "Host") == 0 && strcmp(headerField.value, "images-na.ssl-images-amazon.com") == 0);
    return 0;
}


static char * test_leading_whitespace() {
    char strHeaderField[] = "   Host:images-na.ssl-images-amazon.com\r\n";

    HeaderField headerField;

    mu_assert("Error: test_leading_whitespace", false == parseHeaderField(strHeaderField, &headerField));
    return 0;
}


static char * test_whitespace_immediately_before_colon() {
    char strHeaderField[] = "Host :images-na.ssl-images-amazon.com\r\n";

    HeaderField headerField;

    mu_assert("Error: test_whitespace_immediately_before_colon", false == parseHeaderField(strHeaderField, &headerField));
    return 0;
}


static char * test_whitespace_immediately_after_colon() {
    char strHeaderField[] = "Host:     images-na.ssl-images-amazon.com\r\n";

    HeaderField headerField;

    mu_assert("Error: test_whitespace_immediately_after_colon", true == parseHeaderField(strHeaderField, &headerField)
        && strcmp(headerField.name, "Host") == 0 && strcmp(headerField.value, "images-na.ssl-images-amazon.com") == 0);
    return 0;
}


static char * test_whitespace_in_value() {
    char strHeaderField[] = "Host:images-na.ssl-images-amazon.   com\r\n";

    HeaderField headerField;

    mu_assert("Error: test_whitespace_in_value", false == parseHeaderField(strHeaderField, &headerField));
    return 0;
}


static char * test_missing_CRLF_at_end() {
    char strHeaderField[] = "Host:images-na.ssl-images-amazon.com";

    HeaderField headerField;

    mu_assert("Error: test_missing_CRLF_at_end", false == parseHeaderField(strHeaderField, &headerField));
    return 0;
}


static char * test_CRLF_in__middle_of_value() { return 0; } //this will be caught in the next header value


static char * test_super_long_value() {
    char strHeaderField[] = "Host:images-na.ssl-images-amazon.comoiwjefoiwjefoijwefoijwefoiwejfoijwefiowejfoijwofijweotiweoitweoitoweijtoiwejroiwejrloweijrfolwejfaiojflowejiafliawoejtolwaehtwaeuiothlowehflowaehfloweahflowajietlowaejtloawehtioawehtloiwehtolawehtolawihetowehtloweaihtloawhtloawehtloawehtloawhtweiohtgoiowehtgoiehwngoihwesoigewoignoewgnioweongoweiwelginowegnowengowengoewing";

    HeaderField headerField;

    mu_assert("Error: test_super_long_value", false == parseHeaderField(strHeaderField, &headerField));
    return 0;
}



char * run_all_header_field_tests() {

     mu_run_test(test_normal);
     mu_run_test(test_leading_whitespace);
     mu_run_test(test_whitespace_immediately_before_colon);
     mu_run_test(test_whitespace_immediately_after_colon);
     mu_run_test(test_whitespace_in_value);
     mu_run_test(test_missing_CRLF_at_end);
     mu_run_test(test_super_long_value);

     return 0;
}
