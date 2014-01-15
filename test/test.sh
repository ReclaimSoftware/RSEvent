mkdir -p ../build

$CC \
        -std=c99 \
        -Wall \
        -o ../build/runtests \
        -isystem ../ \
        -isystem ../upstream/RSTest \
        -isystem ../upstream/RSVarint \
        ../upstream/RSTest/RSTest.c \
        ../upstream/RSVarint/RSVarint.c \
        ../RSEvent.c \
        test-reading.c \
        test-writing.c \
        runtests.c &&
    ../build/runtests
