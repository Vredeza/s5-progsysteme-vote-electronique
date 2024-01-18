//
// Created by Joan on 02/01/2024.
//

#include "../common/include/circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>

void initCircularBuffer(CircularBuffer *cb, int size) {
    cb->buffer = (Commande *)malloc(sizeof(Commande) * size);
    cb->size = size;
    cb->front = -1;
    cb->rear = -1;
    pthread_mutex_init(&cb->mutex, NULL);
    pthread_cond_init(&cb->not_empty, NULL);
    pthread_cond_init(&cb->not_full, NULL);
}

void cleanupCircularBuffer(CircularBuffer *cb) {
    free(cb->buffer);
    pthread_mutex_destroy(&cb->mutex);
    pthread_cond_destroy(&cb->not_empty);
    pthread_cond_destroy(&cb->not_full);
}

int isFull(CircularBuffer *cb) {
    return (cb->front == (cb->rear + 1) % cb->size);
}

int isEmpty(CircularBuffer *cb) {
    return (cb->front == -1);
}

void enqueue(CircularBuffer *cb, Commande commande) {
    pthread_mutex_lock(&cb->mutex);
    while (isFull(cb)) {
        pthread_cond_wait(&cb->not_full, &cb->mutex);
    }
    if (isEmpty(cb)) {
        cb->front = 0;
        cb->rear = 0;
    } else {
        cb->rear = (cb->rear + 1) % cb->size;
    }
    cb->buffer[cb->rear] = commande;
    pthread_cond_signal(&cb->not_empty);
    pthread_mutex_unlock(&cb->mutex);
}

Commande dequeue(CircularBuffer *cb) {
    pthread_mutex_lock(&cb->mutex);
    while (isEmpty(cb)) {
        pthread_cond_wait(&cb->not_empty, &cb->mutex);
    }
    Commande commande = cb->buffer[cb->front];
    if (cb->front == cb->rear) {
        cb->front = -1;
        cb->rear = -1;
    } else {
        cb->front = (cb->front + 1) % cb->size;
    }
    pthread_cond_signal(&cb->not_full);
    pthread_mutex_unlock(&cb->mutex);
    return commande;
}