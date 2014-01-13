mkdir -p ../build

$CC \
        -std=c99 \
        -Wall \
        -o ../build/runtests \
        -isystem ../ \
        -isystem ../upstream/RSVarint \
        ../upstream/RSVarint/RSVarint.c \
        ../RSEvent.c \
        framework.c \
        test-reading.c \
        test-writing.c \
        runtests.c &&
    ../build/runtests
