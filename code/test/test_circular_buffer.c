#include "../common/include/circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* enqueueThread(void* arg) {
    CircularBuffer* cb = (CircularBuffer*)arg;
    for (int i = 0; i < 5; ++i) {
        usleep(rand() % 400000);
        enqueue(cb, i);
        printf("Enqueued: %d\n", i);
    }
    return NULL;
}

void* dequeueThread(void* arg) {
    CircularBuffer* cb = (CircularBuffer*)arg;
    for (int i = 0; i < 5; ++i) {
        usleep(rand() % 400000);
        int data = dequeue(cb);
        printf("Dequeued: %d\n", data);
    }
    return NULL;
}

int main() {
    srand((unsigned int)time(NULL));
    CircularBuffer cb;
    initCircularBuffer(&cb, 5);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    pthread_t enqueueThreadID, dequeueThreadID;
    pthread_create(&enqueueThreadID, NULL, enqueueThread, &cb);
    pthread_create(&dequeueThreadID, NULL, dequeueThread, &cb);
    pthread_join(enqueueThreadID, NULL);
    pthread_join(dequeueThreadID, NULL);

    printf("isFull: %d\n", isFull(&cb));
    printf("isEmpty: %d\n", isEmpty(&cb));

    cleanupCircularBuffer(&cb);
    return 0;
}