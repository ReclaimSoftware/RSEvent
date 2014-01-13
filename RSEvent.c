#include <stdlib.h>
#include <string.h>
#include <RSEvent.h>
#include <RSVarint.h>


RSEvent* RSEvent_create(uint32_t valueCapacity) {
    RSEvent *event = calloc(sizeof(RSEvent), 1);
    if (!event) {
        return NULL;
    }
    event->valueCapacity = valueCapacity;
    event->value = malloc(valueCapacity);
    if (!event->value) {
        RSEvent_free(event);
        return NULL;
    }
    return event;
}


void RSEvent_free(RSEvent *event) {
    if (event->value) {
        free(event->value);
    }
    free(event);
}


void RSEvent_write(RSEvent *event, uint8_t *data, uint32_t *offset) {
    int64_t deltaMono = event->monotonicMicroseconds - event->lastMonotonicMicroseconds;
    int64_t deltaEpoch = event->epochMicroseconds - event->lastEpochMicroseconds;
    int64_t deltaDelta = deltaEpoch - deltaMono;

    RSVarintSignedWrite64(deltaMono, data, offset);
    RSVarintSignedWrite64(deltaDelta, data, offset);
    RSVarintWrite(event->type, data, offset);
    RSVarintWrite(event->valueSize, data, offset);

    // TODO: RSWriteData(src, size, dest, offset)
    memcpy(&(data[*offset]), event->value, event->valueSize);
    *offset = *offset + event->valueSize;

    event->lastMonotonicMicroseconds = event->monotonicMicroseconds;
    event->lastEpochMicroseconds = event->epochMicroseconds;
}


void RSEvent_writeWithCurrentTime(RSEvent *event, uint8_t *data, uint32_t *offset) {
    
}


RSEventReader* RSEventReader_create(FILE *file, uint32_t valueCapacity) {
    RSEventReader *reader = calloc(sizeof(RSEventReader), 1);
    if (!reader) {
        return NULL;
    }
    reader->event = RSEvent_create(valueCapacity);
    if (!reader->event) {
        RSEventReader_free(reader);
        return NULL;
    }
    if (!file) {
        reader->errorMessage = "file pointer is NULL";
        return reader;
    }
    reader->file = file;
    int c = fgetc(reader->file);
    if (c == EOF) {
        reader->errorMessage = "file is empty";
        return reader;
    }
    if (c != RSEventCurrentFormatVersion) {
        reader->errorMessage = "upsupported format version";
        return reader;
    }
    return reader;
}


void RSEventReader_free(RSEventReader *reader) {
    if (reader->event) {
        RSEvent_free(reader->event);
    }
    free(reader);
}


int RSEventReader_next(RSEventReader *reader) {
    if (reader->errorMessage) {
        return 0;
    }

    int64_t monotonicTimestampDelta;
    if (!RSVarintSignedFread64(reader->file, &monotonicTimestampDelta)) {
        return 0;
    }

    int64_t epochTimestampDeltadelta;
    if (!RSVarintSignedFread64(reader->file, &epochTimestampDeltadelta)) {
        reader->errorMessage = "error reading an event's epochTimestampDeltadelta";
        return 0;
    }

    uint64_t eventType;
    if (!RSVarintFread64(reader->file, &eventType)) {
        reader->errorMessage = "error reading an event's eventType";
        return 0;
    }
    if (eventType > 42949672965) {
        reader->errorMessage = "eventType is too large";
        return 0;
    }

    uint64_t valueSize;
    if (!RSVarintFread64(reader->file, &valueSize)) {
        reader->errorMessage = "error reading an event's valueSize";
        return 0;
    }
    if (eventType > 4294967295) {
        reader->errorMessage = "eventType is too large";
        return 0;
    }
    if (valueSize > reader->event->valueCapacity) {
        reader->errorMessage = "an event's valueSize exceeded your valueCapacity";
        return 0;
    }

    if (fread(reader->event->value, valueSize, 1, reader->file) != 1) {
        reader->errorMessage = "error reading an event's value";
        return 0;
    }

    reader->event->monotonicMicroseconds += monotonicTimestampDelta;
    reader->event->epochMicroseconds += monotonicTimestampDelta + epochTimestampDeltadelta;
    reader->event->type = eventType;
    reader->event->valueSize = valueSize;

    return 1;
}


void RSEventFileHeaderWrite(uint8_t *data, uint32_t *offset) {
    uint32_t i = *offset;
    data[i] = RSEventCurrentFormatVersion;
    *offset = i + 1;
}
