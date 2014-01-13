#include "framework.h"

void test_reading();
void test_writing();

int main(int argc, char **argv) {
    start_suite();
    test_reading();
    test_writing();
    pass_suite();
    return 0;
}
