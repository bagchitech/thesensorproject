#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include<stdio.h>

typedef struct{
    double *data;
    size_t capacity;
    size_t count;
    size_t head;

}RingBuffer;

void rb_init(RingBuffer *rb, size_t capacity);

void rb_push(RingBuffer *rb, size_t capacity);

double rb_at(const RingBuffer *rb, size_t index);

void rb_free(RingBuffer *rb);

#endif //RINGBUFFER_H_