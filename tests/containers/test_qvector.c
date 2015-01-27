#include "../qunit.h"
#include "qlibc.h"
#include <string.h>

/*
To test:

static bool add(qvector_t *vector, const void *object, size_t size);
static bool addstr(qvector_t *vector, const char *str);
static bool addstrf(qvector_t *vector, const char *format, ...);
static void *toarray(qvector_t *vector, size_t *size);
static char *tostring(qvector_t *vector);
static size_t size(qvector_t *vector);
static size_t datasize(qvector_t *vector);
static void clear(qvector_t *vector);
static bool debug(qvector_t *vector, FILE *out);
static void free_(qvector_t *vector);
*/

QUNIT_START("Test qvector.c");

TEST("add()") {
    int testdata[] = { 100, 500, 1000, 10000 };
    int totalofdata = 0;
    int finaltotal = 0;

    qvector_t *vector = qvector(0);

    for(int i = 0; i < (sizeof testdata / sizeof *testdata); i++) {
        totalofdata += testdata[i];
        vector->add(vector, (void *)&testdata[i], sizeof(int));
    }

    // get the chunk as a string
    int *final = vector->toarray(vector, NULL);
    for(int i = 0; i < sizeof(final); i++) {
        finaltotal += final[i];
    }

    ASSERT_EQUAL_INT(finaltotal, totalofdata);

    // release
    free(final);
    vector->free(vector);
}

TEST("addstr()") {
    const char *testdata[] = { "ABCDEFG", "1234567890", "~!@#$%^&*()_+{}|:\"<>?", "`-=[]';/.," };

    for(int i = 0; i < (sizeof testdata / sizeof *testdata); i++){
        qvector_t *vector = qvector(0);
        vector->addstr(vector, testdata[i]);

        // get the chunk as a string
        char *final = vector->tostring(vector);

        // printf("Str: %s\n", final);
        ASSERT_EQUAL_STR(final, testdata[i]);

        // release
        free(final);
        vector->free(vector);
    }
}

QUNIT_END();