#ifndef MINUNIT_H_INCLUDED
#define MINUNIT_H_INCLUDED

//from jera.com/techinfo/jtns/jtn002.html
//yep, 3 lines for the test library

 int tests_run;

 #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
 #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)




#endif // MINUNIT_H_INCLUDED
