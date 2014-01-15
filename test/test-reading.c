#include <RSEvent.h>
#include <RSTest.h>

void test_reading() {
    DESCRIBE("RSEventReader") {

        IT("errors when the file pointer is null") {
            RSEventReader *reader = RSEventReader_create(NULL, 123);
            assert_zero(RSEventReader_next(reader));
            assert_strings_equal(reader->errorMessage, "file pointer is NULL");
            RSEventReader_free(reader);
        }

        IT("errors when the file is empty") {
            FILE *file = assert_fopen("fixtures/empty_file", "r");
            RSEventReader *reader = RSEventReader_create(file, 123);
            assert_zero(RSEventReader_next(reader));
            assert_strings_equal(reader->errorMessage, "file is empty");
            RSEventReader_free(reader);
            fclose(file);
        }

        IT("errors when the format version is not 1") {
            FILE *file = assert_fopen("fixtures/02", "r");
            RSEventReader *reader = RSEventReader_create(file, 123);
            assert_zero(RSEventReader_next(reader));
            assert_strings_equal(reader->errorMessage, "upsupported format version");
            RSEventReader_free(reader);
            fclose(file);
        }

        IT("reads a file with no events") {
            FILE *file = assert_fopen("fixtures/01", "r");
            RSEventReader *reader = RSEventReader_create(file, 123);
            assert_zero(RSEventReader_next(reader));
            assert_null(reader->errorMessage);
            RSEventReader_free(reader);
            fclose(file);
        }

        IT("reads some events") {
            FILE *file = assert_fopen("fixtures/some_events", "r");
            RSEventReader *reader = RSEventReader_create(file, 123);
            RSEvent *event = reader->event;
            int notDone;

            notDone = RSEventReader_next(reader);
            assert_null(reader->errorMessage);
            assert_one(notDone);
            assert_ints_equal(event->monotonicMicroseconds, 2);
            assert_ints_equal(event->epochMicroseconds, 3);
            assert_ints_equal(event->type, 7);
            assert_ints_equal(event->valueSize, 3);
            assert_data_equal(event->value, (uint8_t *)"123", 3);

            notDone = RSEventReader_next(reader);
            assert_null(reader->errorMessage);
            assert_one(notDone);
            assert_ints_equal(event->monotonicMicroseconds, 7);
            assert_ints_equal(event->epochMicroseconds, 7);
            assert_ints_equal(event->type, 128);
            assert_ints_equal(event->valueSize, 3);
            assert_data_equal(event->value, (uint8_t *)"123", 3);

            notDone = RSEventReader_next(reader);
            assert_null(reader->errorMessage);
            assert_zero(notDone);

            RSEventReader_free(reader);
            fclose(file);
        }
    }
}
