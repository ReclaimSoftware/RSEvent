#ifndef __RSEvent_h__
#define __RSEvent_h__

#include <stdint.h>
#include <stdio.h>


static const uint8_t RSEventCurrentFormatVersion = 1;


typedef struct {
    int64_t monotonicMicroseconds;
    int64_t epochMicroseconds;
    uint32_t type;
    uint32_t valueSize;
    uint8_t *value;

    // private
    uint32_t valueCapacity;
    int64_t lastMonotonicMicroseconds;
    int64_t lastEpochMicroseconds;
} RSEvent;

typedef struct {
    FILE *file;
    RSEvent *event;
    uint64_t num_events_read;
    char *errorMessage;
} RSEventReader;


RSEvent* RSEvent_create(uint32_t valueCapacity);
void RSEvent_free(RSEvent *event);
void RSEvent_write(RSEvent *event, uint8_t *data, uint32_t *offset);
void RSEvent_writeWithCurrentTime(RSEvent *event, uint8_t *data, uint32_t *offset);

RSEventReader* RSEventReader_create(FILE *file, uint32_t valueCapacity);
void RSEventReader_free(RSEventReader *reader);
int RSEventReader_next(RSEventReader *reader);

void RSEventFileHeaderWrite(uint8_t *data, uint32_t *offset);

#endif
