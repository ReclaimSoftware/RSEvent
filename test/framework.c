#include "framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


static uint64_t framework_g_num_tests = 0;
static struct timeval t0;
static struct timeval t1;


void fail(char *message) {
    fprintf(stderr, "FAIL: %s\n", message);
    exit(1);
}


void assert_ints_equal(int64_t x, int64_t y) {
    if (x != y) {
        fprintf(stderr, "x: %lld\n", (long long)x);
        fprintf(stderr, "y: %lld\n", (long long)y);
        fail("assert_ints_equal");
    }
}


void assert_uints_equal(uint64_t x, uint64_t y) {
    if (x != y) {
        fprintf(stderr, "x: %llu\n", (unsigned long long)x);
        fprintf(stderr, "y: %llu\n", (unsigned long long)y);
        fail("assert_uints_equal");
    }
}


void assert_data_equal(uint8_t *x, uint8_t *y, uint32_t size) {
    if (memcmp(x, y, size) != 0) {
        for (uint32_t i = 0; i < size; i++) {
            if (x[i] == y[i]) {
                fprintf(stderr, "%02X %02X\n", x[i], y[i]);
            } else {
                fprintf(stderr, "%02X %02X <<<\n", x[i], y[i]);
            }
        }
        fail("assert_data_equal");
    }
}


void assert_strings_equal(char *x, char *y) {
    if (!x) {
        fail("assert_strings_equal: x is null");
    }
    if (!y) {
        fail("assert_strings_equal: y is null");
    }
    if (strcmp(x, y) != 0) {
        fprintf(stderr, "x: '%s'\n", x);
        fprintf(stderr, "y: '%s'\n", y);
        fail("assert_strings_equal");
    }
}


void assert_one(int64_t x) {
    if (x != 1) {
        fprintf(stderr, "x: %lld\n", (long long)x);
        fail("assert_one");
    }
}


void assert_zero(int64_t x) {
    if (x != 0) {
        fail("assert_zero");
    }
}


void assert_null(char *x) {
    if (x) {
        fprintf(stderr, "x: '%s'\n", x);
        fail("assert_null");
    }
}


FILE* assert_fopen(char *path, char *mode) {
    FILE *file = fopen(path, mode);
    if (!file) {
        fail("assert_fopen");
    }
    return file;
}


void start_test() {
    framework_g_num_tests++;
}


void start_suite() {
    fprintf(stderr, "Running tests...\n");
    gettimeofday(&t0, NULL);
}


void pass_suite() {
    gettimeofday(&t1, NULL);
    int64_t microseconds = (
        ((t1.tv_sec - t0.tv_sec) * 1000000) +
        (t1.tv_usec - t0.tv_usec)
    );
    fprintf(stderr, "All %lld tests passed in %lld microseconds.\n",
                        (long long)framework_g_num_tests,
                        (long long)microseconds);
}
