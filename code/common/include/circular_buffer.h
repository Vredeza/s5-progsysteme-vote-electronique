//
// Created by Joan on 02/01/2024.
//

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <pthread.h>

typedef struct {
    int *buffer;
    int size;
    int front;
    int rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} CircularBuffer;

void initCircularBuffer(CircularBuffer *cb, int size);
void cleanupCircularBuffer(CircularBuffer *cb);
int isFull(CircularBuffer *cb);
int isEmpty(CircularBuffer *cb);
void enqueue(CircularBuffer *cb, int data);
int dequeue(CircularBuffer *cb);

#endif // CIRCULAR_BUFFER_H