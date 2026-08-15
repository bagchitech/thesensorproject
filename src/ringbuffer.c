#include "ringbuffer.h"

int rb_init(RingBuffer *rb, size_t capacity){
    if (!rb || capacity == 0) return -1;
    rb->data = malloc(sizeof(double)*capacity);
    if(!rb->data) return -1;
    rb->capacity = capacity;
    rb->head = 0;
    rb->count =0; 
    return 0;
}

void rb_push(RingBuffer *rb, double value) {
    if (!rb || !rb->data) return;

    rb->data[rb->head] = value;                 
    rb->head = (rb->head + 1) % rb->capacity;   

    if (rb->count < rb->capacity) {
        rb->count++;                           
    }
}

double rb_at(const RingBuffer *rb, size_t index){
    return rb->data[index];
}

// How many values are currently stored.
size_t rb_count(const RingBuffer *rb){
    return rb->count;
}

// Free the buffer's memory.
void rb_free(RingBuffer *rb){
    free(rb->data);
    free(rb);
}