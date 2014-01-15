#include <RSTest.h>

void test_reading();
void test_writing();

int main(int argc, char **argv) {
    start_suite();
    test_reading();
    test_writing();
    end_suite();
    return 0;
}
