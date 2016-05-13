/*
 * Runs several tests to make sure that my_setjmp and my_longjmp
 * are working properly.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "my_setjmp.h" 
#include "c_except.h"

#define FAIL 0
#define PASS 1

void report(int);
int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();

int divide(int, int);
int my_atoi(char[]);
int get_digit(char);

int main() {

    report(test1());
    report(test2());
    report(test3());
    report(test4());
    report(test5());
    report(test6());
    report(test7());

}


/* ----TEST 1---- */

/*
 * This function tests whether a DIVIDE_BY_ZERO error is properly
 * thrown by the divide function.
 *
 */

int test1() {

    printf("[throw DIVIDE_BY_ZERO:        ");

    TRY(

        int a = 5;
        int b = 0;
        
        divide(a, b);

    )
    CATCH(DIVIDE_BY_ZERO, return PASS);
    END_TRY;

    return FAIL;

}

/* ---END TEST--- */

/* ----TEST 2---- */

/*
 * This function tests to make sure that an error is not thrown and that
 * execution proceeds normally when there are no errors.
 *
 */

int test2(void) {

    printf("[do not throw exception:      ");

    TRY(

        int a = 5;
        int b = 5;

        divide(a, b);

    )
    END_TRY;

    return PASS;

}

/* ---END TEST--- */

/* ----TEST 3---- */

/*
 * This function tests to make sure that when the error 0 is thrown,
 * my_longjmp in fact returns 1 as it is supposed to.
 *
 */

int test3(void) {

    printf("[longjmp(buf, 0) return 1:    ");

    TRY(

        char s[] = "921h8";

        int a = my_atoi(s);
        a += 5;

    )
    CATCH(UNSPECIFIED_EXCEPTION, return PASS);
    END_TRY;

    return FAIL;

}

/* ---END TEST--- */

/* ----TEST 4---- */

/*
 * This functions tests to make sure that execution proceeds normally
 * when it is several functions deep and there are no errors.
 *
 */

int test4() {

    printf("[do not throw exception across\n");
    printf(" several functions:           ");

    TRY(

        char s[] = "92108";

        int a = my_atoi(s);
        a += 5;

    )
    CATCH(UNSPECIFIED_EXCEPTION, return FAIL);
    END_TRY;

    return PASS;

}

/* ---END TEST--- */

/*
 * This function tests to make sure that my_setjmp and my_longjmp
 * work, even when execution is several "try levels" deep.
 *
 */

/* ----TEST 5---- */

int test5(void) {

    printf("[longjump correctly across\n");
    printf(" several functions:           ");

    TRY(

        char s[] = "921h8";

        int a = my_atoi(s);
        a += 5;

    )
    CATCH(UNSPECIFIED_EXCEPTION, return PASS);
    END_TRY;

    return FAIL;

}

/* ---END TEST--- */

/*
 * This function tests to make sure that when several errors are
 * possible, the correct one is returned by my_longjmp
 *
 */

/* ----TEST 6---- */

int test6(void) {

    printf("[return correct exception:    ");

    TRY(

        char a[] = "533654";
        char b[] = "98d1";

        divide(my_atoi(a), my_atoi(b));   

    )
    CATCH(DIVIDE_BY_ZERO, return FAIL);
    CATCH(UNSPECIFIED_EXCEPTION, return PASS);
    END_TRY;

    return FAIL;

}

/* ---END TEST--- */

/* ----TEST 7---- */

/*
 * This function is similar to the previous one, but with the error switched.
 *
 */

int test7(void) {

    printf("[return correct exception:    ");

    TRY(

        char a[] = "533654";
        char b[] = "0000";

        divide(my_atoi(a), my_atoi(b));   

    )
    CATCH(DIVIDE_BY_ZERO, return PASS);
    CATCH(UNSPECIFIED_EXCEPTION, return FAIL);
    END_TRY;

    return FAIL;

}

/* ---END TEST--- */








/* METHODS FOR TESTING */


/* Divides a by b, throws exception if b is 0. */
int divide(int a, int b) {

    if (b == 0) {
        THROW(DIVIDE_BY_ZERO);
    }

    return a / b;
}

/* 
 * Converts string decimal representation of a positive integer to 
 * the corresponding int value. 
 */
int my_atoi(char s[]) {

    int i = 0;
    int value = 0;

    while (s[i] != '\0') {

        TRY(

            value *= 10;
            value += get_digit(s[i]);

        )
        END_TRY;

        i++;

    }

    return value;

}

/* Converts the characters 0 through 9 into their corresponding int values. */
int get_digit(char d) {

    if (('0' <= d) && (d <= '9')) {

        return (int) (d - '0');

    }

    THROW(0);

    return -1;

}

/* END METHODS FOR TESTING */



/* Prints PASS or FAIL based on the result of the test. */
void report(int result) {

    if (result == PASS) {

        printf("PASS]\n");

    }

    else {

        printf("FAIL]\n");

    }

}
