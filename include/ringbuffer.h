#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include<stdio.h>

typedef struct{
    double *data;
    size_t capacity; //buffer size
    size_t count;
    size_t head;

}RingBuffer;

// Set up a buffer with the given capacity. Returns 0 on success, -1 on failure.
int rb_init(RingBuffer *rb, size_t capacity);

// Add a value. If the buffer is full, overwrites the oldest value.
void rb_push(RingBuffer *rb, double value);

// Read the value at logical position `index` (0 = oldest, count-1 = newest).
double rb_at(const RingBuffer *rb, size_t index);

// How many values are currently stored.
size_t rb_count(const RingBuffer *rb);

// Free the buffer's memory.
void rb_free(RingBuffer *rb);

#endif //RINGBUFFER_H_