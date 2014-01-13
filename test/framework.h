#ifndef __framework_h__
#define __framework_h__


#include <stdint.h>
#include <stdio.h>


#define TEST(x)         start_test();
#define IT(x)           start_test();
#define DESCRIBE(x)     /**/


void fail(char *message);
void assert_ints_equal(int64_t x, int64_t y);
void assert_uints_equal(uint64_t x, uint64_t y);
void assert_data_equal(uint8_t *x, uint8_t *y, uint32_t size);
void assert_strings_equal(char *x, char *y);
void assert_one(int64_t x);
void assert_zero(int64_t x);
void assert_null(char *x);
FILE* assert_fopen(char *path, char *mode);

void start_test();
void start_suite();
void pass_suite();


#endif
