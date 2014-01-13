**An efficient codec for a stream of `(type, value, t_epoch, t_mono)` events**


## RSEvent
```c
typedef struct {
    int64_t monotonicMicroseconds;
    int64_t epochMicroseconds;
    uint32_t type;
    uint32_t valueSize;
    uint8_t *value;
} RSEvent;
```


## Writing events
```c
uint8_t *data = ...;
uint32_t offset = 0;
RSEvent *event = RSEvent_create(valueCapacity);
if (!event) {
    // error
}
RSEventFileHeaderWrite(data, &offset);

event->type = 123;
event->value[0] = 0xFF;
event->valueSize = 1;
RSEvent_writeWithCurrentTime(event, data, &offset);

RSEvent_free(event);
if (fwrite(data, offset, 1, file) != 1) {
    // error
}
```


## Reading events
```c
RSEventReader *reader = RSEventReader_create(file, valueCapacity);
if (!reader) {
  // ...
}
RSEvent *event = reader.event;
while(RSEventReader_next()) {
  switch (event->type) {
    // ...
  }
}
if (reader->errorMessage) {
  // ...
}
RSEventReader_free(event);
```


## Event File Format

    file header + zero or more events


### File Header

    file_format    : varint (0x01)


### Event

    monotonic_timestamp_delta   : signed varint
    epoch_timestamp_deltadelta  : signed varint
    event_type                  : varint
    value_size                  : varint
    value                       : bytes


#### Decoding the timestamps

    monotonic = 0
    epoch = 0
    loop:
      monotonic += monotonic_delta
      epoch += monotonic_delta + epoch_timestamp_deltadelta


### [License: MIT](LICENSE.txt)
