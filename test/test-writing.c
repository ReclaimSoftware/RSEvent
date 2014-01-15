#include <RSEvent.h>
#include <RSTest.h>
#include <string.h>


void test_writing() {

    TEST("RSEventFileHeaderWrite") {
        uint8_t data[3]     = {0xAA, 0xBB, 0xCC};
        uint8_t expected[3] = {0xAA, 0x01, 0xCC};
        uint32_t offset = 1;
        RSEventFileHeaderWrite(data, &offset);
        assert_ints_equal(offset, 2);
        assert_data_equal(data, expected, 3);
    }

    TEST("create/free") {
        RSEvent_free(RSEvent_create(100));
    }

    TEST("RSEvent_write") {
        uint8_t data[17];
        data[0] = 0xEE;
        data[16] = 0xBB;
        uint32_t offset = 1;
        uint8_t expected[17] = {
            0xEE,

            0x04, // (2) monotonicMicroseconds += 2     -> 2
            0x02, // (1) epochMicroseconds += 2 + 1     -> 3
            0x07, // (7) type
            0x03, // (3) valueSize
            0x31, 0x32, 0x33, // "123"

            0x0A, // (5) monotonicMicroseconds += 5
            0x01, // (-1) epochMicroseconds += 5 + -1
            0x80, 0x01, // (128) type
            0x03, // (3) valueSize
            0x31, 0x32, 0x33, // "123"

            0xBB
        };

        RSEvent *event = RSEvent_create(100);
        event->type = 7;
        event->valueSize = 3;
        memcpy(event->value, "123", 3);

        event->monotonicMicroseconds = 2;
        event->epochMicroseconds = 3;
        RSEvent_write(event, data, &offset);

        event->monotonicMicroseconds += 5;
        event->epochMicroseconds += 4;
        event->type = 128;
        RSEvent_write(event, data, &offset);

        assert_data_equal(data, expected, 17);

        RSEvent_free(event);
    }
}
